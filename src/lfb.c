#include "headers/project.h"

#define CONVERT_PIXEL(data,pixel) {\
pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
data += 4; \
}

/* PC Screen Font as used by Linux Console */
typedef struct {
	unsigned int magic;
	unsigned int version;
	unsigned int headersize;
	unsigned int flags;
	unsigned int numglyph;
	unsigned int bytesperglyph;
	unsigned int height;
	unsigned int width;
	unsigned char glyphs;
} __attribute__((packed)) psf_t;
extern volatile unsigned char _binary_src_font_font_psf_start;

unsigned int width, height, pitch;
unsigned char *lfb;

/**
 * Set screen resolution to 1024x768
 */
void lfb_init()
{
	mbox[0] = 35*4;
	mbox[1] = MBOX_REQUEST;

	mbox[2] = 0x48003;  //set phy wh
	mbox[3] = 8;
	mbox[4] = 8;
	mbox[5] = 1024;         //FrameBufferInfo.width
	mbox[6] = 768;          //FrameBufferInfo.height

	mbox[7] = 0x48004;  //set virt wh
	mbox[8] = 8;
	mbox[9] = 8;
	mbox[10] = 1024;        //FrameBufferInfo.virtual_width
	mbox[11] = 768;         //FrameBufferInfo.virtual_height
	
	mbox[12] = 0x48009; //set virt offset
	mbox[13] = 8;
	mbox[14] = 8;
	mbox[15] = 0;           //FrameBufferInfo.x_offset
	mbox[16] = 0;           //FrameBufferInfo.y.offset
	
	mbox[17] = 0x48005; //set depth
	mbox[18] = 4;
	mbox[19] = 4;
	mbox[20] = 32;          //FrameBufferInfo.depth

	mbox[21] = 0x48006; //set pixel order
	mbox[22] = 4;
	mbox[23] = 4;
	mbox[24] = 1;           //RGB, not BGR preferably

	mbox[25] = 0x40001; //get framebuffer, gets alignment on request
	mbox[26] = 8;
	mbox[27] = 8;
	mbox[28] = 4096;        //FrameBufferInfo.pointer
	mbox[29] = 0;           //FrameBufferInfo.size

	mbox[30] = 0x40008; //get pitch
	mbox[31] = 4;
	mbox[32] = 4;
	mbox[33] = 0;           //FrameBufferInfo.pitch

	mbox[34] = MBOX_TAG_LAST;

	if(mbox_call(MBOX_CH_PROP) && mbox[20]==32 && mbox[28]!=0) {
		mbox[28]&=0x3FFFFFFF;
		width=mbox[5];
		height=mbox[6];
		pitch=mbox[33];
		lfb=(void*)((unsigned long)mbox[28]);
	} else {
		uart_puts("Unable to set screen resolution to 1024x768x32\n");
	}
}

void lfb_draw_pixel(unsigned int x, unsigned int y, char r, char g, char b)
{
	unsigned char *ptr = lfb;
	char pixel[4], data_array[4], *data;

	data = &(data_array[0]);
	data++;
	
	ptr += pitch * y;
	ptr += 4 * x;
	
	pixel[0] = r;
	pixel[1] = g;
	pixel[2] = b;
	pixel[3] = 0;

	*((unsigned int *)ptr)=*((unsigned int *)&pixel);	
}

/**
 * Display a string
 */
void lfb_print(int x, int y, char *s)
{
	// get our font
	psf_t *font = (psf_t*)&_binary_src_font_font_psf_start;
	// draw next character if it's not zero
	while(*s) {
		// get the offset of the glyph. Need to adjust this to support unicode table
		unsigned char *glyph = (unsigned char*)&_binary_src_font_font_psf_start +
		 font->headersize + (*((unsigned char*)s)<font->numglyph?*s:0)*font->bytesperglyph;
		// calculate the offset on screen
		int offs = (y * font->height * pitch) + (x * (font->width) * 4); //There was a +1 on font->width
		// variables
		int i,j, line, mask, bytesperline=(font->width+7)/8;
		// handle carrige return
		if(*s=='\r') {
			x=0;
		} else
		// new line
		if(*s=='\n') {
			x=0; y++;
		} else {
			// display a character
			for(j=0;j<font->height;j++){
				// display one row
				line=offs;
				mask=1<<(font->width-1);
				for(i=0;i<font->width;i++){
					// if bit set, we use white color, otherwise black
			*((unsigned int*)(lfb + line))=((int)*glyph) & mask ? 0xFFFFFF : 0;
					mask>>=1;
					line+=4;
				}
				// adjust to next line
				glyph+=bytesperline;
				offs+=pitch;
			}
			x++;
		}
		// next character
		s++;
	}
}


