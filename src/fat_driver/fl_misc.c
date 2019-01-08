#include <stdlib.h>
#include <stdint.h>
#include "include/misc.h"

//=========================================================================
// fatlib_lfn_cache_init: Clear long filename cache
//=========================================================================
void fatlib_lfn_cache_init(struct lfn_cache *lfn, int wipe_table)
{
	lfn->no_of_strings = 0;
#if FATLIB_INC_LFN_SUPPORT
	//Zero buffer also
	if(wipe_table)
	{
		for(int i = 0; i < MAX_LONG_FILENAME_ENTRIES; i++)
		{
			memset(lfn->string[i], 0x00, MAX_LFN_ENTRY_LENGTH);
		}
	}
#endif
}

//=========================================================================
// fatlib_lfn_cache_entry:
// Function extracts lfn test from sector at offset
//=========================================================================
#if FATLIB_INC_LFN_SUPPORT
void fatlib_lfn_cache_entry(struct lfn_cache *lfn, uint8_t *entry_buffer)
{
	uint8_t LFN_index = entry_buffer[0] & 0x1F;

	//Limit filename t cache size
	if(LFN_index > MAX_LONG_FILENAME_ENTRIES)
	{
		return;
	}

	//This is an error
	if(LFN_index == 0)
	{
		return;
	}

	if(lfn->no_of_strings == 0)
	{
		lfn->no_of_strings = LFN_index;
	}


	lfn->string[LFN_index-1][0] = entry_buffer[1];
	lfn->string[LFN_index-1][1] = entry_buffer[3];
	lfn->string[LFN_index-1][2] = entry_buffer[5];
	lfn->string[LFN_index-1][3] = entry_buffer[7];
	lfn->string[LFN_index-1][4] = entry_buffer[9];
	lfn->string[LFN_index-1][5] = entry_buffer[0x0E];
	lfn->string[LFN_index-1][6] = entry_buffer[0x10];
	lfn->string[LFN_index-1][7] = entry_buffer[0x12];
	lfn->string[LFN_index-1][8] = entry_buffer[0x14];
	lfn->string[LFN_index-1][9] = entry_buffer[0x16];
	lfn->string[LFN_index-1][10] = entry_buffer[0x18];
	lfn->string[LFN_index-1][11] = entry_buffer[0x1C];
	lfn->string[LFN_index-1][12] = entry_buffer[0x1E];

	for(uint8_t i = 0; i < MAX_LFN_ENTRY_LENGTH; i++)
	{
		if(lfn->string[LFN_index-1][i] == 0xFF)
		{
			lfn->string[LFN_index-1][i] = 0x20; //Replace with spaces
		}
	}
}
#endif

//=========================================================================
// fatlib_lfn_cache_get: Get a refrence to the lfn
//=========================================================================
#if FATLIB_INC_LFN_SUPPORT
char *fatlib_lfn_cache_get(struct lfn_cache *lfn)
{
	//Null terminate LFN
	if(lfn->no_of_strings == MAX_LONG_FILENAME_ENTRIES)
	{
		lfn->null = '\0';
	}
	else if(lfn->no_of_strings)
	{
		lfn->string[lfn->no_of_strings][0] = '\0';
	}
	else
	{
		lfn->string[0][0] = '\0';
	}

	return (char *)&lfn->string[0][0];
}
#endif

//=========================================================================
// fatlib_entry_lfn_text: If LFN text entry found
//=========================================================================
#if FATLIB_INC_LFN_SUPPORT
int fatlib_entry_lfn_text(struct fat_dir_entry *entry)
{
	if((entry->attr & FILE_ATTR_LFN_TEXT) == FILE_ATTR_LFN_TEXT)
	{
		return 1;
	}
	else
		return 0;
}
#endif

//=========================================================================
// fatlib_entry_lfn_invalid: IF SFN found not relating to LFN
//=========================================================================
#if FATLIB_INC_LFN_SUPPORT
int fatlib_entry_lfn_invalid(struct fat_dir_entry * entry)
{
	if(entry->name[0] == FILE_HEADER_BLANK || entry->name[0] == FILE_HEADER_DELETED || entry->attr == FILE_ATTR_VOLUME_ID || entry->attr & FILE_ATTR_SYSHID)
	{
		return 1;
	}
	else
		return 0;
}
#endif

