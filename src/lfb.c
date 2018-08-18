#include "headers/project.h"

uint32_t pitch;
unsigned char *lfb;

/**
 * Set screen resolution to 1024x768
 */
void lfb_init()
{
	mailbox[0] = 35*4;
	mailbox[1] = MBOX_REQUEST;

	mailbox[2] = 0x48003;  //set phy wh
	mailbox[3] = 8;
	mailbox[4] = 8;
	mailbox[5] = 1024;         //FrameBufferInfo.width
	mailbox[6] = 768;          //FrameBufferInfo.height

	mailbox[7] = 0x48004;  //set virt wh
	mailbox[8] = 8;
	mailbox[9] = 8;
	mailbox[10] = 1024;        //FrameBufferInfo.virtual_width
	mailbox[11] = 768;         //FrameBufferInfo.virtual_height
	
	mailbox[12] = 0x48009; //set virt offset
	mailbox[13] = 8;
	mailbox[14] = 8;
	mailbox[15] = 0;           //FrameBufferInfo.x_offset
	mailbox[16] = 0;           //FrameBufferInfo.y.offset
	
	mailbox[17] = 0x48005; //set depth
	mailbox[18] = 4;
	mailbox[19] = 4;
	mailbox[20] = 32;          //FrameBufferInfo.depth

	mailbox[21] = 0x48006; //set pixel order
	mailbox[22] = 4;
	mailbox[23] = 4;
	mailbox[24] = 1;           //RGB, not BGR preferably

	mailbox[25] = 0x40001; //get framebuffer, gets alignment on request
	mailbox[26] = 8;
	mailbox[27] = 8;
	mailbox[28] = 4096;        //FrameBufferInfo.pointer
	mailbox[29] = 0;           //FrameBufferInfo.size

	mailbox[30] = 0x40008; //get pitch
	mailbox[31] = 4;
	mailbox[32] = 4;
	mailbox[33] = 0;           //FrameBufferInfo.pitch

	mailbox[34] = MBOX_TAG_LAST;

	if(mailbox_call(MBOX_CH_PROP) && mailbox[20]==32 && mailbox[28]!=0) {
		mailbox[28]&=0x3FFFFFFF;
		lfb_width=mailbox[5];
		lfb_height=mailbox[6];
		pitch=mailbox[33];
		lfb=(void*)((uint64_t)mailbox[28]);
	} else {
		uart_puts("Unable to set screen resolution to 1024x768x32\n");
	}
}

void lfb_draw_pixel(uint32_t x, uint32_t y, char r, char g, char b)
{
	unsigned char *ptr = lfb;
	uint32_t pixel;
	
	ptr += pitch * y;
	ptr += (x<<2);
	
	pixel = (b<<16)|(g<<8)|r;

	*((uint32_t *)ptr)=*((uint32_t *)&pixel);	
}

/**
 * Display a string
 */
void lfb_print(int32_t x, int32_t y, char *s)
{
	// get our font
	psf_t *font = (psf_t*)&_binary_src_font_font_psf_start;
	// draw next character if it's not zero
	while(*s) {
		// get the offset of the glyph. Need to adjust this to support unicode table
		unsigned char *glyph = (unsigned char*)&_binary_src_font_font_psf_start +
		 font->headersize + (*((unsigned char*)s)<font->numglyph?*s:0)*font->bytesperglyph;
		// calculate the offset on screen
		int32_t offs = (y * font->height * pitch) + (x * (font->width) * 4); //There was a +1 on font->width
		// variables
		int32_t i,j, line, mask, bytesperline=(font->width+7)/8;
		// handle carrige return
		if(*s=='\r') {
			x=0;
		} 
		else if(*s=='\n') 
		{
			x=0; y++;
		}
		else if(*s != 255)
		{
			// display a character
			for(j=0;j<font->height;j++)
			{
				// display one row
				line=offs;
				mask=1<<(font->width-1);
				for(i=0;i<font->width;i++)
				{
					// if bit set, we use white color, otherwise black
					*((uint32_t*)(lfb + line))=((int32_t)*glyph) & mask ? 0xFFFFFF : 0;
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


void lfb_special_print(int32_t x, int32_t y, char *s, bool set_background_color, char main_r, char main_g, char main_b, char back_r, char back_g, char back_b)
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
				*((uint32_t*)(lfb + line))=((int)*glyph) & mask ? rgb_to_hex(main_r, main_g, main_b) : rgb_to_hex(back_r, back_g, back_b);
						mask>>=1;
						line+=4;
			}
			else
			{
				if(((int)*glyph) & mask)
				{
					*((uint32_t*)(lfb + line))= rgb_to_hex(main_r, main_g, main_b);
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

void lfb_hex(uint32_t *x, uint32_t *y, uint32_t d)
{
	uint32_t n;
	int32_t c;
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
	uint64_t a,b,d, temp;
	uint32_t x = 0, y = 0;
	unsigned char c;
	for(a=(uint64_t)ptr;a<(uint64_t)ptr+512;a+=16) {
		lfb_hex(&x, &y, a); 
	lfb_print(x, y, ": ");
	x++;
	x++;
		for(b=0;b<16;b++) {
			c=*((unsigned char*)(a+b));
			d=(uint32_t)c;d>>=4;d&=0xF;d+=d>9?0x37:0x30;
		lfb_print(x, y, (char *)&d);
		x++;
			d=(uint32_t)c;d&=0xF;d+=d>9?0x37:0x30;
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

void lfb_clear()
{
	mailbox[0] = 7*4;
	mailbox[1] = MBOX_REQUEST;
	mailbox[2] = 0x00040002;
	mailbox[3] = 4;
	mailbox[4] = 4;
	mailbox[5] = 0;
	mailbox[6] = MBOX_TAG_LAST;
	mailbox_call(MBOX_CH_PROP);
}

void lfb_clear_rect()
{
	unsigned char *ptr = lfb;
	//Clearing the whole screen
	for(uint32_t y = 0; y < lfb_height; y++)
	{
		for(uint32_t x = 0; x < lfb_width; x++)
		{
			*((uint32_t*)ptr)=0x00000000;
			ptr += 4;
		}
		ptr+=pitch-lfb_width*4;
	}
}
