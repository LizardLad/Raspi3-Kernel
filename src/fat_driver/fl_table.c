#include <string.h>
#include <stdint.h>
#include "include/defs.h"
#include "include/access.h"
#include "include/table.h"

#ifndef FAT_BUFFERS
	#define FAT_BUFFERS	1
#endif

#ifndef FAT_BUFFER_SECTORS
	#define FAT_BUFFER_SECTORS	1
#endif

#if FAT_BUFFERS < 1 || FAT_BUFFER_SECTORS < 1
	#error "FAT_BUFFERS & FAT_BUFFER_SECTORS must be at least 1"
#endif

//==========================================================================
// 					FAT Sector Buffer
//==========================================================================
#define FAT32_GET_32BIT_WORD(pbuf, location)	( GET_32BIT_WORD(pbuf->ptr, location) )
#define FAT32_SET_32BIT_WORD(pbuf, location, value)	{ SET_32BIT_WORD(pbuf->ptr, location, value); pbuf->dirty = 1; }
#define FAT16_GET_16BIT_WORD(pbuf, location)	( GET_16BIT_WORD(pbuf->ptr, location) )
#define FAT16_SET_16BIT_WORD(pbuf, location, value)	{ SET_16BIT_WORD(pbuf->ptr, location, value); pbuf->dirty = 1; }

//==========================================================================
// fatlib_fat_init:
//==========================================================================
void fatlib_fat_init(struct fatfs *fs)
{
	fs->fat_buffer_head = NULL;

	for(int i = 0; i < FAT_BUFFERS; i++)
	{
		//Initalise buffers to invalid
		fs->fat_buffers[i].address = FAT32_INVALID_CLUSTER;
		fs->fat_buffers[i].dirty = 0;

		memset(fs->fat_buffers[i].sector, 0x00, sizeof(fs->fat_buffers[i].sector));
		fs->fat_buffers[i].ptr = NULL;

		//Add to head of queue
		fs->fat_buffers[i].next = fs->fat_buffer_head;
		fs->fat_buffer_head = &fs->fat_buffers[i];
	}
}