//=========================================================================
// fatlib_entry_lfn_exists: If LFN exits and the correlating SFN is found
//=========================================================================
#if FATLIB_INC_LFN_SUPPORT
int fatlib_entry_lfn_exists(struct lfn_cache *lfn, struct fat_dir_entry *entry)
{
	if(entry->attr != FILE_ATTR_LFN_TEXT && entry->name[0] != FILE_HEADER_BLANK && entry->name[0] != FILE_HEADER_DELETED && entry->attr != FILE_ATTR_VOLUME_ID && !(entry->attr & FILE_ATTR_SYSHID) && lfn->no_of_strings)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
#endif

//=========================================================================
// fatlib_entry_sfn_only: If only SFN exists no LFN 
//=========================================================================
int fatlib_entry_sfn_only(struct fat_dir_entry *entry)
{
	if(entry->attr!=FILE_ATTR_LFN_TEXT && entry->name[0]!=FILE_HEADER_BLANK && entry->name[0]!=FILE_HEADER_DELETED && entry->attr!=FILE_ATTR_VOLUME_ID && !(entry->attr&FILE_ATTR_SYSHID))
		return 1;
	else
		return 0;
}

//=========================================================================
// fatlib_entyr_is_dir: Returns 1 if directory
//=========================================================================
int fatlib_entry_is_dir(struct fat_dir_entry *entry)
{
	if(entry->attr & FILE_TYPE_DIR)
	{
		return 1;
	}
	else
		return 0;
}

//=========================================================================
// fatlib_entry_is_file: Returns 1 if it is a file entry
//=========================================================================
int fatlib_entry_is_file(struct fat_dir_entry *entry)
{
	if(entry->attr & FILE_TYPE_FILE)
	{
		return 1;
	}
	else
		return 0;
}

//=========================================================================
// fatlib_lfn_entries_required: Calculate number of 13 char entries
//=========================================================================
#if FATLIB_INC_LFN_SUPPORT
int fatlib_lfn_entries_required(char *filename)
{
	int length = (int)strlen(filename);

	if(length)
		return (length + MAX_LFN_ENTRY_LENGTH - 1) / MAX_LFN_ENTRY_LENGTH;
	else
		return 0;
}
#endif

//=========================================================================
// fatlib_filename_to_lfn:
//=========================================================================
#if FATLIB_INC_LFN_SUPPORT
void fatlib_filename_to_lfn(char *filename, uint8_t *buffer, int entry, uint8_t sfn_checksum)
{
	int name_indexes[MAX_LFN_ENTRY_LENGTH] = {1, 3, 5, 7, 9, 0x0E, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1C, 0x1E};

	int length = (int)strlen(filename);
	int entries_required = fatlib_lfn_entries_required(filename);

	//Filename offset
	int start = entry * MAX_LFN_ENTRY_LENGTH;

	//Init to zero
	memset(buffer, 0x00, FAT_DIR_ENTRY_SIZE);

	//LFN entry number
	buffer[0] = (uint8_t)(((entries_required - 1) == entry) ? (0x40 | (entry + 1)) : (entry + 1));

	//LFN flag
	buffer[11] = 0x0F;

	//Checksum of SFN
	buffer[13] = sfn_checksum;

	//Copy to buffer
	for(int i = 0; i < MAX_LFN_ENTRY_LENGTH; i++)
	{
		if((start + i) < length)
		{
			buffer[name_indexes[i]] = filename[start+i];
		}
		else if((start + i) == length)
		{
			buffer[name_indexes[i]] = 0x00;
		}
		else
		{
			buffer[name_indexes[i]] = 0xFF;
			buffer[name_indexes[i]+1] = 0xFF;
		}
	}
}
#endif

//=========================================================================
// fatlib_sfn_create_entry: Create the sfn directory entry
//=========================================================================
#if FATLIB_INC_WRITE_SUPPORT
void fatlib_sfn_create_entry(char *shortname, uint32_t size, uint32_t start_cluster, struct fat_dir_entry *entry, int dir)
{
	//Copy sfn
	for(int i = 0; i < FAT_SFN_SIZE_FULL; i++)
	{
		entry->name[i] = shortname[i];
	}

	//Unless RTC set to 1980
	entry->crt_time_tenth = 0x00;
	entry->crt_time[0] = entry->crt_time[1] = 0x00;
	entry->crt_date[1] = 0x00;
	entry->crt_date[0] = 0x20;
	entry->last_access_date[1] = 0x00;
	entry->last_access_date[0] = 0x20;
	entry->write_time[0] = entry->write_time[1] = 0x00;
	entry->write_date[1] = 0x00;
	entry->write_date[0] = 0x20;

	if(!dir)
	{
		entry->attr = FILE_TYPE_FILE;
	}
	else
	{
		entry->attr = FILE_TYPE_DIR;
	}

	entry->NTRes = 0x00;

	entry->first_cluster_high = FAT_HTONS((uint16_t)((start_cluster >> 16) & 0xFFFF));
	entry->first_cluster_low = FAT_HTONS((uint16_t)((start_cluster >> 0) & 0xFFFF));
		entry->file_size = FAT_HTONL(size);
}
#endif

//=========================================================================
// fatlib_lfn_create_sfn: Create a padded SFN
//=========================================================================
#if FATLIB_INC_WRITE_SUPPORT
int fatlib_lfn_create_sfn(char *sfn_output, char *filename)
{
	int dot_pos = -1;
	char ext[3];
	int pos = 0;
	int length = (int)strlen(filename);

	//Invalid to start with .
	if(filename[0] == '.')
	{
		return 0;
	}

	memset(sfn_output, ' ', FAT_SFN_SIZE_FULL);
	memset(ext, ' ', 3);

	int i = 0;
	for(; i < length; i++)
	{
		if(filename[i] == '.')
		{
			dot_pos = i;
		}
	}

	//Extract extentions
	if(dot_pos != -1)
	{
		for(i = dot_pos + 1; i < dot_pos + 4; i++)
		{
			if(i < length)
			{
				ext[i-(dot_pos+1)] = filename[i];
			}
		}
		//Shorten the len to the dot pos
		length = dot_pos;
	}

	//Add filename part
	for(i = 0; i < length; i++)
	{
		if(filename[i] != ' ' && filename[i] != '.')
		{
			if(filename[i] >= 'a' && filename[i] <= 'z')
			{
				sfn_output[pos++] = filename[i] - 'a' + 'A';
			}
			else
			{
				sfn_output[pos++] = filename[i];
			}
		}

		//Fill upto 8 chars
		if(pos == FAT_SFN_SIZE_PARTIAL)
		{
			break;
		}
	}

	//Add extention part
	for(i = FAT_SFN_SIZE_PARTIAL; i < FAT_SFN_SIZE_FULL; i++)
	{
		if(ext[i-FAT_SFN_SIZE_PARTIAL] >= 'a' && ext[i-FAT_SFN_SIZE_PARTIAL] <= 'z')
		{
			sfn_output[i] = ext[i - FAT_SFN_SIZE_PARTIAL] - 'a' + 'A';
		}
		else
		{
			sfn_output[i] = ext[i - FAT_SFN_SIZE_PARTIAL];
		}
	}

	return 1;
}

//=========================================================================
// fatlib_itoa:
//=========================================================================
static void fatlib_itoa(uint32_t num, char *s)
{
	char* cp;
	char outbuf[12];
	const char digits[] = "0123456789ABCDEF";

	// Build string backwards
	cp = outbuf;
	do
	{
		*cp++ = digits[(int)(num % 10)];
	}
	while ((num /= 10) > 0);

	*cp-- = 0;

	// Copy in forwards
	while (cp >= outbuf)
		*s++ = *cp--;

	*s = 0;
}
#endif

//=========================================================================
// fatlib_lfn_generate_tail:
// sfn_input: Input short filename, spaced & in uppercase
// sfn_output: Output short filename with tail
//=========================================================================
#if FATLIB_INC_LFN_SUPPORT
#if FATLIB_INC_WRITE_SUPPORT
int fatlib_lfn_generate_tail(char *sfn_output, char *sfn_input, uint32_t tail_num)
{
	int tail_len;
	char tail_str[12];

	if(tail_num > 99999)
	{
		return 0;
	}

	//Convert to number
	memset(tail_str, 0x00, sizeof(tail_str));
	tail_str[0] = '~';
	fatlib_itoa(tail_num, tail_str+1);

	//Copy in the filename
	memcpy(sfn_output, sfn_input, FAT_SFN_SIZE_FULL);

	//Overwrite with tail
	tail_len = (int)strlen(tail_str);
	memcpy(sfn_output + (FAT_SFN_SIZE_PARTIAL - tail_len), tail_str, tail_len);

	return 1;
}
#endif
#endif

//=========================================================================
// fatlib_convert_from_fat_time: Convert FAT time to h/m/s
//=========================================================================
#if FATLIB_INC_TIME_DATE_SUPPORT
void fatlib_convert_from_fat_time(uint16_t fat_time, int *hours, int *minutes, int *seconds)
{
	*hours = (fat_time >> FAT_TIME_HOURS_SHIFT) & FAT_TIME_HOURS_MASK;
	*minutes = (fat_time >> FAT_TIME_MINUTES_SHIFT) & FAT_TIME_MINUTES_MASK;
	*seconds = (fat_time >> FAT_TIME_SECONDS_SHIFT) & FAT_TIME_SECONDS_MASK * FAT_TIME_SECONDS_SCALE;
}

//=========================================================================
// fatlib_convert_from_fat_date: Convert FAT date to d/m/y
//=========================================================================
void fatlib_convert_from_fat_date(uint16_t fat_date, int *day, int *month, int *year)
{
	*day = (fat_date >> FAT_DATE_DAY_SHIFT) & FAT_DATE_DAY_MASK;
	*month = (fat_date >> FAT_DATE_MONTH_SHIFT) & FAT_DATE_MONTH_MASK;
	*year = (fat_date >> FAT_DATE_YEAR_SHIFT) & FAT_DATE_YEAR_MASK + FAT_DATE_YEAR_OFFSET;
}

//=========================================================================
// fatlib_convert_fat_time: Convert h/m/s to FAT time
//=========================================================================
uint16_t fatlib_convert_to_fat_time(int hours, int minutes, int seconds)
{
	uint16_t fat_time = 0;

	//Most FAT times have a res of 2 secs
	seconds /= FAT_TIME_SECONDS_SCALE;

	fat_time = (hours & FAT_DATE_HOURS_MASK) << FAT_DATE_HOURS_SHIFT;
	fat_time |= (minutes & FAT_DATE_MINUTES_MASK) << FAT_DATE_MINUTES_SHIFT;
	fat_time |= (seconds & FAT_DATE_SECONDS_MASK) << FAT_DATE_SECONDS_SHIFT;

	return fat_time;
}

//=========================================================================
// fatlib_convert_fat_time: Convert h/m/s to FAT time
//=========================================================================
uint16_t fatlib_convert_to_fat_date(int day, int month, int year)
{
	uint16_t fat_date = 0;

	//FAT dates are relative to 1980
	if(year >= FAT_DATE_YEAR_OFFSET)
	{
		year -=FAT_DATE_YEAR_OFFSET;
	}

	fat_date = (day & FAT_DATE_DAY_MASK) << FAT_DATE_DAY_SHIFT;
	fat_date |= (month & FAT_DATE_MONTH_MASK) << FAT_DATE_MONTH_SHIFT;
	fat_date |= (year & FAT_DATE_YEAR_MASK) << FAT_DATE_YEAR_SHIFT;

	return fat_date;
}
#endif

//=========================================================================
// fatlib_print_sector:
//=========================================================================
#ifdef FATFS_DEBUG
void fatlib_print_sector(uint32_t sector, uint8_t *data)
{
	FAT_PRINTF(("Sector %d:\n", sector));

	for(int i = 0; i < FAT_SECTOR_SIZE; i++)
	{
		if(!((i) % 16))
		{
			FAT_PRINTF(("  %04d: ", i));
		}

		FAT_PRINTF(("%02x", data[i]));
		if(!((i + 1) % 4))
		{
			FAT_PRINTF((" "));
		}

		if(!((i + 1) % 16))
		{
			FAT_PRINTF(("   "));
			for(int j = 0; j < 16; j++)
			{
				char ch = data[i - 15 + j];

				//Is printable?
				if(ch > 31 && ch < 127)
				{
					FAT_PRINTF(("%c", ch));
				}
				else
				{
					FAT_PRINTF(("."));
				}
			}
			FAT_PRINTF(("\n"));
		}
	}
}
#endif
