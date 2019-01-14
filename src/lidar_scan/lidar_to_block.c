#include <stdint.h>
#include <math.h>
#include "include/lidar.h"
#include "include/virt_env.h"
#include "include/lidar.h"

struct vec3_int_t lidar_round(struct vec3_t val)
{
    if(val.x - (int)val.x < 0.5)
        val.x = (int)val.x;
    else
        val.x = 1 + (int)val.x;
    if(val.y - (int)val.y < 0.5)
        val.y = (int)val.y;
    else
        val.y = 1 + (int)val.y;
    if(val.z - (int)val.z < 0.5)
        val.z = (int)val.z;
    else
        val.z = 1 + (int)val.z;
}

void set_output_block(struct vec3_int_t block_to_set)
{
    out_env_set(block_to_set.x, block_to_set.y, block_to_set.z, 1);
}

void lidar_to_block()
{
    struct vec3_int_t temp;
    while(1)
    {
        if(flags & (LIDAR_SET << LIDAR_QUEUE_FULL_SHIFT))
        {
            for(int i = 0; i < 513; i++)
            {
                temp = lidar_round(queue[i]);
                //Set a new virt environment block based on ping
                set_output_block(temp);
            }
            flags &= !(LIDAR_SET << LIDAR_QUEUE_FULL_SHIFT);
        }
    }
}