#include <string.h>
#include <stdint.h>
#include "include/cache.h"

//Per file cluster improves performance
//Requires some memory space

//============================================================================
// fat_cache_init:
//============================================================================
int fatlib_cache_init(struct fatfs *fs, FL_FILE *file)
{
#ifndef FAT_CLUSTER_CACHE_ENTRIES
	for(int i = 0; i < FAT_CLUSTER_CACHE_ENTRIES; i++)
	{
		file->cluster_cache_index[i] = 0xFFFFFFFF;
		file->cluster_cache_data[i] = 0;
	}
#endif
	return 1;
}

//============================================================================
// fatlib_cache_get_next_cluster:
//============================================================================
int fatlib_cache_get_next_cluster(struct fatfs *fs, FL_FILE *file, uint32_t cluster_index, uint32_t *p_next_cluster)
{
#ifndef FAT_CLUSTER_CACHE_ENTRIES
	uint32_t slot = cluster_index % FAT_CLUSTER_CACHE_ENTRIES;

	if(file->cluster_cache_index[slot] == cluster_index)
	{
		*p_next_cluster = file->cluster_cache_data[slot];
		return 1;
	}
#endif
	return 0;
}

//============================================================================
// fatlib_cache_set_next_cluster:
//============================================================================
int fatlib_cache_set_next_cluster(struct fatfs *fs, FL_FILE *file, uint32_t cluster_index, uint32_t next_cluster)
{
#ifndef FAT_CLUSTER_CACHE_ENTRIES
	uint32_t slot = cluster_index % FAT_CLUSTER_CACHE_ENTRIES;

	if(file->cluster_cache_index[slot] == cluster_index)
	{
		file->cluster_cache_data[slot] = next_cluster;
	}
	else
	{
		file->cluster_cache_index[slot] = cluster_index;
		file->cluster_cache_data[slot] = next_cluster;
	}
#endif
	return 1;
}
