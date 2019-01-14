#ifndef LIDAR_H
#define LIDAR_H

#define LIDAR_CLEAR 0
#define LIDAR_SET 1
#define LIDAR_QUEUE_FULL_SHIFT 0
#define LIDAR_QUEUE_EMPTY_SHIFT 1

struct vec3_t
{
	double x;
	double y;
	double z;
};

struct vec3_int_t
{
	int x;
	int y;
	int z;
};

struct lidar_info_t
{
    struct vec3_t location;
    struct vec3_t lookat;
};

extern struct vec3_t queue[512];
extern uint8_t flags;

#endif