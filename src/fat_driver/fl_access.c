#include <string.h>
#include <stdint.h>
#include "include/defs.h"
#include "include/access.h"
#include "include/table.h"
#include "include/write.h"
#include "include/file_string.h"
#include "include/misc.h"
#include "include/options.h"

//==========================================================================
// fatlib_init: Load FAT Params
//==========================================================================
int fatlib_init(struct fatfs *fs)
{
	uint8_t num_of_fats;
	uint16_t reserved_sectors;
	uint32_t FATSz;
	uint32_t root_dir_sectors;
	uint32_t total_sectors;
	uint32_t data_sectors;
	uint32_t count_of_clusters;
	uint8_t valid_partition = 0;

	fs->current_sector.address = FAT32_INVALID_CLUSTER;
	fs->current_sector.dirty = 0;

	fs->next_free_cluster = 0; //Invalid

	fatlib_fat_init(fs);

	if(!fs->disk_io.read_media)
		return FAT_INIT_MEDIA_ACCESS_ERROR;

	//MBR: Sector 0 on the disk
	//NOTE: Some removable media doesn't have this.
	
	//Load MBR (LBA 0) into the 512 byte buffer
	if(!fs->disk_io.read_media(0, fs->current_sector.sector, 1))
		return FAT_INIT_MEDIA_ACCESS_ERROR;

	//Make sure 0x55 and 0xAA are at the end of the sector
	// (this should be the case regardless of the MBR or boot sector)
	if(fs->current_sector.sector[SIGNATURE_POSITION] != 0x55 || fs->current_sector.sector[SIGNATURE_POSITION + 1] != 0xAA)
		return FAT_INIT_INVALID_SIGNATURE;

	//Now check again using the access function to prove endian conversion function
	if(GET_16BIT_WORD(fs->current_sector.sector, SIGNATURE_POSITION) != SIGNATURE_VALUE)
		return FAT_INIT_ENDIAN_ERROR;

	if(sizeof(struct fat_dir_entry) != FAT_DIR_ENTRY_SIZE)
		return FAT_INIT_STRUCT_PACKING;

	switch(fs->current_sector.sector[PARTITION1_TYPECODE_LOCATION])
	{
		case 0x0B:
		case 0x06:
		case 0x0C:
		case 0x0E:
		case 0x0F:
		case 0x05:
			valid_partition = 1;
			break;
		case 0x00:
			valid_partition = 0;
			break;
		default:
			if(fs->current_sector.sector[PARTITION1_TYPECODE_LOCATION] <= 0x06)
				valid_partition = 1;
			break;
	}

	if(valid_partition)
		fs->lba_begin = GET_32BIT_WORD(fs->current_sector.sector, PARTITION1_LBA_BEGIN_LOCATION);
	else
		fs->lba_begin = 0;

	//Load volume 1 table into sector buffer
	//If MBR less drive may already be in buffer
	if(!fs->disk_io.read_media(fs->lba_begin, fs->current_sector.sector, 1))
		return FAT_INIT_MEDIA_ACCESS_ERROR;

	if(GET_16BIT_WORD(fs->current_sector.sector, 0x0B) != FAT_SECTOR_SIZE)
		return FAT_INIT_INVALID_SECTOR_SIZE;

	fs->sectors_per_cluster = fs->current_sector.sector[BPB_SECPERCLUS];
	reserved_sectors = GET_16BIT_WORD(fs->current_sector.sector, BPB_RSVDSECCNT);
	num_of_fats = fs->current_sector.sector[BPB_NUMFATS];
	fs->root_entry_count = GET_16BIT_WORD(fs->current_sector.sector, BPB_ROOTENTCNT);

	if(GET_16BIT_WORD(fs->current_sector.sector, BPB_FATSZ16) != 0)
		fs->fat_sectors = GET_16BIT_WORD(fs->current_sector.sector, BPB_FATSZ16);
	else
		fs->fat_sectors = GET_32BIT_WORD(fs->current_sector.sector, BPB_FAT32_FATSZ32);

	fs->rootdir_first_cluster = GET_32BIT_WORD(fs->current_sector.sector, BPB_FAT32_ROOTCLUS);
	fs->fs_info_sector = GET_16BIT_WORD(fs->current_sector.sector, BPB_FAT32_FSINFO);

	fs->rootdir_first_sector = reserved_sectors + (num_of_fats * fs->fat_sectors);
	fs->rootdir_sectors = ((fs->root_entry_count * 32) + (FAT_SECTOR_SIZE - 1)) / FAT_SECTOR_SIZE;

	fs->fat_begin_lba = fs->lba_begin + reserved_sectors;

	fs->cluster_begin_lba = fs->fat_begin_lba + (num_of_fats * fs->fat_sectors);
	if(GET_16BIT_WORD(fs->current_sector.sector, 0x1FE) != 0xAA55)
		return FAT_INIT_INVALID_SIGNATURE;

	root_dir_sectors = ((GET_16BIT_WORD(fs->current_sector.sector, BPB_ROOTENTCNT) * 32) + (GET_16BIT_WORD(fs->current_sector.sector, BPB_BYTESPERSEC) - 1)) / GET_16BIT_WORD(fs->current_sector.sector, BPB_BYTESPERSEC);

	if(GET_16BIT_WORD(fs->current_sector.sector, BPB_FATSZ16) != 0)
		FATSz = GET_16BIT_WORD(fs->current_sector.sector, BPB_FATSZ16);
	else
		FATSz = GET_32BIT_WORD(fs->current_sector.sector, BPB_FAT32_FATSZ32);

	if(GET_16BIT_WORD(fs->current_sector.sector, BPB_TOTSEC16) != 0)
		total_sectors = GET_16BIT_WORD(fs->current_sector.sector, BPB_TOTSEC16);
	else
		total_sectors = GET_32BIT_WORD(fs->current_sector.sector, BPB_TOTSEC32);

	data_sectors = total_sectors - (GET_16BIT_WORD(fs->current_sector.sector, BPB_RSVDSECCNT) + (fs->current_sector.sector[BPB_NUMFATS] * FATSz) + root_dir_sectors);

	if(fs->sectors_per_cluster != 0)
	{
		count_of_clusters = data_sectors / fs->sectors_per_cluster;

		if(count_of_clusters < 4085)
			return FAT_INIT_WRONG_FILESYS_TYPE;
		else if(count_of_clusters < 65525)
		{
			fs->rootdir_first_cluster = 0;

			fs->fat_type = FAT_TYPE_16;
			return FAT_INIT_OK;
		}
		else
		{
			fs->fat_type = FAT_TYPE_32;
			return FAT_INIT_OK;
		}
	}
	else
		return FAT_INIT_WRONG_FILESYS_TYPE;
}

