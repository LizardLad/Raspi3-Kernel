#ifndef LFB_H
#define LFB_H

#include "stdbool.h"

uint32_t lfb_height, lfb_width;
unsigned char *lfb;

/* PC Screen Font as used by Linux Console */
typedef struct {
        uint32_t magic;
        uint32_t version;
        uint32_t headersize;
        uint32_t flags;
        uint32_t numglyph;
        uint32_t bytesperglyph;
        uint32_t height;
        uint32_t width;
        unsigned char glyphs;
} __attribute__((packed)) psf_t;
extern volatile unsigned char _binary_src_font_font_psf_start;

void lfb_init();
void lfb_draw_pixel(uint32_t x, uint32_t y, char r, char g, char b);
void lfb_hex(uint32_t *x, uint32_t *y, uint32_t d);
void lfb_dump(void *ptr);
void lfb_file_dump(void *ptr);
void lfb_print(int32_t x,int32_t y, char *s);
void lfb_special_print(int32_t x,int32_t y, char *s, bool set_background_color, char main_r, char main_g, char main_b, char back_r, char back_g, char back_b);

void lfb_clear();
void lfb_clear_rect();

#endif
