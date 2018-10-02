/*#include "headers/project.h"

void InvalidateDataCacheRange(uint32_t nAddress, uint32_t nLength)
{
	nLength += DATA_CACHE_LINE_LENGTH_MIN;
	while (1)
	{
		asm volatile ("dc ivac, %0" : : "r" (nAddress) : "memory");
		if (nLength < DATA_CACHE_LINE_LENGTH_MIN)
		{
			break;
		}
		nAddress += DATA_CACHE_LINE_LENGTH_MIN;
		nLength -= DATA_CACHE_LINE_LENGTH_MIN;
	}
}*/
