#include <stdint.h>
#include <stdbool.h>
#include "../include/malloc.h"
#include "../include/printf.h"
#include "../include/multicore.h"
#include "include/load_virt_env.h"
#include "include/env_file_type.h"

char *virt_env;
int32_t SCX, SCY, SCZ, CX, CY, CZ;
bool env_loaded;

int init_virt_env()
{
	set_env_loaded(false);
	get_world_dimentions_from_file(&SCX, &SCY, &SCZ, &CX, &CY, &CZ, "world.bin"); //Open the file and grab the dimentions
	virt_env = malloc(SCX * SCY * SCZ * CX * CY * CZ); //Allocate space for the environment and it is done
	int res;
	if(res = load_world())
	{
		printf("[CORE %d] [ERROR] Load world failed with error code %d\n", get_core_id(), res);
	}
}

inline void env_set(int32_t x, int32_t y, int32_t z, uint8_t val)
{
	virt_env[(x) + (SCX * CX * y) + (z * SCX * SCY * CX * CY)] = val; //Wrapper to make a contiguous piece of memory appear like a 3D array
}

inline uint8_t env_get(int32_t x, int32_t y, int32_t z, uint8_t val)
{
	return virt_env[(x) + (SCX * CX * y) + (z * SCX * SCY * CX * CY)]; //Wrapper to make a contiguous piece of memory appear like a 3D array
}

inline void set_env_loaded(bool val)
{
	env_loaded = val;
}

inline bool get_env_loaded()
{
	return env_loaded;
}

inline bool is_env_loaded() //Returns false if true to make if statements easier
{
	return !env_loaded; //USAGE EXAMPLE: if(is_env_loaded()) { printf("[CORE %d] [ERROR] Virtual environment didn't load properly!\n", get_core_id()); return; }
}

