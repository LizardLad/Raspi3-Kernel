#ifndef FAT_CACHE_H
#define FAT_CACHE_H

#include <stdint.h>
#include "filelib.h"

//=================================================================
// Functions
//=================================================================
int fatlib_cache_init(struct fatfs *fs, FL_FILE *file);
int fatlib_cache_get_next_cluster(struct fatfs *fs, FL_FILE *file, uint32_t cluster_index, uint32_t *p_next_cluster);
int fatlib_cache_set_next_cluster(struct fatfs *fs, FL_FILE *file, uint32_t cluster_index, uint32_t next_cluster);

#endif
