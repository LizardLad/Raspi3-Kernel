#ifndef MBOX_H
#define MBOX_H

#include <stdbool.h>

/* a properly aligned buffer */
extern volatile uint32_t mailbox[36];

#define MBOX_REQUEST    0

/* channels */
#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

/* tags */

/* Hardware info commands */
#define	MBOX_TAG_GET_BOARD_MODEL	0x00010001	// Get board model
#define	MBOX_TAG_GET_BOARD_REVISION	0x00010002	// Get board revision
#define	MBOX_TAG_GET_BOARD_MAC_ADDRESS	0x00010003	// Get board MAC address
#define	MBOX_TAG_GET_BOARD_SERIAL	0x00010004	// Get board serial
#define	MBOX_TAG_GET_ARM_MEMORY		0x00010005	// Get ARM memory
#define	MBOX_TAG_GET_VC_MEMORY		0x00010006	// Get VC memory
#define	MBOX_TAG_GET_CLOCKS		0x00010007	// Get clocks

#define MBOX_TAG_SET_POWER		0x28001
#define MBOX_TAG_SET_CLKRATE		0x38002
#define MBOX_TAG_SET_VOLTAGE		0x38003
#define MBOX_TAG_GET_MAX_CLKRATE	0x30004

#define CLK_UART_ID		0x2		//Mailbox tag channel UART clock id
#define CLK_ARM_ID		0x3		//Mailbox tag channel ARM clock id
#define CLK_V3D_ID		0x5 		//Mailbox tag channel V3D clock id

#define MBOX_TAG_EXECUTE_QPU	0x00030011
#define MBOX_TAG_ENABLE_QPU	0x00030012

#define MBOX_TAG_ALLOCATE_MEMORY	0x0003000C	// Allocate Memory
#define MBOX_TAG_LOCK_MEMORY		0x0003000D	// Lock memory
#define MBOX_TAG_UNLOCK_MEMORY		0x0003000E	// Unlock memory
#define MBOX_TAG_RELEASE_MEMORY 	0x0003000F	// Release Memory

#define MBOX_TAG_EXECUTE_CODE	0x00030010	// Execute code

/* Temp Commands */
#define MBOX_TAG_GET_TEMPERATURE	0x00030006
#define MBOX_TAG_GET_MAX_TEMPERATURE	0x0003000A

/* GPIO commands */
#define MAILBOX_TAG_GET_GET_GPIO_STATE	0x00030041	// Get GPIO state
#define MAILBOX_TAG_SET_GPIO_STATE	0x00038041	// Set GPIO state

/* Mailbox status */
#define MAIL_EMPTY	0x40000000	/* Mailbox Status Register: Mailbox Empty */
#define MAIL_FULL	0x80000000	/* Mailbox Status Register: Mailbox Full  */

/* Mailbox structure tag */
#define MBOX_TAG_LAST           0

int32_t mailbox_call(uint8_t ch);
bool mailbox_tag_write(uint32_t message);
uint32_t mailbox_tag_read();
bool mailbox_tag_message (uint32_t* response_buf,	// Pointer to response buffer 
			  uint8_t data_count,		// Number of uint32_t data to be set for call
			  ...);
#endif
