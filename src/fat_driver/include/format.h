#ifndef FORMAT_H
#define FORMAT_H

#include <stdint.h>
#include "defs.h"
#include "options.h"
#include "access.h"

//==========================================================================
// Prototypes
//==========================================================================
int fatlib_format(struct fatfs *fs, uint32_t volume_sectors, const char *name);
int fatlib_format_fat16(struct fatfs *fs, uint32_t volume_sectors, const char *name);
int fatlib_format_fat32(struct fatfs *fs, uint32_t volume_sectors, const char *name);

#endif