//==========================================================================
// fatlib_lba_of_cluster: This function converts a clus num into a sector / lba
//==========================================================================
uint32_t fatlib_lba_of_cluster(struct fatfs *fs, uint32_t cluster_number)
{
	if(fs->fat_type == FAT_TYPE_16)
		return (fs->cluster_begin_lba + (fs->root_entry_count * 32 / FAT_SECTOR_SIZE) + ((cluster_number - 2) * fs->sectors_per_cluster));
	else
		return ((fs->cluster_begin_lba + ((cluster_number - 2) * fs->sectors_per_cluster)));
}

//==========================================================================
// fatlib_sector_read:
//==========================================================================
int fatlib_sector_read(struct fatfs *fs, uint32_t lba, uint8_t *target, uint32_t count)
{
	return fs->disk_io.read_media(lba, target, count);
}

//==========================================================================
// fatlib_sector_write:
//==========================================================================
int fatlib_sector_write(struct fatfs *fs, uint32_t lba, uint8_t *target, uint32_t count)
{
	return fs->disk_io.write_media(lba, target, count);
}

//==========================================================================
// fatlib_sector_reader: From the provided start_cluster and sector offset
// Returns True if success, returns false if not (including if read out of range)
//==========================================================================
int fatlib_sector_reader(struct fatfs *fs, uint32_t start_cluster, uint32_t offset, uint8_t *target)
{
	uint32_t sector_to_read = 0;
	uint32_t cluster_to_read = 0;
	uint32_t cluster_chain = 0;
	uint32_t lba;

	if(fs->fat_type == FAT_TYPE_16 && start_cluster == 0)
	{
		if(offset < fs->rootdir_sectors)
			lba = fs->lba_begin + fs->rootdir_first_sector + offset;
		else
			return 0;
	}
	else
	{
		cluster_chain = start_cluster;

		cluster_to_read = offset / fs->sectors_per_cluster;
		sector_to_read = offset - (cluster_to_read * fs->sectors_per_cluster);

		for(uint32_t i = 0; i < cluster_to_read; i++)
			cluster_chain = fatlib_find_next_cluster(fs, cluster_chain);

		if(cluster_chain == FAT32_LAST_CLUSTER)
			return 0;
		lba = fatlib_lba_of_cluster(fs, cluster_chain) + sector_to_read;
	}

	if(target)
		return fs->disk_io.read_media(lba, target, 1);
	else if(lba != fs->current_sector.address)
	{
		fs->current_sector.address = lba;
		return fs->disk_io.read_media(fs->current_sector.address, fs->current_sector.sector, 1);
	}
	else
		return 1;
}

