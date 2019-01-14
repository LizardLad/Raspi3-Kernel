#include <stdlib.h>
#include <string.h>
#include "include/defs.h"
#include "include/access.h"
#include "include/table.h"
#include "include/write.h"
#include "include/misc.h"
#include "include/file_string.h"
#include "include/filelib.h"
#include "include/cache.h"
#include "include/format.h"
#include "include/fl_debug.h"
#include "../include/mutex.h"
#include "../include/printf.h"

//==========================================================================
// Local global vars
//==========================================================================
static FL_FILE			_files[FATLIB_MAX_OPEN_FILES];
static int			_filelib_init = 0;
static int			_filelib_valid = 0;
static struct fatfs		_fs;
static struct fat_list		_open_file_list;
static struct fat_list		_free_file_list;

//==========================================================================
// Macros
//==========================================================================

// Macro for checking if filelib is initialized
#define CHECK_FL_INIT()	{ if (_filelib_init==0) fl_init(); }

#define FL_LOCK(a)	do { if ((a)->fl_lock) (a)->fl_lock(&((a)->lock)); } while (0)
#define FL_UNLOCK(a)	do { if ((a)->fl_unlock) (a)->fl_unlock(&((a)->lock)); } while(0)

//==========================================================================
// Local Functions
//==========================================================================
void _fl_init();


//==========================================================================
// _allocate_file: Find a slot in the open files buffer for a new file
//==========================================================================
static FL_FILE *_allocate_file()
{
	struct fat_node *node = fat_list_pop_head(&_free_file_list);
	if(node)
		fat_list_insert_last(&_open_file_list, node);
	return fat_list_entry(node, FL_FILE, list_node);
}

//==========================================================================
// _check_file_open: Returns true if the file is already open
//==========================================================================
static int _check_file_open(FL_FILE *file)
{
	struct fat_node *node;

	fat_list_for_each(&_open_file_list, node)
	{
		FL_FILE *open_file = fat_list_entry(node, FL_FILE, list_node);
		if(open_file != file)
		{
			if((fatlib_compare_names(open_file->path, file->path)) && (fatlib_compare_names(open_file->filename, file->filename)))
				return 1;
		}
	}
	return 0;
}

//==========================================================================
// _free_file: Free open file handle
//==========================================================================
static void _free_file(FL_FILE *file)
{
	fat_list_remove(&_open_file_list, &file->list_node);
	fat_list_insert_last(&_free_file_list, &file->list_node);
}

//==========================================================================
//				Low Level
//==========================================================================

//==========================================================================
// _open_directory: Cycle through path string to find the start 
// cluster address of the highest subdir.
//==========================================================================
static int _open_directory(char *path, uint32_t *path_cluster)
{
	int levels = fatlib_total_path_levels(path);
	char current_folder[FATLIB_MAX_LONG_FILENAME];
	struct fat_dir_entry short_file_entry;
	uint32_t start_cluster = fatlib_get_root_cluster(&_fs);

	for(int sublevel = 0; sublevel < (levels + 1); sublevel++)
	{
		if(fatlib_get_substring(path, sublevel, current_folder, sizeof(current_folder)) == -1)
			return 0;
		if(fatlib_get_file_entry(&_fs, start_cluster, current_folder, &short_file_entry))
		{
			if(fatlib_entry_is_dir(&short_file_entry))
				start_cluster = ((FAT_HTONS((uint32_t)short_file_entry.first_cluster_high)) << 16) + FAT_HTONS(short_file_entry.first_cluster_low);
			else
				return 0;
		}
		else
			return 0;
	}
	*path_cluster = start_cluster;
	return 1;
}

