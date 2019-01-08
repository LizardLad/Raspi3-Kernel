#ifndef FILESTRING_H
#define FILESTRING_H

//==========================================================================
// Functions
//==========================================================================
int fatlib_total_path_levels(char *path);
int fatlib_get_substring(char *path, int level_required, char *output, int max_length);
int fatlib_split_path(char *full_path, char *path, int max_path, char *filename, int max_filename);
int fatlib_compare_names(char *string_a, char *string_b);
int fatlib_string_ends_with_slash(char *path);
int fatlib_get_sfn_display_name(char *out, char *in);
int fatlib_get_extension(char *filename, char *out, int max_length);
int fatlib_create_path_string(char *path, char *filename, char *out, int max_length);

#ifndef NULL
	#define NULL (void *)0x0
#endif
#endif
