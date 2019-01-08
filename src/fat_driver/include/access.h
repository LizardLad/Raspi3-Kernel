#ifndef ACCESS_H
#define ACCESS_H

#include <stdint.h>
#include "defs.h"
#include "options.h"
#include "../../include/mutex.h"

//==========================================================================
// Defines
//==========================================================================
#define FAT_INIT_OK			0
#define FAT_INIT_MEDIA_ACCESS_ERROR	(-1)
#define FAT_INIT_INVALID_SECTOR_SIZE	(-2)
#define FAT_INIT_INVALID_SIGNATURE	(-3)
#define FAT_INIT_ENDIAN_ERROR		(-4)
#define FAT_INIT_WRONG_FILESYS_TYPE	(-5)
#define FAT_INIT_WRONG_PARTITION_TYPE	(-6)
#define FAT_INIT_STRUCT_PACKING		(-7)

#define FAT_DIR_ENTRIES_PER_SECTOR	(FAT_SECTOR_SIZE / FAT_DIR_ENTRY_SIZE)

//==========================================================================
// Funcion pointers
//==========================================================================
typedef int (*fn_diskio_read)(uint32_t sector, uint8_t *buffer, uint32_t sector_count);
typedef int (*fn_diskio_write)(uint32_t sector, uint8_t *buffer, uint32_t sector_count);

//==========================================================================
// Structures
//==========================================================================
struct disk_if
{
	//User supplied function pointers for disk I/O
	fn_diskio_read	read_media;
	fn_diskio_write	write_media;
};

//Forward declaration
struct fat_buffer;

struct fat_buffer
{
	uint8_t sector[FAT_SECTOR_SIZE * FAT_BUFFER_SECTORS];
	uint32_t address;
	int dirty;
	uint8_t *ptr;

	//Next in chain
	struct fat_buffer *next;
};

typedef enum e_fat_type
{
	FAT_TYPE_16,
	FAT_TYPE_32
} t_fat_type;

struct fatfs
{
	//Filesystem globals
	uint8_t sectors_per_cluster;
	uint32_t cluster_begin_lba;
	uint32_t rootdir_first_cluster;
	uint32_t rootdir_first_sector;
	uint32_t rootdir_sectors;
	uint32_t fat_begin_lba;
	uint16_t fs_info_sector;
	uint32_t lba_begin;
	uint32_t fat_sectors;
	uint32_t next_free_cluster;
	uint16_t root_entry_count;
	uint16_t reserved_sectors;
	uint8_t num_of_fats;
	t_fat_type fat_type;

	struct disk_if	disk_io;

	// [OPTIONAL] For thread saftey
	void (*fl_lock)(mutex_r_t *);
	void (*fl_unlock)(mutex_r_t *);

	mutex_r_t lock;

	struct fat_buffer current_sector;

	struct fat_buffer *fat_buffer_head;
	struct fat_buffer fat_buffers[FAT_BUFFERS];
};

struct fs_dir_list_status
{
	uint32_t sector;
	uint32_t cluster;
	uint8_t offset;
};

struct fs_dir_ent
{
	char filename[FATLIB_MAX_LONG_FILENAME];
	uint8_t is_dir;
	uint32_t cluster;
	uint32_t size;

#if FATLIB_INC_TIME_DATE_SUPPORT
	uint16_t access_date;
	uint16_t write_time;
	uint16_t create_date;
	uint16_t create_time;
#endif
};

//==========================================================================
// Functions
//==========================================================================
int fatlib_init(struct fatfs *fs);
uint32_t fatlib_lba_of_cluster(struct fatfs *fs, uint32_t cluster_number);
int fatlib_sector_reader(struct fatfs *fs, uint32_t start_cluster, uint32_t offset, uint8_t *target);
int fatlib_sector_read(struct fatfs *fs, uint32_t lba, uint8_t *target, uint32_t count);
int fatlib_sector_write(struct fatfs *fs, uint32_t lba, uint8_t *target, uint32_t count);
int fatlib_read_sector(struct fatfs *fs, uint32_t cluster, uint32_t sector, uint8_t *target);
int fatlib_write_sector(struct fatfs *fs, uint32_t cluster, uint32_t sector, uint8_t *target);
void fatlib_show_details(struct fatfs *fs);
uint32_t fatlib_get_root_cluster(struct fatfs *fs);
uint32_t fatlib_get_file_entry(struct fatfs *fs, uint32_t cluster, char *name_to_find, struct fat_dir_entry *short_file_entry);
int fatlib_sfn_exists(struct fatfs *fs, uint32_t cluster, char *shortname);
int fatlib_update_file_length(struct fatfs *fs, uint32_t cluster, char *shortname, uint32_t file_length);
int fatlib_mark_file_deleted(struct fatfs *fs, uint32_t cluster, char *shortname);
void fatlib_list_directory_start(struct fatfs *fs, struct fs_dir_list_status *dirls, uint32_t start_cluster);
int fatlib_list_directory_next(struct fatfs *fs, struct fs_dir_list_status *dirls, struct fs_dir_ent *entry);
int fatlib_update_timestamps(struct fat_dir_entry *directory_entry, int create, int modify, int access);

#endif