//==========================================================================
// create_directory: Cycle through path string and create the end directory
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
static int _create_directory(char *path)
{
	FL_FILE *file;
	struct fat_dir_entry short_file_entry;
	char short_filename[FAT_SFN_SIZE_FULL];
	int tail_num = 0;

	file = _allocate_file();
	if(!file)
		return 0;

	//Clear filename
	memset(file->path, '\0', sizeof(file->path));
	memset(file->filename, '\0', sizeof(file->filename));

	//Split into filename and dir path
	if(fatlib_split_path((char *)path, file->path, sizeof(file->path), file->filename, sizeof(file->filename)) == -1)
	{
		_free_file(file);
		return 0;
	}

	if(_check_file_open(file))
	{
		_free_file(file);
		return 0;
	}

	//If file is in the root dir
	if(file->path[0] == 0)
		file->parent_cluster = fatlib_get_root_cluster(&_fs);
	else
	{
		//Find parent directory start cluster
		if(!_open_directory(file->path, &file->parent_cluster))
		{
			_free_file(file);
			return 0;
		}
	}

	//Check if same filename exists in directory
	if(fatlib_get_file_entry(&_fs, file->parent_cluster, file->filename, &short_file_entry) == 1)
	{
		_free_file(file);
		return 0;
	}
	
	file->start_cluster = 0;

	//Create the file space for the folder (at leasto one clisters worth)
	if(!fatlib_allocate_free_space(&_fs, 1, &file->start_cluster, 1))
	{
		_free_file(file);
		return 0;
	}

	//Erase new directory cluster
	memset(file->file_data_sector, 0x00, FAT_SECTOR_SIZE);
	for(int i = 0; i < _fs.sectors_per_cluster; i++)
	{
		if(!fatlib_write_sector(&_fs, file->start_cluster, i, file->file_data_sector))
		{
			_free_file(file);
			return 0;
		}
	}

#if FATLIB_INC_LFN_SUPPORT

	do
	{
		fatlib_lfn_create_sfn(short_filename, file->filename);

		if(tail_num != 0)
			fatlib_lfn_generate_tail((char *)file->short_filename, short_filename, tail_num);
		else
			memcpy(file->short_filename, short_filename, FAT_SFN_SIZE_FULL);

		if(fatlib_sfn_exists(&_fs, file->parent_cluster, (char *)file->short_filename) == 0)
			break;
		tail_num++;
	} while(tail_num < 9999);
	
	if(tail_num == 9999) // Reached the max number of duplicates of sfns
	{
		fatlib_free_cluster_chain(&_fs, file->start_cluster);
		_free_file(file);
		return 0;
	}
#else
	if(!fatlib_lfn_create_sfn(short_filename, file->filename))
	{
		//Del allocated space
		fatlib_free_cluster_chain(&_fs, file->start_cluster);
		_free_file(file);
		return 0;
	}

	memcpy(file->short_filename, short_filename, FAT_SFN_SIZE_FULL);

	if(fatlib_sfn_exists(&_fs, file->parent_cluster, (char *)file->short_filename))
	{
		fatli_free_cluster_chain(&_fs, file->start_cluster);
		_free_file(file);
		return 0;
	}
#endif

	//Add file to disk
	if(!fatlib_add_file_entry(&_fs, file->parent_cluster, (char *)file->filename, (char *)file->short_filename, file->start_cluster, 0, 1))
	{
		fatlib_free_cluster_chain(&_fs, file->start_cluster);
		_free_file(file);
		return 0;
	}

	file->file_length = 0;
	file->byte_num = 0;
	file->file_data_address = 0xFFFFFFFF;
	file->file_data_dirty = 0;
	file->file_length_changed = 0;

	file->last_fat_lookup.cluster_index = 0xFFFFFFFF;
	file->last_fat_lookup.current_cluster = 0xFFFFFFFF;

	fatlib_fat_purge(&_fs);
	_free_file(file);
	return 1;
}
#endif

//==========================================================================
// _open_file: Open a file for reading
//==========================================================================
static FL_FILE *_open_file(const char *path)
{
	//FIXME There is an error where the incorrect file is found

	FL_FILE *file = _allocate_file();
	struct fat_dir_entry short_file_entry;
	if(!file)
		return NULL;

	memset(file->path, '\0', sizeof(file->path));
	memset(file->filename, '\0', sizeof(file->filename));

	if(fatlib_split_path((char *)path, file->path, sizeof(file->path), file->filename, sizeof(file->filename)) == -1)
	{
		_free_file(file);
		return NULL;
	}

	if(_check_file_open(file))
	{
		_free_file(file);
		return NULL;
	}

	if(file->path[0] == 0)
		file->parent_cluster = fatlib_get_root_cluster(&_fs);
	else
	{
		if(!_open_directory(file->path, &file->parent_cluster))
		{
			_free_file(file);
			return NULL;
		}
	}

	if(fatlib_get_file_entry(&_fs, file->parent_cluster, file->filename, &short_file_entry))
	{
		if(fatlib_entry_is_file(&short_file_entry))
		{
			memcpy(file->short_filename, short_file_entry.name, FAT_SFN_SIZE_FULL);
			file->file_length = FAT_HTONL(short_file_entry.file_size);
			file->byte_num = 0;
			file->start_cluster = ((FAT_HTONS((uint32_t)short_file_entry.first_cluster_high)) << 16) + FAT_HTONS(short_file_entry.first_cluster_low);
			file->file_data_address = 0xFFFFFFFF;
			file->file_data_dirty = 0;
			file->file_length_changed = 0;

			//Quick lookip for next link in the chain
			file->last_fat_lookup.cluster_index = 0xFFFFFFFF;
			file->last_fat_lookup.current_cluster = 0xFFFFFFFF;

			fatlib_cache_init(&_fs, file);
			fatlib_fat_purge(&_fs);
			//fatlib_debug_print_file(file);
			return file;
		}
	}
	_free_file(file);
	return NULL;
}

