#include <stdint.h>
#include "include/mbox.h"

#define ACT_LED_PORT 130

void set_ACT_LED(bool on)
{
	mailbox_tag_message(0, 5, MAILBOX_TAG_SET_GPIO_STATE, 8, 8, ACT_LED_PORT, (uint32_t)on);
}