//==========================================================================
// fatlib_fat_writeback: Writeback 'dirty' FAT sectors to disk
//==========================================================================
static int fatlib_fat_writeback(struct fatfs *fs, struct fat_buffer *pcur)
{
	if(pcur)
	{
		//Writeback sector if changed
		if(pcur->dirty)
		{
			if(fs->disk_io.write_media)
			{
				uint32_t sectors = FAT_BUFFER_SECTORS;
				uint32_t offset = pcur->address - fs->fat_begin_lba;

				//Limit to sectors used for the FAT
				if((offset + FAT_BUFFER_SECTORS) <= fs->fat_sectors)
				{
					sectors = FAT_BUFFER_SECTORS;
				}
				else
				{
					sectors = fs->fat_sectors - offset;
				}
				if(!fs->disk_io.write_media(pcur->address, pcur->sector, sectors))
				{
					return 0;
				}
			}
			pcur->dirty = 0;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

//==========================================================================
// fatlib_fat_read_sector: Read a FAT sector
//==========================================================================
static struct fat_buffer *fatlib_fat_read_sector(struct fatfs *fs, uint32_t sector)
{
	struct fat_buffer *last = NULL;
	struct fat_buffer *pcur = fs->fat_buffer_head;

	//Itterate through sector list
	while(pcur)
	{
		if((sector >= pcur->address) && (sector < (pcur->address + FAT_BUFFER_SECTORS)))
		{
			break;
		}

		//End of list?
		if(pcur->next == NULL)
		{
			//Remove buffer from list
			if(last)
			{
				last->next = NULL;
			}
			//Is this the first and last buffer in the chain?
			else
				fs->fat_buffer_head = NULL;
		}

		last = pcur;
		pcur = pcur->next;
	}

	//Found the sector already in buffer chain
	if(pcur)
	{
		pcur->ptr = (uint8_t *)(pcur->sector + ((sector - pcur->address) * FAT_SECTOR_SIZE));
		return pcur;
	}

	//Else, removed the last item from list
	pcur = last;

	//Add to start of sector buffer list (now newest sector)
	pcur->next = fs->fat_buffer_head;
	fs->fat_buffer_head = pcur;

	//Writeback sector if changed
	if(pcur->dirty)
		if(!fatlib_fat_writeback(fs, pcur))
			return 0;

	//Address is now new sector
	pcur->address = sector;

	//Read next sector
	if(!fs->disk_io.read_media(pcur->address, pcur->sector, FAT_BUFFER_SECTORS))
	{
		//Read failed, invalidate buffer address
		pcur->address = FAT32_INVALID_CLUSTER;
		return NULL;
	}

	pcur->ptr = pcur->sector;
	return pcur;
}

//==========================================================================
// fatlib_fat_purge: Purge 'dirty' FAT sectors to disk
//==========================================================================
int fatlib_fat_purge(struct fatfs *fs)
{
	struct fat_buffer *pcur = fs->fat_buffer_head;

	while(pcur)
	{
		if(pcur->dirty)
		{
			if(!fatlib_fat_writeback(fs, pcur))
			{
				return 0;
			}
		}

		pcur = pcur->next;
	}
	return 1;
}

//==========================================================================
//				General FAT Table Operations
//==========================================================================

//==========================================================================
// fatlib_find_next_cluster: Return cluster number of next cluster in chain
// by reading FAT table and traversing it. Return 0xFFFFFFFF for end of chain
//==========================================================================
uint32_t fatlib_find_next_cluster(struct fatfs *fs, uint32_t current_cluster)
{
	uint32_t fat_sector_offset, position;
	uint32_t next_cluster = 0;
	struct fat_buffer *pbuf;

	//Why is '..' labelled with cluster 0 when it should be 2?
	if(current_cluster == 0)
		current_cluster = 2;

	//Find which sector of FAT table to read
	if(fs->fat_type == FAT_TYPE_16)
		fat_sector_offset = current_cluster / 256;
	else
		fat_sector_offset = current_cluster / 128;

	//Read FAT sector into buffer
	pbuf = fatlib_fat_read_sector(fs, fs->fat_begin_lba + fat_sector_offset);
	if(!pbuf)
		return FAT32_LAST_CLUSTER;

	if(fs->fat_type == FAT_TYPE_16)
	{
		//Find 32bit entry of current sector relating to cluster number
		position = (current_cluster - (fat_sector_offset * 256)) * 2;

		//Read next cluster value from sector buffer
		next_cluster = FAT32_GET_32BIT_WORD(pbuf, (uint16_t)position);

		//Mask out MS 4 bits (only 28bit addressing)
		next_cluster &= 0x0FFFFFFF;

		//If end of chain found
		if(next_cluster >= 0x0FFFFFF8 && next_cluster <= 0x0FFFFFFF)
			return FAT32_LAST_CLUSTER;
	}

	//Else return next cluster
	return next_cluster;
}

//==========================================================================
// fatlib_set_fs_info_next_free_cluster: Write the next free cluster to the FSINFO table
//==========================================================================
void fatlib_set_fs_info_next_free_cluster(struct fatfs *fs, uint32_t new_value)
{
	if(fs->fat_type != FAT_TYPE_16)
	{
		//Load sector to change it
		struct fat_buffer *pbuf = fatlib_fat_read_sector(fs, fs->lba_begin + fs->fs_info_sector);

		if(!pbuf)
			return;

		//Change
		FAT32_SET_32BIT_WORD(pbuf, 492, new_value);
		fs->next_free_cluster = new_value;

		//Write back FSINFO sector to disk
		if(fs->disk_io.write_media)
			fs->disk_io.write_media(pbuf->address, pbuf->sector, 1);

		//Invalidate cache entry
		pbuf->address = FAT32_INVALID_CLUSTER;
		pbuf->dirty = 0;
	}
}

//==========================================================================
// fatlib_find_blank_cluster: Find a free cluster entry by reading the FAT
//==========================================================================
#ifndef FATLIB_INC_WRITE_SUPPORT
int fatlib_find_blank_cluster(struct fatfs *fs, uint32_t start_cluster, uint32_t *free_cluster)
{
	uint32_t fat_sector_offset, position;
	uint32_t next_cluster;
	uint32_t current_cluster = start_cluster;
	struct fat_buffer *pbuf;

	do
	{
		//Find which sector of FAT table to read
		if(fs->fat_type == FAT_TYPE_16)
			fat_sector_offset = current_cluster /  256;
		else
			fat_sector_offset = current_cluster / 128;
		if(fat_sector_offset < fs->fat_sectors)
		{
			//Read FAT sector int buffer
			pbuf = fatlib_fat_read_sector(fs, fs->fat_begin_lba + fat_sector_offset);
			if(!pbuf)
				return 0;

			if(fs->fat_type == FAT_TYPE_16)
			{
				//Find 32bit entry of current sector relating to cluster number
				position = (current_cluster - (fat_sector_offset * 256)) * 2;
				//Read next value from buffer
				next_cluster = FAT16_get_16BIT_WORD(pbuf, (uint16_t)position);
			}
			else
			{
				//Find 32bit entry of current sector relating to cluster number
				position = (current_cluster - (fat_sector_offset * 128)) * 4;
				next_cluster = FAT32_GET_32BIT_WORD(pbuf, (uint16_t)position);
				//Mask agin 28bit addressing
				next_cluster &= 0x0FFFFFFF;
			}
			if(next_cluster != 0)
			{
				current_cluster++;
			}
		}
		else
		{
			//Ran out of FAT sectors
			return 0;
		}
	}
	while(next_cluster != 0x0);

	//Found blank entry
	*free_cluster = current_cluster;
	return 1;
}
#endif

//==========================================================================
// fatlib_fat_set_cluster: Set a cluster link in the chain. NOTE Immediate 
// write (Low performance).
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fatlib_fat_set_cluster(struct fatfs *fs, uint32_t cluster, uint32_t next_cluster)
{
	struct fat_buffer *pbuf;
	uint32_t fat_sector_offset, position;

	//Fins which sector of FAT to read
	if(fs->fat_type == FAT_TYPE_16)
		fat_sector_offset = cluster / 256;
	else
		fat_sector_offset = cluster / 128;

	//Read FAT sector into bufffer
	pbuf = fatlib_fat_read_sector(fs, fs->fat_begin_lba + fat_sector_offset);
	if(!pbuf)
		return 0;

	if(fs->fat_type == FAT_TYPE_16)
	{
		//Find 16bit entry of current sector relating to cluster number
		position = (cluster - (fat_sector_offset * 256)) * 2;

		//Write next clusters value to sector buffer
		FAT16_SET_16BIT_WORD(pbuf, (uint16_t)position, ((uint16_t)next_cluster));
	}
	else
	{
		//Find 32bit entry of current sector relating to cluster number
		position = (cluster - (fat_sector_offset * 128)) * 4;

		//Write next clusters value to sector buffer
		FAT32_SET_32BIT_WORD(pbuf, (uint16_t) position, next_cluster);
	}
	return 1;
}
#endif

//==========================================================================
// fatlib_free_cluster_chain: Follow a chain marking each element as free
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fatlib_free_cluster_chain(struct fatfs *fs, uint32_t start_cluster)
{
	uint32_t last_cluster;
	uint32_t next_cluster = start_cluster;

	//Loop though to end of chain
	while((next_cluster != FAT32_LAST_CLUSTER) && (next_cluster != 0x00000000))
	{
		last_cluster = next_cluster;

		//Find next link
		next_cluster = fatlib_find_next_cluster(fs, next_cluster);

		//Clear last link
		fatlib_fat_set_cluster(fs, last_cluster, 0x00000000);
	}
	return 1;
}
#endif

//==========================================================================
// fatlib_fat_add_cluster_to_chain: Follow a chain marking and then add a new entry
// to the current tail.
//==========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fatlib_fat_add_cluster_to_chain(struct fatfs *fs, uint32_t start_cluster, uint32_t new_entry)
{
	uint32_t last_cluster = FAT32_LAST_CLUSTER;
	uint32_t next_cluster = start_cluster;

	if(start_cluster == FAT32_LAST_CLUSTER)
		return 0;

	//Loop until end of chain
	while(next_cluster != FAT32_LAST_CLUSTER)
	{
		last_cluster = next_cluster;

		//Find next link
		next_cluster = fatlib_find_next_cluster(fs, next_cluster);
		if(!next_cluster)
			return 0;
	}

	//Add lnk in for new cluster
	fatlib_fat_set_cluster(fs, last_cluster, new_entry);

	//Mark new cluster as end of chain
	fatlib_fat_set_cluster(fs, new_entry, FAT32_LAST_CLUSTER);

	return 1;
}
#endif

//==========================================================================
// fatlib_count_free_clusters:
//==========================================================================
uint32_t fatlib_count_free_clusters(struct fatfs *fs)
{
	uint32_t count = 0;
	struct fat_buffer *pbuf;

	for(uint32_t i = 0; i < fs->fat_sectors; i++)
	{
		//Read FAT sector into buffer
		pbuf = fatlib_fat_read_sector(fs, fs->fat_begin_lba + i);
		if(!pbuf)
			break;

		for(uint32_t j = 0; j < FAT_SECTOR_SIZE;)
		{
			if(fs->fat_type == FAT_TYPE_16)
			{
				if(FAT16_GET_16BIT_WORD(pbuf, (uint16_t)j) == 0)
					count++;
				j += 2;
			}
			else
			{
				if(FAT32_GET_32BIT_WORD(pbuf, (uint16_t)j) == 0)
					count++;
				j += 4;
			}
		}
	}
	return count;
}

