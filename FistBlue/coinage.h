/*
 *  coinage.h
 *  MT2
 *
 *  Created by Ben on 19/11/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */
#ifndef _SF2_COINAGE
#define _SF2_COINAGE

void task_creditscreen(void);
void check_coin_lockout(void);
void decode_coincosts(void);
void int_cb_coinage(void);
void coinage_hook(void);


/*!
 @struct coinslot
 @abstract defines a coin acceptor
 @field x0000 0x0000 unknown
 @field count_minor 0x0001
 @field count_major 0x0002
 @field debounce_timer 0x0003
 @field nCoins 0x0004
 @field nCredits 0x0005
 @field holdoff_timer 0x0006
 @field shifted_bits 0x0007
 */
struct coinslot {
	char x0000;
	char count_minor;
	char count_major;
	char debounce_timer;
	char nCoins;
	char nCredits;
	char holdoff_timer;
	char shifted_bits;
};

typedef struct coinslot Coinslot;

#endif