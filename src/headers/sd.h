#ifndef SD_H
#define SD_H

#define SD_OK                0
#define SD_TIMEOUT          -1
#define SD_ERROR            -2

int32_t sd_init();
int32_t sd_readblock(uint32_t lba, uint8_t *buffer, uint32_t num);

#endif