//==========================================================================
// _create_file: Create a new file
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
static FL_FILE *_create_file(const char *filename)
{
	FL_FILE *file;
	struct fat_dir_entry short_file_entry;
	char short_filename[FAT_SFN_SIZE_FULL];

	//Write access?
	if(!_fs.disk_io.write_media)
		return NULL;

	file = _allocate_file();
	if(!file)
		return NULL;

	memset(file->path, '\0', sizeof(file->path));
	memset(file->filename, '\0', sizeof(file->filename));

	if(fatlib_split_path((char *)filename, file->path, sizeof(file->path), file->filename, sizeof(file->filename)) == -1)
	{
		_free_file(file);
		return NULL;
	}

	if(_check_file_open(file))
	{
		_free_file(file);
		return NULL;
	}

	if(file->path[0] == 0)
		file->parent_cluster = fatlib_get_root_cluster(&_fs);
	else
	{
		//Find parent directory start cluster
		if(!_open_directory(file->path, &file->parent_cluster))
		{
			_free_file(file);
			return NULL;
		}
	}

	if(fatlib_get_file_entry(&_fs, file->parent_cluster, file->filename, &short_file_entry) == 1)
	{
		_free_file(file);
		return NULL;
	}

	file->start_cluster = 0;

	if(!fatlib_allocate_free_space(&_fs, 1, &file->start_cluster, 1))
	{
		_free_file(file);
		return NULL;
	}

#if FATLIB_INC_LFN_SUPPORT
	int tail_num = 0;
	do
	{
		fatlib_lfn_create_sfn(short_filename, file->filename);

		if(tail_num != 0)
			fatlib_lfn_generate_tail((char *)file->short_filename, short_filename, tail_num);
		else
			memcpy(file->short_filename, short_filename, FAT_SFN_SIZE_FULL);

		if(fatlib_sfn_exists(&_fs, file->parent_cluster, (char *)file->short_filename) == 0)
			break;
		tail_num++;
	} while(tail_num < 9999);

	if(tail_num == 9999)
	{
		fatlib_free_cluster_chain(&_fs, file->start_cluster);
		_free_file(file);
		return NULL;
	}
#else

	if(!fatlib_lfn_create_sfn(short_filename, file->filename))
	{
		fatlib_free_cluster_chain(&_fs, file->start_cluster);
		_free_file(file);
		return NULL;
	}

	memcpy(file->short_filename, short_filename, FAT_SFN_SIZE_FULL);

	if(fatlib_sfn_exists(&_fs, file->parent_cluster, (char *)file->short_filename))
	{
		fatlib_free_cluster_chain(&_fs, file->start_cluster);
		_free_file(file);
		return NULL:
	}
#endif

	if(!fatlib_add_file_entry(&_fs, file->parent_cluster, (char *)file->filename, (char *)file->short_filename, file->start_cluster, 0, 0))
	{
		fatlib_free_cluster_chain(&_fs, file->start_cluster);
		_free_file(file);
		return NULL;
	}

	file->file_length = 0;
	file->byte_num = 0;
	file->file_data_address = 0xFFFFFFFF;
	file->file_data_dirty = 0;
	file->file_length_changed = 0;
	
	file->last_fat_lookup.cluster_index = 0xFFFFFFFF;
	file->last_fat_lookup.current_cluster = 0xFFFFFFFF;

	fatlib_cache_init(&_fs, file);
	fatlib_fat_purge(&_fs);
	return file;
}
#endif

