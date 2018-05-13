#include "headers/project.h"

void system_init()
{
	/* set up clock for consistent divisor values */
	mbox[0] = 8*4;
	mbox[1] = MBOX_REQUEST;
	mbox[2] = MBOX_TAG_SETVOLTAGE; // set clock rate
	mbox[3] = 8;
	mbox[4] = 3;		//ARM CPU voltage
	mbox[5] = 3;
	mbox[7] = MBOX_TAG_LAST;
	mbox_call(MBOX_CH_PROP);
	
	/* set up clock for consistent divisor values */
		mbox[0] = 8*4;
		mbox[1] = MBOX_REQUEST;
		mbox[2] = MBOX_TAG_SETCLKRATE; // set clock rate
		mbox[3] = 12;
		mbox[4] = 8;
		mbox[5] = 3;           // ARM CPU clock
		mbox[6] = 500000000;   // 500Mhz
		mbox[7] = MBOX_TAG_LAST;
		mbox_call(MBOX_CH_PROP);
}