void lfb_special_print(int x, int y, char *s, bool set_background_color, char main_r, char main_g, char main_b, char back_r, char back_g, char back_b)
{
	// get our font
	psf_t *font = (psf_t*)&_binary_src_font_font_psf_start;
	// draw next character if it's not zero
	while(*s) {
		// get the offset of the glyph. Need to adjust this to support unicode table
		unsigned char *glyph = (unsigned char*)&_binary_src_font_font_psf_start +
		 font->headersize + (*((unsigned char*)s)<font->numglyph?*s:0)*font->bytesperglyph;
		// calculate the offset on screen
		int offs = (y * font->height * pitch) + (x * (font->width) * 4); //There was a +1 on font->width
		// variables
		int i,j, line, mask, bytesperline=(font->width+7)/8;
		// handle carrige return
		if(*s=='\r') {
			x=0;
		} else
		// new line
		if(*s=='\n') {
			x=0; y++;
		} else {
			// display a character
			for(j=0;j<font->height;j++){
				// display one row
				line=offs;
				mask=1<<(font->width-1);
				for(i=0;i<font->width;i++){
					// if bit set, we use white color, otherwise black
			if(set_background_color == true)
			{
				*((unsigned int*)(lfb + line))=((int)*glyph) & mask ? rgb_to_hex(main_r, main_g, main_b) : rgb_to_hex(back_r, back_g, back_b);
						mask>>=1;
						line+=4;
			}
			else
			{
				if(((int)*glyph) & mask)
				{
					*((unsigned int*)(lfb + line))= rgb_to_hex(main_r, main_g, main_b);
									mask>>=1;
									line+=4;	    
				}
				else
				{
					mask >>= 1;
					line += 4;
				}
			}
				}
				// adjust to next line
				glyph+=bytesperline;
				offs+=pitch;
			}
			x++;
		}
		// next character
		s++;
	}
}

void lfb_hex(unsigned int *x, unsigned int *y, unsigned int d)
{
	unsigned int n;
	int c;
	for(c=28;c>=0;c-=4)
	{
		n=(d>>c)&0xF;
		n+=n>9?0x37:0x30;
		lfb_print(*x, *y, (char *)&n);
		*x += 1;
	}
}

void lfb_dump(void *ptr)
{
	unsigned long a,b,d, temp;
	unsigned int x = 0, y = 0;
	unsigned char c;
	for(a=(unsigned long)ptr;a<(unsigned long)ptr+512;a+=16) {
		lfb_hex(&x, &y, a); 
	lfb_print(x, y, ": ");
	x++;
	x++;
		for(b=0;b<16;b++) {
			c=*((unsigned char*)(a+b));
			d=(unsigned int)c;d>>=4;d&=0xF;d+=d>9?0x37:0x30;
		lfb_print(x, y, (char *)&d);
		x++;
			d=(unsigned int)c;d&=0xF;d+=d>9?0x37:0x30;
		lfb_print(x, y, (char *)&d);
		x++;
			lfb_print(x, y, " ");
		x++;
			if(b%4==3)
		{
				lfb_print(x, y, " ");
			x++;
		}
		}
		for(b=0;b<16;b++) {
			c=*((unsigned char*)(a+b));
		temp = (c<32 || c >= 127? '.' : c);
			lfb_print(x, y, (char *)&temp);
		x++;
		}
		x = 0;
		y++;
	}
}