//==========================================================================
// fatlib_read_sector: Read from the provided cluster and sector offset
// Returns true if success, returns false if not
//==========================================================================
int fatlib_read_sector(struct fatfs *fs, uint32_t cluster, uint32_t sector, uint8_t *target)
{
	//FAT16 root dir
	if(fs->fat_type == FAT_TYPE_16 && cluster == 0)
	{
		uint32_t lba;

		if(sector < fs->rootdir_sectors)
			lba = fs->lba_begin + fs->rootdir_first_sector + sector;
		else
			return 0;

		if(target)
			return fs->disk_io.read_media(lba, target, 1);
		else
		{
			fs->current_sector.address = lba;
			return fs->disk_io.read_media(fs->current_sector.address, fs->current_sector.sector, 1);
		}
	}
	//FAT16/32 Other
	else
	{
		if(target)
		{
			uint32_t lba = fatlib_lba_of_cluster(fs, cluster) + sector;
			return fs->disk_io.read_media(lba, target, 1);
		}
		else
		{
			fs->current_sector.address = fatlib_lba_of_cluster(fs, cluster) + sector;
			return fs->disk_io.read_media(fs->current_sector.address, fs->current_sector.sector, 1);
		}
	}
}

//==========================================================================
// fatlib_write_sector: Write to the provided cluster and sector offset
// Returns true of success, returns false if not
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fatlib_write_sector(struct fatfs *fs, uint32_t cluster, uint32_t sector, uint8_t *target)
{
	if(!fs->disk_io.write_media)
		return 0;

	if(fs->fat_type == FAT_TYPE_16 && cluster == 0)
	{
		uint32_t lba;

		if(sector < fs->rootdir_sectors)
			lba = fs->lba_begin + fs->rootdir_first_sector + sector;
		else
			return 0;

		if(target)
		{
			return fs->disk_io.write_media(lba, target, 1);
		}
		else
		{
			fs->current_sector.address = lba;
			return fs->disk_io.write_media(fs->current_sector.address, fs->current_sector.sector, 1);
		}
	}
	//FAT16/32 Other
	else
	{
		if(target)
		{
			uint32_t lba = fatlib_lba_of_cluster(fs, cluster) + sector;
			return fs->disk_io.write_media(lba, target, 1);
		}
		else
		{
			fs->current_sector.address = fatlib_lba_of_cluster(fs, cluster) + sector;
			return fs->disk_io.write_media(fs->current_sector.address, fs->current_sector.sector, 1);
		}
	}
}
#endif

