#ifndef COLOR_H
#define COLOR_H

void hsv_to_rgb(double h, double s, double v, char *r, char *g, char *b);
unsigned int rgb_to_hex(char r, char g, char b);

#endif
