#include <string.h>
#include <stdint.h>
#include "include/defs.h"
#include "include/access.h"
#include "include/table.h"
#include "include/write.h"
#include "include/file_string.h"
#include "include/misc.h"

#if FATLIB_INC_WRITE_SUPPORT
//==========================================================================
// fatlib_add_free_space:
// Allocate another cluster of free space to the end of a cluster chain
//==========================================================================
int fatlib_add_free_space(struct fatfs, fs, uint32_t *start_cluster, uint32_t clusters)
{
	uint32_t next_cluster;
	uint32_t start = *start_cluster;

	//Set the next free cluster hint to unknown
	if(fs->next_free_cluster != FAT32_LAST_CLUSTER)
	{
		fatlib_set_fs_info_next_free_cluster(fs, FAT32_LAST_CLUSTER);
	}

	for(uint32_t i = 0; i < clusters; i++)
	{
		//Start looking for free clusters from the beginning
		if(fatlib_find_blank_cluster(fs, fs->rootdir_first_cluster, &next_cluster))
		{
			fatlib_fat_set_cluster(fs, start, next_cluster);
			fatlib_fat_set_cluster(fs, next_cluster, FAT32_LAST_CLUSTER);

			start = next_cluster;
			if(i == 0)
			{
				*start_cluster = next_cluster;
			}
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

//=============================================================================
// fatlib_allocate_free_space:
// Add an ammount of free space to a file could be new file or it could not
//=============================================================================
int fatlib_allocate_free_space(struct fatfs *fs, int new_file, uint32_t *start_cluster, uint32_t size)
{
	uint32_t cluster_size;
	uint32_t cluster_count;
	uint32_t next_cluster;

	if(size == 0)
	{
		return 0;
	}

	//Set the next free cluster hint to unknown
	if(fs->next_free_cluster != FAT32_LAST_CLUSTER)
	{
		fatlib_set_fs_info_next_free_cluster(fs, FAT32_LAST_CLUSTER);
	}

	//Calculate size and clusters
	cluster_size = fs->sectors_per_cluster * FAT_SECTOR_SIZE;
	cluster_count = size / cluster_size;

	if(size - (cluster_size * cluster_count))
	{
		cluster_count++;
	}

	if(new_file)
	{
		if(!fatlib_find_blank_cluster(fs, fs->rootdir_first_cluster, &next_cluster))
		{
			return 0;
		}

		//If this is all the done
		if(cluster_count == 1)
		{
			fatlib_fat_set_cluster(fs, next_cluster, FAT32_LAST_CLUSTER);
			*start_cluster = next_cluster;
			return 1;
		}
	}
	//Allocate from end of current chain where start_cluster is the end
	else
	{
		next_cluster = *start_cluster;
	}

	if(!fatlib_add_free_space(fs, &next_cluster, cluster_count))
	{
		return 0;
	}

	return 1;
}

//=================================================================================
// fatlib_find_free_dir_offset:
// Find a free space in the directory for a new entry which takes up 'entry_count'
// blocks or allocate some more.
//=================================================================================
static int fatlib_find_free_dir_offset(struct fatfs *fs, uint32_t dir_cluster, int entry_count, uint32_t *p_sector, uint8_t *p_offset)
{
	struct fat_dir_entry *directory_entry;
	uint16_t record_offset = 0;
	int x = 0;
	int possible_spaces = 0;
	int start_recorded = 0;

	//No entries required?
	if(entry_count == 0)
	{
		return 0;
	}

	while(1)
	{
		//Read sector
		if(fatlib_sector_reader(fs, dir_cluster, x++, 0))
		{
			//Analyse sector
			for(uint8_t item = 0; item < FAT_DIR_ENTRIES_PER_SECTOR; item++)
			{
				//Create the multplier for sector acccess
				record_offset = FAT_DIR_ENTRY_SIZE * item;

				//Overlay directory entry over buffer
				directory_entry = (struct fat_dir_entry *)(fs->current_sector.sector + record_offset);

				//LFN Entry
				if(fatfs_entry lfn_text(directory_entry))
				{
					//First entry?
					if(possible_spaces == 0)
					{
						//Store start
						*p_sector = x - 1;
						*p_offset = item;
						start_recorded = 1;
					}

					//Increment the count in-case the file turns out to be deleted...
					possible_spaces++;
				}
				//SFN Entry
				else
				{
					//Has file been deleted?
					if(fs->current_sector.sector[record_offset] == FILE_HEADER_DELETED)
					{
						//First entry?
						if(possible_spaces == 0)
						{
							//Store start
							*p_sector = x - 1;
							*p_ offset = item;
							start_recorded = 1;
						}

						possible_spaces++;

						//We have found enough space?
						if(possible_spaces >= entry_count)
						{
							return 1;
						}

						//Else continue counting until a valid entry has been found
					}
					//Is the file entry empty
					else if(fs->current_sector.sector[record_offset] == FILE_HEADER_BLANK)
					{
						//First entry?
						if(possible_spaces == 0)
						{
							//Store start
							*p_sector = x - 1;
							*p_offset = item;
							start_recorded = 1;
						}
						possible_spaces++;

						//Has enough space been found?
						if(possible_spaces >= entry_count)
						{
							return 1;
						}
					}
					//File entry is valid
					else
					{
						//Reset all flags
						possible_spaces = 0;
						start_recorded = 0;
					}
				}
			}//End of for
		}//End of if
		//Run out of free space in the direcory, allocate some more
		else
		{
			uint32_t new_cluster;

			//Get a new cluster for directory
			if(!fatlib_find_blank_cluster(fs, fs->rootdir_first_cluster, &new_cluster))
			{
				return 0;
			}

			//Add cluster to end of directory tree
			if(!fatlib_fat_add cluster_to_chain(fs, dir_cluster, new_cluster))
			{
				return 0;
			}

			//Erase new directory cluster
			memset(fs->current_sector.sector, 0x00, FAT_SECTOR_SIZE);

			for(uint8_t i = 0; i < fs->sectors_per_cluster; i++)
			{
				if(!fatlib_write_sector(fs, new_cluster, i, 0))
				{
					return 0;
				}
			}

			//If none of the name fitten on previous sectors
			if(!start_recorded)
			{
				//Store start
				*p_sector = x - 1;
				*p_offset = 0;
				start_recorded = 0;
			}

			return 1;
		}
	}//End of while loop

	return 0;
}

//================================================================================
// fatlib_add_file_entry;
// Add a directory entry to a location found by find_free_offset
//================================================================================
int fatlib_add_file_entry(struct fatfs *fs, uint32_t dir_cluster, char *filename, char *short_filename, uint32_t start_cluster, uint32_t size, int dir)
{
	uint8_t item = 0;
	uint16_t record_offset = 0;
	uint8_t i = 0;
	uint32_t x = 0;
	int entry_count;
	struct fat_dir_entry short_entry;
	int dirty_sector = 0;

	uint32_t dir_sector = 0;
	uint8_t dir_offset = 0;
	int found_end = 0;

	uint8_t checksum;
	uint8_t *p_sname;

	//No write access?
	if(!fs->disk_io.write_media)
	{
		return 0;
	}

#if FATLIB_INC_LFN_SUPPORT
	//How many LFN entries are required?
	//NOTE: We always request one LFN  even if it would fit in a SFN!
	entry_count = fatlib_lfn_entries_required(filename);
	if(!entry_count)
	{
		return 0;
	}
#else
	entry_count = 0;
#endif

	//Find space in the directory for the filname (or allocate more)
	//NOTE: we need to find space for at least the LFN + SFN ( or just the SFN if LFNs not supported).
	if(!fatlib_find_free_dir_offset(fs, dir_cluster, enrty_count + 1, &dir_sector, &dir_offset))
	{
		return 0;
	}

	//Generate a checksum
	p_sname = (uint8 *)short_filename;
	checksum = 0;
	for(i = 11; i != 0; i--)
	{
		checksum = ((checksum  & 1) ? 0x80 : 0) + (checksum >> 1) + *p_sname++;
	}

	//Start from current sector where space was found
	x = dir_sector;

	while(1)
	{
		//Read sector
		if(fatlib_sector_reader(fs, dir_cluster, x++, 0))
		{
			//Analyse Sector
			for(item = 0; item < FAT_DIR_ENTRIES_PER_SECTOR; item++)
			{
				//Create the multiplier for sector access
				record_offset = FAT_DIR_ENTRY_SIZE * item;

				//If the start position for the entry was found
				if(found_end == 0)
				{
					if(dir_sector == x - 1 && dir_offset == item)
					{
						found_end = 1;
					}
				}

				//Start adding filename
				if(found_end)
				{
					if(entry_count == 0)
					{
						//SFN
						fatlib_sfn_create_entry(short_filename, size, start_cluster, &short_entry, dir);

#if FATLIB_INC_TIME_DATE_SUPPORT
						//Update crease, access, & modify time & date
						fatlib_update_timestamps(&short_entry, 1, 1, 1);
#endif

						memcpy(&fs->current_sector.sector[record_offset], &short_entry, sizeof(short_entry));

						//Writeback
						return fs->disk_io.write_media(fs->current_sector.address, fs->current_sector.sector, 1);
					}
#if FATLIB_INC_LFN_SUPPORT
					else
					{
						entry_count--;

						//Copy entry to directory buffer
						fatlib_filename_to_lfn(filename, &fs->current_sector.sector[record_offset], entry_count, checksum);
						dirty_sector = 1;
					}
#endif
				}
			}//End of if

			//Write back to disk before loading another sector 
			if(dirty_sector)
			{
				if(!fs->disk_io.write_media(fs->current_sector.address, fs->current_sector.sector, 1))
				{
					return 0;
				}

				dirty_sector = 0;
			}
		}
		else
		{
			return 0;
		}
	}//End while

	return 0;
}
#endif