//==========================================================================
// fatlib_show_details: Show the details about the filesystem
//==========================================================================
void fatlib_show_details(struct fatfs *fs)
{
#ifndef printf
	printf("FAT details:\n");
	printf("    |Type: %s|", (fs->fat_type == FAT_TYPE_32) ? "FAT32" : "FAT16");
	printf("Root dir first cluster: %x|", fs->rootdir_first_cluster);
	printf("FAT begin LBA: 0x%x|", fs->fat_begin_lba);
	printf("Cluster begin LBA: 0x%x|", fs->cluster_begin_lba);
	printf("Sectors per cluster: %d|\n", fs->sectors_per_cluster);
#endif
}

//==========================================================================
// fatlib_get_root_cluster: Get the root dir cluser
//==========================================================================
uint32_t fatlib_get_root_cluster(struct fatfs *fs)
{
	return fs->rootdir_first_cluster;
}

//==========================================================================
// fatlib_get_file_entry: Find the file entry for a filename
//==========================================================================
uint32_t fatlib_get_file_entry(struct fatfs *fs, uint32_t cluster, char *name_to_find, struct fat_dir_entry *short_file_entry)
{
	uint8_t item = 0;
	uint16_t record_offset = 0;
	uint8_t i = 0;
	int x = 0;
	char *long_filename = NULL;
	char short_filename[13];
	struct lfn_cache lfn;
	int dot_required = 0;
	struct fat_dir_entry *directory_entry;

	fatlib_lfn_cache_init(&lfn, 1);

	while(1)
	{
		if(fatlib_sector_reader(fs, cluster, x++, 0))
		{
			for(item =0; item < FAT_DIR_ENTRIES_PER_SECTOR; item++)
			{
				record_offset = FAT_DIR_ENTRY_SIZE * item;
				directory_entry = (struct fat_dir_entry *)(fs->current_sector.sector + record_offset);
#if FATLIB_INC_LFN_SUPPORT
				if(fatlib_entry_lfn_text(directory_entry))
					fatlib_lfn_cache_entry(&lfn, fs->current_sector.sector + record_offset);

				else if(fatlib_entry_lfn_invalid(directory_entry))
				{
					fatlib_lfn_cache_init(&lfn, 0);
					printf("[DEBUG] LFN entry invalid in function fatlib_get_file_entry\n");
				}

				else if(fatlib_entry_lfn_exists(&lfn, directory_entry))
				{
					long_filename = fatlib_lfn_cache_get(&lfn);
					if(fatlib_compare_names(long_filename, name_to_find))
					{
						printf("[DEBUG] Compared the names and file found!\n");
						printf("[DEBUG] Long filename is: %s\n", long_filename);
						printf("[DEBUG] Name to find is: %s\n", name_to_find); 
						memcpy(short_file_entry, directory_entry, sizeof(struct fat_dir_entry));
						return 1;
					}
					fatlib_lfn_cache_init(&lfn, 0);
				}
				else
#endif
				if(fatlib_entry_sfn_only(directory_entry))
				{
					memset(short_filename, 0, sizeof(short_filename));

					for(i = 0; i < 8; i++)
						short_filename[i] = directory_entry->name[i];

					dot_required = 0;
					for(i = 8; i < 11; i++)
					{
						short_filename[i+1] = directory_entry->name[i];
						if(directory_entry->name[i] != ' ')
							dot_required = 1;
					}

					if(dot_required)
					{
						if(short_filename[0] != '.')
							short_filename[8] = '.';
						else
							short_filename[8] = ' ';
					}
					else
						short_filename[8] = ' ';

					if(fatlib_compare_names(short_filename, name_to_find))
					{
						memcpy(short_file_entry, directory_entry, sizeof(struct fat_dir_entry));
						return 1;
					}
					fatlib_lfn_cache_init(&lfn, 0);
				}
			}
		}
		else
			break;
	}
	return 0;
}

