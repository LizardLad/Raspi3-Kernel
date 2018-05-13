#include "headers/project.h"

void system_init()
{
	/* set up clock for consistent divisor values */
	mailbox[0] = 8*4;
	mailbox[1] = MBOX_REQUEST;
	mailbox[2] = MBOX_TAG_SETVOLTAGE; // set clock rate
	mailbox[3] = 8;
	mailbox[4] = 3;		//ARM CPU voltage
	mailbox[5] = 3;
	mailbox[7] = MBOX_TAG_LAST;
	mailbox_call(MBOX_CH_PROP);
	
	/* set up clock for consistent divisor values */
	mailbox[0] = 8*4;
	mailbox[1] = MBOX_REQUEST;
	mailbox[2] = MBOX_TAG_SETCLKRATE; // set clock rate
	mailbox[3] = 12;
	mailbox[4] = 8;
	mailbox[5] = 3;           // ARM CPU clock
	mailbox[6] = 500000000;   // 500Mhz
	mailbox[7] = MBOX_TAG_LAST;
	mailbox_call(MBOX_CH_PROP);
}