//==========================================================================
// _read_sectors: Read sector(s) from disk to file
//==========================================================================
static uint32_t _read_sectors(FL_FILE *file, uint32_t offset, uint8_t *buffer, uint32_t count)
{
	uint32_t sector = 0;
	uint32_t cluster_index = 0;
	uint32_t cluster = 0;
	uint32_t lba;

	cluster_index = offset / _fs.sectors_per_cluster;
	sector = offset - (cluster_index * _fs.sectors_per_cluster);
	if((sector + count) > _fs.sectors_per_cluster)
		count = _fs.sectors_per_cluster - sector;

	if(cluster_index == file->last_fat_lookup.cluster_index)
		cluster = file->last_fat_lookup.current_cluster;
	else
	{
		if(cluster_index && cluster_index == file->last_fat_lookup.cluster_index + 1)
		{
			cluster = file->last_fat_lookup.current_cluster;
		}
		else
		{
			cluster = file->start_cluster;
		}

		for(int i = (cluster_index && cluster_index == file->last_fat_lookup.cluster_index + 1)?file->last_fat_lookup.cluster_index:0; i < cluster_index; i++)
		{
			uint32_t next_cluster;
			
			if(!fatlib_cache_get_next_cluster(&_fs, file, i , &next_cluster)) //Branch on uninitialized value! FIXME
			{
				next_cluster = fatlib_find_next_cluster(&_fs, cluster);
				fatlib_cache_set_next_cluster(&_fs, file, i, next_cluster);
			}
			cluster = next_cluster;
		}
		if(cluster != FAT32_LAST_CLUSTER)
		{
			file->last_fat_lookup.current_cluster = cluster;
			file->last_fat_lookup.cluster_index = cluster_index;
		}
	}

	if(cluster == FAT32_LAST_CLUSTER)
		return 0;
	lba = fatlib_lba_of_cluster(&_fs, cluster) + sector;

	if(fatlib_sector_read(&_fs, lba, buffer, count))
		return count;
	else
		return 0;
}

//==========================================================================
//				External API
//==========================================================================

//==========================================================================
// fl_init: Initialise library
//==========================================================================
void fl_init()
{
	fat_list_init(&_free_file_list);
	fat_list_init(&_open_file_list);

	for(int i = 0; i < FATLIB_MAX_OPEN_FILES; i++)
		fat_list_insert_last(&_free_file_list, &_files[i].list_node);

	_filelib_init = 1;
}

//==========================================================================
// fl_attach_locks:
//==========================================================================
void fl_attach_locks(void (*lock)(mutex_r_t*), void (*unlock)(mutex_r_t*))
{
	_fs.fl_lock = lock;
	_fs.fl_unlock = unlock;
}

//==========================================================================
// fl_attach_media:
//==========================================================================
int fl_attach_media(fn_diskio_read rd, fn_diskio_write wr)
{
	int res;

	//If first library call init fatlib
	CHECK_FL_INIT();

	_fs.disk_io.read_media = rd;
	_fs.disk_io.write_media = wr;

	//Init FAT params
	if((res = fatlib_init(&_fs)) != FAT_INIT_OK) 
	{
		printf("FAT_FS: Error couldn't load FAT details (%d)!\n", res);
		return res;
	}
	_filelib_valid = 1;
	return FAT_INIT_OK;
}

//==========================================================================
// fl_shutdown: Call before shutting down the system
//==========================================================================
void fl_shutdown()
{
	CHECK_FL_INIT();

	FL_LOCK(&_fs);
	fatlib_fat_purge(&_fs);
	FL_UNLOCK(&_fs);
}

//==========================================================================
// fopen: Open or Create a file for reading or writing
//==========================================================================
void *fl_fopen(const char *path, const char *mode)
{
	FL_FILE *file;
	uint8_t flags = 0;

	CHECK_FL_INIT();

	if(!_filelib_valid)
		return NULL;

	if(!path || !mode)
		return NULL;

	// Supported Modes:
	// "r" Open a file for reading.
	// The file must exist.
	// "w" Create an empty file for writing.
	// If a file with the same name already exists its content is erased and the file is treated as a new empty file.
	// "a" Append to a file.
	// Writing operations append data at the end of the file.
	// The file is created if it does not exist.
	// "r+" Open a file for update both reading and writing.
	// The file must exist.
	// "w+" Create an empty file for both reading and writing.
	// If a file with the same name already exists its content is erased and the file is treated as a new empty file.
	// "a+" Open a file for reading and appending.
	// All writing operations are performed at the end of the file, protecting the previous content to be overwritten.
	// You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading, but writing operations
	// will move it back to the end of file.
	// The file is created if it does not exist.

	for(int i = 0; i < (int)strlen(mode); i++)
	{
		switch(mode[i])
		{
			case 'r':
			case 'R':
				flags |= FILE_READ;
				break;
			case 'w':
			case 'W':
				flags |= FILE_WRITE;
				flags |= FILE_ERASE;
				flags |= FILE_CREATE;
				break;
			case 'a':
			case 'A':
				flags |= FILE_WRITE;
				flags |= FILE_APPEND;
				flags |= FILE_CREATE;
				break;
			case '+':
				if(flags & FILE_READ)
					flags |= FILE_WRITE;
				else if(flags & FILE_WRITE)
				{
					flags |= FILE_READ;
					flags |= FILE_ERASE;
					flags |= FILE_CREATE;
				}
				else if(flags & FILE_APPEND)
				{
					flags |= FILE_READ;
					flags |= FILE_WRITE;
					flags |= FILE_APPEND;
					flags |= FILE_CREATE;
				}
				break;
			case 'b':
			case 'B':
				flags |= FILE_BINARY;
				break;
		}
	}

	file = NULL;

#if FATLIB_INC_WRITE_SUPPORT == 0
	flags &= ~(FILE_CREATE | FILE_WRITE | FILE_APPEND);
#endif
	//No write access remove some flags
	if(!_fs.disk_io.write_media)
		flags &= ~(FILE_CREATE | FILE_WRITE | FILE_APPEND);

	FL_LOCK(&_fs);
	
	//Read
	if(flags & FILE_READ)
		file = _open_file(path);

	//Create new
#if FATLIB_INC_WRITE_SUPPORT
	if(!file && (flags & FILE_CREATE))
		file = _create_file(path);
#endif

	//Write existing
	if(!(flags & FILE_READ))
		if((flags & FILE_CREATE) && !file)
			if(flags & (FILE_WRITE | FILE_APPEND))
				file = _open_file(path);
	if(file)
		file->flags = flags;

	FL_UNLOCK(&_fs);
	return file;
}

