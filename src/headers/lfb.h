#ifndef LFB_H
#define LFB_H

void lfb_init();
void lfb_draw_pixel(unsigned int x, unsigned int y, char r, char g, char b);
void lfb_hex(unsigned int *x, unsigned int *y, unsigned int d);
void lfb_dump(void *ptr);
void lfb_file_dump(void *ptr);
void lfb_print(int x, int y, char *s);
void lfb_special_print(int x, int y, char *s, bool set_background_color, char main_r, char main_g, char main_b, char back_r, char back_g, char back_b);

#endif
