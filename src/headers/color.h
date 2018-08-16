#ifndef COLOR_H
#define COLOR_H

void hsv_to_rgb(double h, double s, double v, char *r, char *g, char *b);
uint32_t rgb_to_hex(uint8_t r, uint8_t g, uint8_t b);

#endif