//==========================================================================
// fatlib_sfn_exists: Check if a short filename exists.
// NOTE: short name is XXXXXXXXYYY not XXXXXXXX.YYY
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fatlib_sfn_exists(struct fatfs *fs, uint32_t cluster, char *shortname)
{
	uint8_t item = 0;
	uint16_t record_offset = 0;
	int x = 0;
	struct fat_dir_entry *directory_entry;

	while(1)
	{
		if(fatlib_sector_reader(fs, cluster, x++, 0))
		{
			for(item = 0; item < FAT_DIR_ENTRIES_PER_SECTOR; item++)
			{
				record_offset = FAT_DIR_ENTRY_SIZE * item;
				directory_entry = (struct fat_dir_entry *)(fs->current_sector.sector  + record_offset);

#if FATLIB_INT_LFN_SUPPORT
				if(fatlib_entry_lfn_text(directory_entry));
				else if(fatlib_entry_lfn_invalid(directory_entry));
				else
#endif
				if(fatlib_entry_sfn_only(directory_entry))
				{
					if(strncmp((const char *)directory_entry->name, shortname, 11) == 0)
						return 1;
				}
			}
		}
		else
			break;
	}
	return 0;
}
#endif

//==========================================================================
// fatlib_update_timestamps: Update date/time details
//==========================================================================
#if FATLIB_INC_TIME_DATE_SUPPORT
int fatlib_update_timestamps(struct fat_dir_entry *directory_entry, int create, int modify, int access)
{
	time_t time_now;
	struct tm *time_info;
	uint16_t fat_time;
	uint16_t fat_date;

	time(&time_now);
	time_info = localtime(&time_now);
	fat_time = fatlib_convert_to_fat_time(time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
	fat_date = fatlib_convert_to_fat_date(time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);

	if(create)
	{
		directory_entry->crt_time[1] = fat_time >> 8;
		directory_entry->crt_time[0] = fat_time >> 0;
		directory_entry->crt_date[1] = fat_date >> 8;
		directory_entry->crt_time[0] = fat_date >> 8;
	}
	if(modify)
	{
		
		directory_entry->wrt_time[1] = fat_time >> 8;
		directory_entry->wrt_time[0] = fat_time >> 0;
		directory_entry->wrt_date[1] = fat_date >> 8;
		directory_entry->wrt_date[0] = fat_date >> 0;
	}
	if(access)
	{
		directory_entry->last_access_time[1] = fat_time >> 8;
		directory_entry->last_access_time[0] = fat_time >> 0;
		directory_entry->last_access_date[1] = fat_date >> 8;
		directory_entry->last_access_date[0] = fat_date >> 0;
	}
	return 1;
}
#endif

//==========================================================================
// fatlib_update_file_length: Find a SFN entry and update it
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fatlib_update_file_length(struct fatfs *fs, uint32_t cluster, char *shortname, uint32_t file_length)
{
	uint8_t item = 0;
	uint16_t record_offset = 0;
	int x = 0;
	struct fat_dir_entry *directory_entry;

	if(!fs->disk_io.write_media)
		return 0;

	while(1)
	{
		if(fatlib_sector_reader(fs, cluster, x++, 0))
		{
			for(item = 0; item < FAT_DIR_ENTRIES_PER_SECTOR; item++)
			{
				record_offset = FAT_DIR_ENTRY_SIZE *item;
				directory_entry = (struct fat_dir_entry *)(fs->current_sector.sector + record_offset);
#if FATLIB_INC_LFN_SUPPORT
				if(fatlib_entry_lfn_text(directory_entry));
				else if(fatlib_entry_lfn_invalid(directory_entry));
				else
#endif
				if(fatlib_entry_sfn_only(directory_entry))
				{
					if(strncmp((const char *)directory_entry->name, shortname, 11) == 0)
					{
						directory_entry->file_size = FAT_HTONL(file_length);
#if FATLIB_INC_TIME_DATE_SUPPORT
						fatlib_update_timestamps(directory_entry, 0, 1, 1);
#endif
						memcpy((uint8_t *)(fs->current_sector.sector + record_offset), (uint8_t *)directory_entry, sizeof(struct fat_dir_entry));
						return fs->disk_io.write_media(fs->current_sector.address, fs->current_sector.sector, 1);
					}
				}
			}
		}
		else
			break;
	}
	return 0;
}
#endif

//==========================================================================
// fatlib_mark_file_deleted: Find a SFN entry and mark if as deleted
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fatlib_mark_file_deleted(struct fatfs *fs, uint32_t cluster, char *shortname)
{
	uint8_t item = 0;
	uint16_t record_offset = 0;
	int x = 0;
	struct fat_dir_entry *directory_entry;

	if(!fs->disk_io.write_media)
		return 0;

	while(1)
	{
		if(fatlib_sector_reader(fs, cluster, x++, 0))
		{
			for(item = 0; item < FAT_DIR_ENTRIES_PER_SECTOR; item++)
			{
				record_offset = FAT_DIR_ENTRY_SIZE * item;
				directory_entry = (struct fat_dir_entry *)(fs->current_sector.sector + record_offset);
#if FATLIB_INC_LFN_SUPPORT
				if(fatlib_entry_lfn_text(directory_entry));
				else if(fatlib_entry_lfn_invalid(directory_entry));
				else
#endif
				if(fatlib_entry_sfn_only(directory_entry))
				{
					if(strncmp((const char *)directory_entry->name, shortname, 11)==0)
					{
						directory_entry->name[0] = FILE_HEADER_DELETED;
#if FATLIB_INC_TIME_DATE_SUPPORT
						fatlib_update_timestamps(directory_entry, 0, 1, 1);
#endif
						memcpy((uint8_t *)(fs->current_sector.sector + record_offset), (uint8_t *)directory_entry, sizeof(struct fat_dir_entry));
						return fs->disk_io.write_media(fs->current_sector.address, fs->current_sector.sector, 1);
					}
				}
			}
		}
		else
			break;
	}
	return 0;
}
#endif

//==========================================================================
// fatlib_list_directory_start: Init a dir listing procedure
//==========================================================================
#if FATLIB_DIR_LIST_SUPPORT
void fatlib_list_directory_start(struct fatfs *fs, struct fs_dir_list_status *dirls, uint32_t start_cluster)
{
	dirls->cluster = start_cluster;
	dirls->sector = 0;
	dirls->offset = 0;
}
#endif

//==========================================================================
// fatlib_list_direcotry_next: Get the next entry in the directory.
// Returns: 1 = found, 0 = end of listing    
//==========================================================================
#if FATLIB_DIR_LIST_SUPPORT
int fatlib_list_directory_next(struct fatfs *fs, struct fs_dir_list_status *dirls, struct fs_dir_ent *entry)
{
	uint8_t i, item;
	uint16_t record_offset;
	struct fat_dir_entry *directory_entry;
	char *long_filename = NULL;
	char short_filename[13];
	struct lfn_cache lfn;
	int dot_required = 0;
	int result = 0;

	fatlib_lfn_cache_init(&lfn, 0);

	while(1)
	{
		if(fatlib_sector_reader(fs, dirls->cluster, dirls->sector, 0))
		{
			for(item = dirls->offset; item < FAT_DIR_ENTRIES_PER_SECTOR; item++)
			{
				record_offset = FAT_DIR_ENTRY_SIZE *item;
				directory_entry = (struct fat_dir_entry *)(fs->current_sector.sector + record_offset);
#if FATLIB_INC_LFN_SUPPORT
				if(fatlib_entry_lfn_text(directory_entry))
					fatlib_lfn_cache_entry(&lfn, fs->current_sector.sector + record_offset);
				else if(fatlib_entry_lfn_invalid(directory_entry))
					fatlib_lfn_cache_init(&lfn, 0);
				else if(fatlib_entry_lfn_exists(&lfn, directory_entry))
				{
					long_filename = fatlib_lfn_cache_get(&lfn);
					strncpy(entry->filename, long_filename, FATLIB_MAX_LONG_FILENAME - 1);

					if(fatlib_entry_is_dir(directory_entry))
						entry->is_dir = 1;
					else
						entry->is_dir = 0;

#if FATLIB_INC_TIME_DATE_SUPPORT
					entry->create_time = ((uint16_t)directory_entry->crt_time[1] << 8) | directory_entry->crt_time[0];
					entry->create_date = ((uint16_t)directory_entry->crt_date[1] << 8) | directory_entry->crt_date[0];
					entry->access_date = ((uint16_t)directory_entry->lst_acc_date[1] << 8) | directory_entry->last_access_date[0];
					entry->write_time = ((uint16_t)directory_entry->wrt_time[1] << 8) | directory_entry->wrt_time[0];
					entry->write_date = ((uint16_t)directory_entry->wrt_date[1] << 8) | directory_entry->wrt_date[0];
#endif
					entry->size = FAT_HTONL(directory_entry->file_size);
					entry->cluster = (FAT_HTONS(directory_entry->first_cluster_high) << 16) | FAT_HTONS(directory_entry->first_cluster_low);

					dirls->offset = item + 1;
					result = 1;
					return 1;
				}
				//Normal entry, only 8.3
				else
#endif
				if(fatlib_entry_sfn_only(directory_entry))
				{
					fatlib_lfn_cache_init(&lfn, 0);
					memset(short_filename, 0, sizeof(short_filename));

					for(i = 0; i < 8; i++)
						short_filename[i] = directory_entry->name[i];

					dot_required = 0;
					for(i = 8; i < 11; i++)
					{
						short_filename[i+1] = directory_entry->name[i];
						if(directory_entry->name[i] != ' ')
							dot_required = 1;
					}

					if(dot_required)
					{
						if(short_filename[0] != '.')
							short_filename[8] = '.';
						else
							short_filename[8] = ' ';
					}
					else
						short_filename[8] = ' ';

					fatlib_get_sfn_display_name(entry->filename, short_filename);

					if(fatlib_entry_is_dir(directory_entry))
						entry->is_dir = 1;
					else
						entry->is_dir = 0;

#if FATLIB_INC_TIME_DATE_SUPPORT

					entry->create_time = ((uint16_t)directory_entry->crt_time[1] << 8) | directory_entry->crt_time[0];
			#define FAT_SECTOR_SIZE                     512		entry->create_date = ((uint16_t)directory_entry->crt_date[1] << 8) | directory_entry->crt_date[0];
					entry->access_date = ((uint16_t)directory_entry->lst_acc_date[1] << 8) | directory_entry->last_access_date[0];
					entry->write_time = ((uint16_t)directory_entry->wrt_time[1] << 8) | directory_entry->wrt_time[0];
					entry->write_date = ((uint16_t)directory_entry->wrt_date[1] << 8) | directory_entry->wrt_date[0];
#endif
					entry->size = FAT_HTONL(directory_entry->file_size);
					entry->cluster = (FAT_HTONS(directory_entry->first_cluster_high) << 16) | FAT_HTONS(directory_entry->first_cluster_low);

					dirls->offset = item + 1;
					result = 1;
					return 1;
				}
			}
			dirls->sector++;
			dirls->offset = 0;
		}
		else
			break;
	}
	return result;
}
#endif
