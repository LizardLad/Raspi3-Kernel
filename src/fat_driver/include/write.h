#ifndef FAT_WRITE_H
#define FAT_WRITE_H

#include <stdint.h>
#include "defs.h"
#include "options.h"

//=============================================================================
// Functions
//=============================================================================
int fatlib_add_file_entry(struct fatfs *fs, uint32_t dir_cluster, char *filename, char *short_filename, uint32_t start_cluster, uint32_t size, int dir);
int fatlib_add_free_space(struct fatfs *fs, uint32_t *start_cluster, uint32_t clusters);
int fatlib_allocate_free_space(struct fatfs *fs, int new_file, uint32_t *start_cluster, uint32_t size);

#endif
