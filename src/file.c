#include <stdint.h>
#include <string.h>
#include "include/fat.h"
#include "include/printf.h"

int fileio_test()
{
	fl_list_directory("/");
	FILE *licence = fopen("/cmdline.txt", "r");
	if(licence == NULL)
	{
		printf("Failed to open file!\n");
		return 0;
	}
	char buf[190];
	char *str = buf;
	fread(str, 189, 1, licence);
	printf("[INFO] Attempting read from file: %s", str);
	return 0;
}
