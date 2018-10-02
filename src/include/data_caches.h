#ifndef DATA_CACHE_H
#define DATA_CACHE_H

#define DATA_CACHE_LINE_LENGTH_MIN      64      // min(L1_DATA_CACHE_LINE_LENGTH, L2_CACHE_LINE_LENGTH)

void InvalidateDataCacheRange(uint32_t nAddress, uint32_t nLength);

#endif
