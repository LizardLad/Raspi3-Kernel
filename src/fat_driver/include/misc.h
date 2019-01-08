#ifndef MISC_H
#define MISC_H

#include <stdint.h>

#include "defs.h"
#include "options.h"

//=========================================================================
//Defines
//=========================================================================
#define MAX_LONG_FILENAME_ENTRIES	20
#define MAX_LFN_ENTRY_LENGTH		13

//=========================================================================
//Macros
//=========================================================================
#define GET_32BIT_WORD(buffer, location)    ( ((uint32_t)buffer[location+3]<<24) + ((uint32_t)buffer[location+2]<<16) + ((uint32_t)buffer[location+1]<<8) + (uint32_t)buffer[location+0] )
#define GET_16BIT_WORD(buffer, location)    ( ((uint16_t)buffer[location+1]<<8) + (uint16_t)buffer[location+0] )

#define SET_32BIT_WORD(buffer, location, value)    { buffer[location+0] = (uint8_t)((value)&0xFF); \
                                                  buffer[location+1] = (uint8_t)((value>>8)&0xFF); \
                                                  buffer[location+2] = (uint8_t)((value>>16)&0xFF); \
                                                  buffer[location+3] = (uint8_t)((value>>24)&0xFF); }

#define SET_16BIT_WORD(buffer, location, value)    { buffer[location+0] = (uint8_t)((value)&0xFF); \
                                                  buffer[location+1] = (uint8_t)((value>>8)&0xFF); }

//=========================================================================
//Structures
//=========================================================================
struct lfn_cache
{
#if FATLIB_INC_LFN_SUPPORT
	//Long file name structure (max 260 LFN length)
	uint8_t string[MAX_LONG_FILENAME_ENTRIES][MAX_LFN_ENTRY_LENGTH];
	uint8_t null;
#endif
	uint8_t no_of_strings;
};

//=========================================================================
//Functions
//=========================================================================
//Functions returning void
void fatlib_lfn_cache_init(struct lfn_cache *lfn, int wipe_table);
void fatlib_lfn_cache_entry(struct lfn_cache *lfn, uint8_t *entry_buffer);
void fatlib_filename_to_lfn(char *filename, uint8_t *buffer, int entry, uint8_t sfn_checksum);
void fatlib_sfn_create_entry(char *short_filename, uint32_t size, uint32_t start_cluster, struct fat_dir_entry *entry, int dir);
void fatlib_convert_from_fat_time(uint16_t fat_time, int *hours, int *minutes, int *seconds);
void fatlib_convert_from_fat_date(uint16_t fat_date, int *day, int *month, int *year);
void fatlib_print_sector(uint32_t sector, uint8_t *data);

//Functions returning int
int fatlib_entry_lfn_text(struct fat_dir_entry *entry);
int fatlib_entry_lfn_invalid(struct fat_dir_entry *entry);
int fatlib_entry_lfn_exists(struct lfn_cache *lfn, struct fat_dir_entry *entry);
int fatlib_entry_sfn_only(struct fat_dir_entry *entry);
int fatlib_entry_is_dir(struct fat_dir_entry *entry);
int fatlib_entry_is_file(struct fat_dir_entry *entry);
int fatlib_lfn_entries_required(char *filename);
int fatlib_lfn_create_sfn(char *sfn_output, char *filename);
int fatlib_lfn_generate_tail(char *sfn_output, char *sfn_input, uint32_t tail_num);

//Functions returning uint16_t
uint16_t fatlib_convert_to_fat_time(int hours, int minutes, int seconds);
uint16_t fatlib_convert_to_fat_date(int day, int months, int years);

//Functions returning char *
char * fatlib_lfn_cache_get(struct lfn_cache *lfn);

#endif
