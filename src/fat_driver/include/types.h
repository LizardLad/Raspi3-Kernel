#ifndef FATLIB_TYPES
#define FATLIB_TYPES

#include <string.h>

//=========================================================================
//System specific types
//=========================================================================
#ifndef NULL
	#define NULL ((void *)0x00)
#endif

//=========================================================================
//Endian Macros
//=========================================================================
//Fat is little endian so big endian systems need a swap of words

//Little Endian no swap required
#if FATLIB_IS_LITTLE_ENDIAN == 1
	#define FAT_HTONS(n) (n)
	#define FAT_HTONL(n) (n)

#else //Big Endian - Swap required!
	#define FAT_HTONS(n) ((((uint16)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8))
	#define FAT_HTONL(n) (((((uint32)(n) & 0xFF)) << 24) | ((((uint32)(n) & 0xFF00)) << 8) | ((((uint32)(n) & 0xFF0000)) >> 8) | ((((uint32)(n) & 0xFF000000)) >> 24))

#endif

//=========================================================================
//Struct Packing Compile Options
//=========================================================================
#ifdef __GNUC__
	#define STRUCT_PACK
	#define STRUCT_PACK_BEGIN
	#define STRUCT_PACK_END
	#define STRUCT_PACKED		__attribute__((packed))
#else
	//Other compilers may require other methods of packing structs
	#define STRUCT_PACK
	#define STRUCT_PACK_BEGIN
	#define STRUCT_PACK_END
	#define STRUCT_PACKED
#endif

#endif
