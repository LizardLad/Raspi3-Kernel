#include "include/sd.h"
#include "include/printf.h"
#include "fat_driver/include/filelib.h"

//Returns 1 on error, 0 on success
int fat_init()
{
	sd_init();
	fl_init();

	if(fl_attach_media(sd_readblock, sd_writeblock) != FAT_INIT_OK)
	{
		printf("[ERROR] Media attach failed!\n[CRITTICAL] FAT Systems failed to start!\n");
		return 1;
	}
	return 0;
}
