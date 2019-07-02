#ifndef MBOX_CALLS_H
#define MBOX_CALLS_H

#include <stdbool.h>
#include "edid.h"

uint32_t get_videocore_firmware_rev();
uint32_t get_board_model();
uint32_t get_board_revision();
uint32_t get_board_mac_address();
uint32_t get_board_serial();
bool get_edid(struct edid_data *data);

#endif
