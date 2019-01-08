#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>
#include "options.h"
#include "misc.h"

//=========================================================================
// Prototypes
//=========================================================================

void fatlib_fat_init(struct fatfs *fs);
int fatlib_fat_purge(struct fatfs *fs);
uint32_t fatlib_find_next_cluster(struct fatfs *fs, uint32_t current_cluster);
void fatlib_set_fs_info_next_free_cluster(struct fatfs *fs, uint32_t new_value);
int faatlib_find_blank_cluster(struct fatfs *fs, uint32_t start_cluster, uint32_t *free_cluster);
int fatlib_fat_set_cluster(struct fatfs *fs, uint32_t cluster, uint32_t next_cluster);
int fatlib_fat_add_cluster_to_chain(struct fatfs *fs, uint32_t start_cluster, uint32_t new_entry);
int fatlib_free_cluster_chain(struct fatfs *fs, uint32_t start_cluster);
uint32_t fatlib_count_free_clusters(struct fatfs *fs);

#endif

