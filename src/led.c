#include "headers/project.h"

void set_ACT_LED(bool on)
{
	mbox[0] = 8*4;
	mbox[7] = 0;
	mbox[1] = 0;
	mbox[2] = 0x00038041;
	mbox[3] = 8;
	mbox[4] = 8;
	mbox[5] = 130;
	mbox[6] = (uint32_t)on;
	mbox_call(0x8);
}
