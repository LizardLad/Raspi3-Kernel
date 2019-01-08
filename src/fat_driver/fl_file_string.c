#include <string.h>
#include <stdint.h>
#include "include/file_string.h"

//==========================================================================
// fatlib_total_path_levels: Take a filename and path and count the sub levels
// of folders. e.g. C:/folder/file.zip = 1 level
// Acceptable input formats are:
// 	c:\folder\file.zip
// 	/dev/etc/samba.conf
// Returns -1 = Error, 0 or more = OK
//==========================================================================
int fatlib_total_path_levels(char *path)
{
	int levels = 0;
	char expected_char;

	if(!path) return -1;

	if(*path == '/')
	{
		expected_char = '/';
		path++;
	}
	else if(path[1] == ':' || path[2] == '\\')
	{
		expected_char = '\\';
		path += 3;
	}
	else return -1;
	
	while(*path)
	{
		for(; *path;)
		{
			if(*path == expected_char)
			{
				path++;
				break;
			}
			path++;
		}
		levels++;
	}

	//Sub file itself
	return levels - 1;
}

//==========================================================================
// fatlib_get_substring: Get a substring from path which contains the folder
// (or file) at the specified level.
// e.g. C:\folder\file.zip : Level 0 = C:\folder, Level 1 = file.zip
// Returns -1 = Error, 0 = OK
//=========================================================================
int fatlib_get_substring(char *path, int level_req, char *output, int max_length)
{
	int copy_point = 0;
	int levels = 0;
	char expected_char;

	if(!path || max_length <= 0) return -1;

	if(*path == '/')
	{
		expected_char = '/';
		path++;
	}
	else if(path[1] ==':' || path[2] == '\\')
	{
		expected_char = '\\';
		path += 3;
	}
	else return -1;

	int path_length = strlen(path);

	for(int i = 0; i < path_length; i++)
	{
		//If a '\' is found then increase the level
		if(*path == expected_char) levels++;

		if((levels == level_req) && (*path != expected_char) && (copy_point < (max_length - 1)))
		{
			output[copy_point++] = *path;
		}
		path++;
	}
	output[copy_point] = '\0';

	if(output[0] != '\0')
		return 0;
	else
		return -1;
}

//==========================================================================
// fatlib_split_path: Full path countains the passed in string.
// Returned is the path string and the file name string
// e.g. C:\folder\file.zip -> path = C:\folder filename = file.zip
// e.g. C:\file.zip -> path = [blank] filename = file.zip
//==========================================================================
int fatlib_split_path(char *full_path, char *path, int max_path, char *filename, int max_filename)
{
	int string_index;
	
	int levels = fatlib_total_path_levels(full_path);
	if(levels == -1) return -1;

	if(fatlib_get_substring(full_path, levels, filename, max_filename) != 0) return -1;

	if(levels == 0) path[0] = '\0';
	else
	{
		string_index = strlen(full_path) - strlen(filename);
		if(string_index > max_path) string_index = max_path;

		memcpy(path, full_path, string_index);
		path[string_index - 1] = '\0';
	}
	return 0;
}

//==========================================================================
// filename_strcmp_case_sensitive: cmp two strings with case sensitivity
//==========================================================================
int filename_strcmp_case_sensitive(char *string_1, char *string_2, int n)
{
	int diff;
	char a, b;

	while(n--)
	{
		a = *string_1;
		b = *string_2;

		if((a >= 'A') && (a <= 'Z')) a += 32;
		if((b >= 'A') && (b <= 'Z')) b += 32;

		diff = a - b;
		if(diff) return diff;

		if((*string_1 == 0) || (*string_2 == 0)) break; //FIXME should be &&
		string_1++;
		string_2++;
	}
	return 0;
}

//=========================================================================
// filename_get_extension: Get index to extension in filename
// Returns -1 if not found
//==========================================================================
int filename_get_extention(char *string)
{
	int dot_position = -1;
	char *string_source = string;

	while(*string_source)
	{
		if(*string_source == '.') dot_position = string_source - string;
		string_source++;
	}
	return dot_position;
}