//==========================================================================
// _write_sectors: Write sector(s) to disk
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
static uint32_t _write_sectors(FL_FILE *file, uint32_t offset, uint8_t *buf, uint32_t count)
{
	uint32_t sector_number = 0;
	uint32_t cluster_index = 0;
	uint32_t cluster = 0;
	uint32_t last_cluster = FAT32_LAST_CLUSTER;
	uint32_t lba;
	uint32_t total_write_count = count;
	uint32_t i;

	cluster_index = offset / _fs.sectors_per_cluster;
	sector_number = offset - (cluster_index * _fs.sectors_per_cluster);

	if((sector_number + count) > _fs.sectors_per_cluster)
		count = _fs.sectors_per_cluster - sector_number;

	if(cluster_index == file->last_fat_lookup.cluster_index)
		cluster = file->last_fat_lookup.current_cluster;
	//Else walk the chain
	else
	{
		if(cluster_index && cluster_index == file->last_fat_lookup.cluster_index + 1)
		{
			i = file->last_fat_lookup.cluster_index;
			cluster = file->last_fat_lookup.current_cluster;
		}
		//Start the search from the beggining
		else
		{
			i = 0;
			cluster = file->start_cluster;
		}

		for(; i < cluster_index; i++)
		{
			uint32_t next_cluster;

			if(!fatlib_cache_get_next_cluster(&_fs, file, i, &next_cluster))
			{
				next_cluster = fatlib_find_next_cluster(&_fs, cluster);
				fatlib_cache_set_next_cluster(&_fs, file, i, next_cluster);
			}
			last_cluster = cluster;
			cluster = next_cluster;

			if(cluster == FAT32_LAST_CLUSTER)
				break;
		}
		if(cluster == FAT32_LAST_CLUSTER)
		{
			if(!fatlib_add_free_space(&_fs, &last_cluster, (total_write_count + _fs.sectors_per_cluster - 1) / _fs.sectors_per_cluster))
				return 0;
			cluster = last_cluster;
		}

		file->last_fat_lookup.current_cluster = cluster;
		file->last_fat_lookup.cluster_index = cluster_index;
	}

	lba = fatlib_lba_of_cluster(&_fs, cluster) + sector_number;

	if(fatlib_sector_write(&_fs, lba, buf, count))
		return count;
	else
		return 0;
}
#endif

//==========================================================================
// fl_fflush: Flush un-written data to the file
//==========================================================================
int fl_fflush(void *f)
{
#if FATLIB_INC_WRITE_SUPPORT
	FL_FILE *file = (FL_FILE *)f;

	CHECK_FL_INIT();

	if(file)
	{
		FL_LOCK(&_fs);

		if(file->file_data_dirty)
		{
			if(_write_sectors(file, file->file_data_address, file->file_data_sector, 1))
				file->file_data_dirty = 0;
		}

		FL_UNLOCK(&_fs);
	}
#endif
	return 0;
}

