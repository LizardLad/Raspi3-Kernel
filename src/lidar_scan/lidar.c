#include <stdint.h>
#include <math.h>
#include "include/lidar.h"
#include "include/virt_env.h"

struct vec3_t queue[512];
uint8_t flags;

struct vec3_t vec3_sub(struct vec3_t a, struct vec3_t b)
{
	struct vec3_t out;
	out.x = a.x - b.x;
	out.y = a.y - b.y;
	out.z = a.z - b.z;
	return out;
}

struct vec3_t vec3_mul(struct vec3_t a, struct vec3_t b)
{
	struct vec3_t out;
	out.x = a.x * b.x;
	out.y = a.y * b.y;
	out.z = a.z * b.z;
	return out;
}

struct vec3_t vec3_add(struct vec3_t a, struct vec3_t b)
{
	struct vec3_t out;
	out.x = a.x + b.x;
	out.y = a.y + b.y;
	out.z = a.z + b.z;
	return out;
}

struct vec3_t get_lookat(double z_rot, double y_rot)
{
	struct vec3_t lookat = { 0.0 };
	lookat.y = y_rot;
	lookat.z = z_rot;
	return lookat;
}

double get_distance_to_block(struct vec3_t block, struct vec3_t camera)
{
	struct vec3_t block_cam_dif_vec = vec3_sub(camera, block);

	double xy_dis = sqrt(pow(block_cam_dif_vec.x, 2) + pow(block_cam_dif_vec.y, 2));
	double xyz_dis = sqrt(pow(block_cam_dif_vec.z, 2) + pow(xy_dis, 2));

	//This won't work if the xyz distance is the same as the z displacement
	double dis_in_block = 0.5; //Half the cube width of 1 because if x and y are 0 some stuff fails
	if(block_cam_dif_vec.z != xyz_dis)
	{	
		double theta = asin(block_cam_dif_vec.z / xyz_dis);
		dis_in_block = 0.5 / cos(theta);
	}

	dis_in_block = 0.0; //Just check some stuff
	return xyz_dis - dis_in_block;
}

struct vec3_t get_block(struct vec3_t lookat, struct vec3_t position, int ray_length, uint8_t *blk, struct vec3_t max)
{
	struct vec3_t testpos = position;
	struct vec3_t step = { 0.2, 0.2, 0.2 };
	struct vec3_t radian = { (M_PI / 180) , (M_PI / 180), (M_PI / 180)};

	for(int i = 0; i < ray_length; i++) //The actuall ray length is the step * ray_length
	{
		testpos = vec3_add(testpos, vec3_mul(vec3_mul(lookat, radian), step));
		
		/* If a block isn't air the ray has collided with something */
		/* Air is index 0 */
		if(env_get((int)floor(testpos.x), (int)floor(testpos.y), (int)floor(testpos.z)))
		{
			break;
		}
	}

	testpos.x = (int)testpos.x;
	testpos.y = (int)testpos.y;
	testpos.z = (int)testpos.z;

	return testpos;
}

void update_lidar(struct lidar_info_t *lidar)
{
    if(lidar->lookat.x == 359)
    {
        lidar->lookat.x = 0;
        if(lidar->lookat.y == 359)
        {
            lidar->lookat.y = 0;
            if(lidar->location.x >= SCX * CX)
            {
                lidar->location.x = 0;
                if(lidar->location.y >= SCY * CY)
                {
                    lidar->location.y = 0;
                    if(lidar->location.z >= SCZ * CZ)
                    {
                        lidar->location.z = 0;
                    }
                    else
                    {
                        lidar->location.z + 10;
                    }
                }
                else
                {
                    lidar->location.y + 10;
                }
            }
            else
            {
                lidar->location.x + 10;
            }
        }
        else
        {
            lidar->lookat.y++;
        }
    }
    else
    {
        lidar->lookat.x++;
    }
}

void run_lidar()
{
    struct lidar_info_t lidar = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    while(1)
    {
        update_lidar(&lidar);
        lidar_scan(&lidar);
    }
}