/*
 *  coinage.c
 *  MT2
 *
 *  Created by Ben on 19/11/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include "coinage.h"
#include "sf2const.h"
#include "sf2macros.h"
#include "structs.h"

#include "lib.h"

extern Game g;

static void sub_dee(void) {
	g.x02db = g.x02db & 0xfff3;
}

static void sub_6c68(void) {
	if (g.NumberCredits < 4) {
		if (g.JapanJumper) {
			QueueEffect((u16 []){0x40b, 0x40d, 0x40c, 0x40c}[g.NumberCredits],0);
		} else {
			QueueEffect((u16 []){0x40b, 0x40d, 0x40b, 0x40e}[g.NumberCredits],0);
		}
	} else {
		QueueEffect(0x40c,0);
	}
}	

static void sub_6c38(void) {
	if(g.NumberCredits <= 1 && g.JapanJumper == FALSE) {
		QueueEffect(0x040b,0);	// Push 1P Start
	} else {
		QueueEffect(0x040c,0);	// Push 1P or 2P start
	}
	
}

static void sub_6c24(void) {
	if(g.ContinueCoin) {
		// 6c58 Two credits required to start, one to continue
		if (g.NumberCredits != g.NumberCreditsDash) {
			g.NumberCreditsDash = g.NumberCredits;
			sub_6c68();
		}
	} else {
		// 6c2a
		if(g.NumberCredits != g.NumberCreditsDash) {
			g.NumberCreditsDash = g.NumberCredits;
			sub_6c38();
		}
	}
}


static void sub_6cc8(void) {		// 6cc8 jumped straight to in freeplay
	u16 buttons = (!g.RawButtons0 & g.RawButtons0Dash);
	if (buttons & (BUTTON_P1ST | BUTTON_P2ST)) {
		if (g.x0302) {
			g.x0302 = 0;
		}
		if (buttons & BUTTON_P2ST) {
			if (g.FreePlay) {
				g.Debug_0x31e = TRUE;
				startgame(BOTH_HUMAN);
			} else {
				// ... do when not drunk
			}
		} else if (buttons & BUTTON_P1ST) {
			if (g.FreePlay) {
				g.Debug_0x31e = FALSE;
				startgame(ONLY_P1);
			} else {
				// ... again, sober
			}
		}
	}
}

void task_creditscreen(void) {
	switch (g.mode0) {
		case 0:
			NEXT(g.mode0);
			g.NumberCreditsDash = g.NumberCredits;
			if (g.ContinueCoin) {
				sub_6c68();
			} else {
				sub_6c38();
			}
			break;
		case 2:
			NEXT(g.mode0);
			QueueEffect(0x1000, 0);
			fadenwait5(1);
			break;
		case 4:
			sub_6cc8();
			break;
		FATALDEFAULT;
	}
}