//==========================================================================
// filename_trimlength: Get length of string excluding trailing spaces
// Returns -1 if not found or index otherwise
//==========================================================================
int filename_trimlength(char *str, int strlen)
{
	int length = strlen;
	char *string_source = str + strlen - 1;

	while(strlen != 0)
	{
		if(!(*string_source == ' '))
			length = string_source - str;
		else break;

		string_source--;
		strlen--;
	}
	return length;
}

//==========================================================================
// fatlib_compare_names: Compare two filenames without any copying or modification of original string
// Returns 1 if match, 0 if not
//==========================================================================
int fatlib_compare_names(char *string_a, char *string_b)
{
	char *ext_1 = NULL;
	char *ext_2 = NULL;

	int ext_1_pos, ext_2_pos;
	int file_1_length, file_2_length;

	ext_1_pos = filename_get_extention(string_a);
	ext_2_pos = filename_get_extention(string_b);

	//Note: Trailing spaces can affect things here
	if((ext_1_pos == -1) && (ext_2_pos != -1)) return 0;
	if((ext_2_pos == -1) && (ext_1_pos != -1)) return 0;

	if(ext_1_pos != -1)
	{
		ext_1 = string_a + ext_1_pos + 1;
		ext_2 = string_b + ext_2_pos + 1;
		 if(strlen(ext_1) != strlen(ext_2)) return 0;
		 if(filename_strcmp_case_sensitive(ext_1, ext_2, strlen(ext_1)) != 0) return 0;

		 file_1_length = ext_1_pos;
		 file_2_length = ext_2_pos;
	}
	//No extensions
	else
	{
		file_1_length = strlen(string_a);
		file_2_length = strlen(string_b);
	}
	
	file_1_length = filename_trimlength(string_a, file_1_length);
	file_2_length = filename_trimlength(string_b, file_2_length);
	
	if(file_1_length != file_2_length) return 0;
	if(filename_strcmp_case_sensitive(string_a, string_b, file_1_length) != 0) return 0;
	else
		return 1;
}

//==========================================================================
// fatlib_string_ends_with_slash: Does the string end with a slash (\ or /)
//==========================================================================
int fatlib_string_ends_with_slash(char *path)
{
	if(path)
	{
		while(*path)
		{
			if(!(*(path+1)))
			{
				if(*path == '\\' || *path == '/')
					return 1;
			}
			path++;
		}
	}
	return 0;
}

//==========================================================================
//fatlib_get_sfn_display_name: Get display name for SFN entry
//==========================================================================
int fatlib_get_sfn_display_name(char *out, char *in)
{
	int len = 0;
	while(*in && len <= 11)
	{
		char a = *in++;
		if(a == ' ')
			continue;
		else if((a >= 'A') && (a <= 'Z'))
			a += 32;
		*out++ = a;
		len++;
	}
	*out = '\0';
	return 1;
}

//==========================================================================
// fatlib_get_extention: Get extention of filename passed in 'filename'
// Returned extension is always lower case.
// Returns: 1 if OK, 0 if not. 
//==========================================================================
int fatlib_get_extension(char *filename, char *out, int maxlen)
{
	int len = 0;

	int ext_pos = filename_get_extention(filename);
	if(ext_pos > 0 && out && maxlen)
	{
		filename += ext_pos + 1;
		while(*filename && len < (maxlen - 1))
		{
			char a = *filename++;

			if((a >= 'A') && (a <= 'Z')) a += 32;

			*out++ = a;
			len++;
		}
		*out = '\0';
		return 1;
	}
	return 0;
}

//==========================================================================
// fatlib_create_path_string: append path & filename to create file path string.
// Returns 1 if all good, 0 if error
//==========================================================================
int fatlib_create_path_sring(char *path, char *filename, char *out, int maxlen)
{
	int len = 0;
	char last = 0;
	char seperator = '/';

	if(path && filename && out && maxlen > 0)
	{
		while(*path && len < (maxlen - 2))
		{
			last = *path++;
			if(last=='\\')
				seperator = '\\';
			*out++ = last;
			len++;
		}

		//Add a seperator if trailing one not found
		if(last != '\\' && last != '/')
			*out++ = seperator;

		while(*filename && len < (maxlen - 1))
		{
			*out++ = *filename++;
			len++;
		}

		*out = '\0';
		return 1;
	}
	return 0;
}
