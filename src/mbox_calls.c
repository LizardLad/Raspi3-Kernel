//This file defines all calls to the mailbox that return a value about the hardware
#include <stdbool.h>
#include <stdint.h>
#include "include/mbox.h"
#include "include/edid.h"


uint32_t get_videocore_firmware_rev()
{
	uint32_t buff[4];
	mailbox_tag_message(buff, 4, MBOX_TAG_GET_FIRMWARE_REV, 0, 4, 0);
	for(int i = 0; i < 4; i++)
	{
		printf("%x\n", buff[i]);
	}
	return 0;
}

uint32_t get_board_model()
{
	uint32_t buff[4];
	mailbox_tag_message(buff, 4, MBOX_TAG_GET_BOARD_MODEL, 0, 4, 0);
	for(int i = 0; i < 4; i++)
	{
		printf("%x\n", buff[i]);
	}
	return 0;
}

uint32_t get_board_revision()
{
	uint32_t buff[4];
	mailbox_tag_message(buff, 4, MBOX_TAG_GET_BOARD_REVISION, 0, 4, 0);
	for(int i = 0; i < 4; i++)
	{
		printf("%x\n", buff[i]);
	}
	return 0;
}

uint32_t get_board_mac_address()
{
	uint32_t buff[6];
	mailbox_tag_message(buff, 4, MBOX_TAG_GET_BOARD_MAC_ADDRESS, 0, 6, 0);
	for(int i = 0; i < 6; i++)
        {
                printf("%x\n", buff[i]);
        }
        return 0;
}

uint32_t get_board_serial()
{
	uint32_t buff[8];
	mailbox_tag_message(buff, 4, MBOX_TAG_GET_BOARD_SERIAL, 0, 8, 0);
	for(int i = 0; i < 8; i++)
        {
                printf("%x\n", buff[i]);
        }
        return 0;
}

//uint32_t get_arm_memory() TODO
//uint32_t get_gpu_memory_split() TODO
//uint32_t get_clocks() somehow uses a parent clock and a child clock? TODO

bool get_edid(struct edid_data *data)
{
	uint32_t buff[36];
	if(1 == mailbox_tag_message(buff, 4, MBOX_TAG_GET_EDID, 0, 4, 0)) //Since I only want the first block this should work
	{
		data = (struct edid_data*)&(buff[2]);
		return true; //Success
	}
	return false; //Failed
}

