#ifndef SD_H
#define SD_H

#define SD_OK                0
#define SD_TIMEOUT          -1
#define SD_ERROR            -2

#include <stdint.h>

int sd_init();
int sd_readblock(uint32_t lba, uint8_t *buffer, uint32_t num);
int sd_writeblock(uint32_t lba, uint8_t *buffer, uint32_t num);

#endif