//==========================================================================
// fl_fclose: Close an open file
//==========================================================================
void fl_fclose(void *f)
{
	FL_FILE *file = (FL_FILE *)f;

	CHECK_FL_INIT();

	if(file)
	{
		FL_LOCK(&_fs);

		fl_fflush(f);

		if(file->file_length_changed)
		{
#if FATLIB_INC_WRITE_SUPPORT
			fatlib_update_file_length(&_fs, file->parent_cluster, (char *)file->short_filename, file->file_length);
#endif
			file->file_length_changed = 0;
		}

		file->byte_num = 0;
		file->file_length = 0;
		file->start_cluster = 0;
		file->file_data_address = 0xFFFFFFFF;
		file->file_data_dirty = 0;
		file->file_length_changed = 0;

		_free_file(file);

		fatlib_fat_purge(&_fs);

		FL_UNLOCK(&_fs);
	}
}

//==========================================================================
// fl_fgetc: Get a character in the stream
//==========================================================================
int fl_fgetc(void *f)
{
	uint8_t data;
	int res = fl_fread(&data, 1, 1, f);
	if(res == 1)
		return (int)data;
	else
		return res;
}

//==========================================================================
// fl_fgets: Get a sring from a stream
//==========================================================================
char *fl_fgets(char *s, int n, void *f)
{
	int index = 0;

	if(n > 0)
	{
		while(index < (n - 1))
		{
			int ch = fl_fgetc(f);

			if(ch < 0)
				break;

			s[index++] = (char)ch;

			if(ch == '\n')
				break;
		}

		if(index > 0)
			s[index] = '\0';
	}

	return (index > 0) ? s : NULL;
}

//==========================================================================
// fl_fread: Read a block of data from the file
//==========================================================================
int fl_fread(void *buffer, int size, int length, void *f)
{
	uint32_t sector;
	uint32_t offset;
	int copy_count;
	int count = size * length;
	int bytes_read = 0;

	FL_FILE *file = (FL_FILE *)f;

	CHECK_FL_INIT();

	if(buffer == NULL || file == NULL)
		return -1;

	if(!(file->flags & FILE_READ))
		return -1;

	if(!count)
		return 0;

	if(file->byte_num >= file->file_length)
		return -1;

	if((file->byte_num + count) > file->file_length)
		count = file->file_length - file->byte_num;

	sector = file->byte_num / FAT_SECTOR_SIZE;
	offset = file->byte_num % FAT_SECTOR_SIZE;

	while(bytes_read < count)
	{
		if((offset == 0) && ((count - bytes_read) >= FAT_SECTOR_SIZE))
		{
			uint32_t sectors_read = _read_sectors(file, sector, (uint8_t *)((uint8_t *)buffer + bytes_read), (count - bytes_read) / FAT_SECTOR_SIZE);
			if(sectors_read)
			{
				copy_count = FAT_SECTOR_SIZE * sectors_read;

				sector += sectors_read;
				offset = 0;
			}
			else
				break;
		}
		else
		{
			if(file->file_data_address != sector)
			{
				if(file->file_data_dirty)
					fl_fflush(file);

				if(!_read_sectors(file, sector, file->file_data_sector, 1))
					break;

				file->file_data_address = sector;
				file->file_data_dirty = 0;
			}

			copy_count = FAT_SECTOR_SIZE - offset;

			if(copy_count > (count - bytes_read))
				copy_count = (count - bytes_read);

			memcpy((uint8_t * )((uint8_t *)buffer + bytes_read), (uint8_t *)(file->file_data_sector + offset), copy_count);

			sector++;
			offset = 0;
		}

		bytes_read += copy_count;
		file->byte_num += copy_count;
	}

	return bytes_read;
}

//==========================================================================
// fl_fseek: Seek to a specific place in the file
//==========================================================================
int fl_fseek(void *f, long offset, int origin)
{
	FL_FILE *file = (FL_FILE *)f;
	int res = -1;
	
	CHECK_FL_INIT();

	if(!file)
		return -1;

	if(origin == SEEK_END && offset != 0)
		return -1;

	FL_LOCK(&_fs);

	//Invalidate file buffer
	file->file_data_address = 0xFFFFFFFF;
	file->file_data_dirty = 0;

	if(origin == SEEK_SET)
	{
		file->byte_num = (uint32_t)offset;
		if(file->byte_num > file->file_length)
			file->byte_num = file->file_length;
		res = 0;
	}
	else if(origin == SEEK_CUR)
	{
		//Positive shift
		if(offset >= 0)
		{
			file->byte_num += offset;

			if(file->byte_num > file->file_length)
				file->byte_num = file->file_length;
		}
		//Negative shift
		else
		{
			offset = -offset;

			if((uint32_t)offset > file->byte_num)
				file->byte_num = 0;
			else
				file->byte_num -= offset;
		}
		res = 0;
	}
	else if(origin == SEEK_END)
	{
		file->byte_num = file->file_length;
		res = 0;
	}
	else 
		res = -1;

	FL_UNLOCK(&_fs);

	return res;
}

