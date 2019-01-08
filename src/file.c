#include <stdint.h>
#include "include/fat.h"
#include "include/printf.h"

int fileio_test()
{
	FILE *licence = fopen("licence.broadcom", "r");
	char str[100];
	fread(str, 11, 1, licence);
	printf("%s\n", str);
	return 0;
}
