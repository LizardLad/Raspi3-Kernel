#include "headers/project.h"

void set_ACT_LED(bool on)
{
	mailbox[0] = 8*4;
	mailbox[7] = 0;
	mailbox[1] = 0;
	mailbox[2] = 0x00038041;
	mailbox[3] = 8;
	mailbox[4] = 8;
	mailbox[5] = 130;
	mailbox[6] = (uint32_t)on;
	uint32_t addr = (uint32_t)(uintptr_t)&mailbox[0];
	InvalidateDataCacheRange(addr, sizeof(mailbox)/ DATA_CACHE_LINE_LENGTH_MIN);
	mailbox_tag_write(addr);
	mailbox_tag_read();
}