//==========================================================================
// fl_fgetpos: Get the current file position
//==========================================================================
int fl_fgetpos(void *f, uint32_t *position)
{
	FL_FILE *file = (FL_FILE *)f;

	if(!file)
		return -1;

	FL_LOCK(&_fs);

	*position = file->byte_num;

	FL_UNLOCK(&_fs);

	return 0;
}

//==========================================================================
// fl_ftell: Get the current file position
//==========================================================================
long fl_ftell(void *f)
{
	uint32_t pos = 0;
	fl_fgetpos(f, &pos);
	return (long)pos;
}

//==========================================================================
// fl_feof: Is the file pointer at the end of the stream?
//==========================================================================
int fl_feof(void *f)
{
	FL_FILE *file = (FL_FILE *)f;
	int res;

	if(!file)
		return -1;

	FL_LOCK(&_fs);

	if(file->byte_num == file->file_length)
		res = EOF;
	else
		res = 0;

	FL_UNLOCK(&_fs);
	return res;
}

//==========================================================================
// fl_fwrite: Write a block of data to the stream
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fl_fwrite(const void *data, int size, int count, void *f)
{
	FL_FILE *file = (FL_FILE *)f;
	uint32_t sector;
	uint32_t offset;
	uint32_t length = (size * count);
	uint8_t *buffer = (uint8_t *)data;
	uint32_t bytes_written = 0;
	uint32_t copy_count;

	CHECK_FL_INIT();

	if(!file) return -1;

	FL_LOCK(&_fs);

	if(!(file->flags & FILE_WRITE))
	{
		FL_UNLOCK(&_fs);
		return -1;
	}

	if(file->flags & FILE_APPEND)
		file->byte_num = file->file_length;

	sector = file->byte_num / FAT_SECTOR_SIZE;
	offset = file->byte_num % FAT_SECTOR_SIZE;

	while(bytes_written < length)
	{
		if((offset == 0) && ((length - bytes_written) >= FAT_SECTOR_SIZE))
		{
			uint32_t sectors_written;

			if(file->file_data_address != 0xFFFFFFFF)
			{
				if(file->file_data_dirty)
					fl_fflush(file);
				file->file_data_address = 0xFFFFFFFF;
				file->file_data_dirty = 0;
			}

			sectors_written = _write_sectors(file, sector, (uint8_t *)(buffer + bytes_written), (length - bytes_written) / FAT_SECTOR_SIZE);
			copy_count = FAT_SECTOR_SIZE * sectors_written;

			bytes_written += copy_count;
			file->byte_num += copy_count;

			sector += sectors_written;
			offset = 0;

			if(!sectors_written)
				break;
		}
		else
		{
			copy_count = FAT_SECTOR_SIZE - offset;

			if(copy_count > (length - bytes_written))
				copy_count = (length - bytes_written);

			if(file->file_data_address != sector)
			{
				if(file->file_data_dirty)
					fl_fflush(file);

				if(copy_count != FAT_SECTOR_SIZE)
				{
					if(!_read_sectors(file, sector, file->file_data_sector, 1))
						memset(file->file_data_sector, 0x00, FAT_SECTOR_SIZE);
				}
				file->file_data_address = sector;
				file->file_data_dirty = 0;
			}

			memcpy((uint8_t *)(file->file_data_sector + offset), (uint8_t *)(buffer + bytes_written), copy_count);

			file->file_data_dirty = 1;
			bytes_written += copy_count;
			file->byte_num += copy_count;

			sector++;
			offset = 0;
		}
	}

	if(file->byte_num > file->file_length)
	{
		file->file_length = file->byte_num;
		file->file_length_changed = 1;
	}
#if FATLIB_INC_TIME_DATE_SUPPORT
	//This will force the update of the date and time
	file->file_length_changed = 1;
#endif

	FL_UNLOCK(&_fs);

	return (size * count);
}
#endif

//==========================================================================
// fl_fputc: Write a character to the stream
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fl_fputc(int c, void *f)
{
	uint8_t data = (uint8_t)c;
	int res = fl_fwrite(&data, 1, 1, f);
	if(res == 1)
		return c;
	else
		return res;
}
#endif

//==========================================================================
// fl_fputs: Write a character string to the stream
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fl_fputs(const char *string, void *f)
{
	int len = strlen(string);
	int res = fl_fwrite(string, 1, len, f);

	if(res == len)
		return len;
	else
		return res;
}
#endif

