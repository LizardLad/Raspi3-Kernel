#include "include/stdint.h"

void hsv_to_rgb(double h, double s, double v, char *r, char *g, char *b)
{
	h = h > 360.0 ? 0.0 : h;
	double P, Q, T, temp_r, temp_g, temp_b, fract;

	(h == 360.0) ? (h = 0.0) : (h /= 60.0);

	fract = h - (int64_t)h;	

	P = v*(1.0 - s);
	Q = v*(1.0 - s * fract);
	T = v*(1.0 - s*(1.0 - fract));

	if(0. <= h && h < 1.)
	{
		temp_r = v; temp_g = T; temp_b = P;
	}
	else if(1. <= h && h < 2.)
	{
		temp_r = Q; temp_g = v; temp_b = P;
	}
	else if (2. <= h && h < 3.)
	{
		temp_r = P; temp_g = v; temp_b = T;
	}
	else if (3. <= h && h < 4.)
	{
		temp_r = P; temp_g = Q; temp_b = v;
	}
	else if (4. <= h && h < 5.)
	{
		temp_r = T; temp_g = P; temp_b = v;
	}
	else if (5. <= h && h < 6.)
	{
		temp_r = v; temp_g = P; temp_b = Q;
	}
	else
	{
		temp_r = 0; temp_g = 0; temp_b = 0;
	}
	*r = (char)(255 * temp_r);
	*g = (char)(255 * temp_g);
	*b = (char)(255 * temp_b);
}



uint32_t rgb_to_hex(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

