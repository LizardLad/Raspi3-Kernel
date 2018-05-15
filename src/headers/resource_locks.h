#ifndef RESOURCE_LOCKS_H
#define RESOURCE_LOCKS_H

/* 
 * This file describes resource locks used by the different cores
 * If a resource is in use the bool will be set to true
 * If a resource is ready to be used the bool will be set to false
 * The lock will be set by the functions the locks relate to
 * The actual variables are defined in this file
 *
 * Not much more to say than that :)
 */

typedef struct
{
	;
} GPIO_LOCKS_t;

typedef struct
{
	;
} LFB_LOCKS_t;

typedef struct
{
	;
} FAT_LOCKS_t;

typedef struct
{
	;
} UART_LOCKS_t;

typedef struct
{
	;
} MAILBOX_LOCKS_t;

/*
 * Actual lock variable definitions
 */

MAILBOX_LOCKS_t MAILBOX_LOCKS;
GPIO_LOCKS_t GPIO_LOCKS;
UART_LOCKS_t UART_LOCKS;
LFB_LOCKS_t  LFB_LOCKS;
FAT_LOCKS_t  FAT_LOCKS;

#endif
