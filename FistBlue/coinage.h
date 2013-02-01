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
void sub_1ed0(void);

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