//==========================================================================
// fl_remove: Remove a file from the filesystem
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fl_remove(const char *filename)
{
	FL_FILE *file;
	int res = -1;

	FL_LOCK(&_fs);

	file = fl_fopen((char *)filename, "r");
	if(file)
	{
		if(fatlib_free_cluster_chain(&_fs, file->start_cluster))
		{
			if(fatlib_mark_file_deleted(&_fs, file->parent_cluster, (char *)file->short_filename))
			{
				fl_fclose(file);
				res = 0;
			}
		}
	}

	FL_UNLOCK(&_fs);
	return res;
}
#endif

//==========================================================================
//fl_created_directory: create a directory based on a path
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fl_created_directory(const char *path)
{
	int res;

	CHECK_FL_INIT();

	FL_LOCK(&_fs);
	res = _create_directory((char *)path);
	FL_UNLOCK(&_fs);
	return res;
}
#endif

//==========================================================================
// fl_list_directory: List a directory based on a path
//==========================================================================
#if FATLIB_DIR_LIST_SUPPORT
void fl_list_directory(const char *path)
{
	FL_DIR dir_stat;

	CHECK_FL_INIT();

	FL_LOCK(&_fs);

	printf("\nDirectory %s\n", path);

	if(fl_opendir(path, &dir_stat))
	{
		fl_dirent dir_entry;

		while(fl_readdir(&dir_stat, &dir_entry) == 0)
		{
#if FATLIB_INC_TIME_DATE_SUPPORT
			int d, m, y, h, mn, s;
			fatlib_convert_from_fat_time(dir_entry.write_time, &h, &mn, &s);
			fatlib_convert_from_fat_date(dir_entry.write_date, &d, &m, &y);
			printf("%02d/%02d/%04d  %02d:%02d", d, m, y, h, mn);
#endif
			if(dir_entry.is_dir)
			{
				printf("%s <DIR>\n", dir_entry.filename);
			}
			else
			{
				printf("%s [%d bytes]\n", dir_entry.filename, dir_entry.size);
			}
		}
		fl_closedir(&dir_stat);
	}
	FL_UNLOCK(&_fs);
}
#endif

//==========================================================================
// fl_opendir: Opens a directory for listing
//==========================================================================
#if FATLIB_DIR_LIST_SUPPORT
FL_DIR *fl_opendir(const char *path, FL_DIR *dir)
{
	int levels;
	int res = 1;
	uint32_t cluster = FAT32_INVALID_CLUSTER;

	CHECK_FL_INIT();

	FL_LOCK(&_fs);
	
	levels = fatlib_total_path_levels((char *)path) + 1;

	if(levels == 0) //Never executes FIXME
		cluster = fatlib_get_root_cluster(&_fs);
	else
		res = _open_directory((char *)path, &cluster);

	if(res)
		fatlib_list_directory_start(&_fs, dir, cluster);

	FL_UNLOCK(&_fs);
	return cluster != FAT32_INVALID_CLUSTER ? dir : 0;
}
#endif

//==========================================================================
// fl_readdir: Get next item in directory
//==========================================================================
#if FATLIB_DIR_LIST_SUPPORT
int fl_readdir(FL_DIR *dirls, fl_dirent *entry)
{
	int res = 0;

	CHECK_FL_INIT();
	FL_LOCK(&_fs);

	res = fatlib_list_directory_next(&_fs, dirls, entry);

	FL_UNLOCK(&_fs);
	return res ? 0 : -1;
}
#endif

//==========================================================================
// fl_close_dir: Close directory after listing
//==========================================================================
#if FATLIB_DIR_LIST_SUPPORT
int fl_closedir(FL_DIR *dir)
{
	//Not used
	return 0;
}
#endif

//==========================================================================
// fl_is_dir: Is this a directory?
//==========================================================================
#if FATLIB_DIR_LIST_SUPPORT
int fl_is_dir(const char *path)
{
	int res = 0;
	FL_DIR dir;

	if(fl_opendir(path, &dir))
	{
		res = 1;
		fl_closedir(&dir);
	}
	return res;
}
#endif

//==========================================================================
// fl_format: Format a partition with either FAT16 or FAT32 based in size
//==========================================================================
#if FATLIB_INC_FORMAT_SUPPORT
int fl_format(uint32_t volume_sectors, const char *name)
{
	return fatlib_format(&_fs, volume_sectors, name);
}
#endif

//==========================================================================
// fl_get_fs:
//==========================================================================
#ifdef FATLIB_INT_TEST_HOOKS
struct fatfs *fl_get_fs()
{
	return &_fs;
}
#endif
