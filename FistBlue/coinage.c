/*
 *  coinage.c
 *  MT2
 *
 *  Created by Ben on 19/11/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include "sf2const.h"
#include "sf2macros.h"
#include "structs.h"

#include "lib.h"
#include "coinage.h"

extern Game g;

static void sub_dee(void) {
	g.x02db = g.x02db & 0xfff3;
}


void decode_start_service(void) {	// 1e7a was swirlything
	g.StartServiceButtons =
		((g.RawButtons0Dash & 0x4) >> 2 ) |
		((g.RawButtons0     & 0x4) >> 1 ) |
		((g.x0078           & 0x4) >> 0 ) |
		((g.x0079			& 0x4) << 1 );
	g.coinslot1.x0007 =
	((g.RawButtons0Dash & 0x1) << 3 ) |
	((g.RawButtons0     & 0x1) << 2 ) |
	((g.x0078           & 0x1) << 1 ) |
	((g.x0079			& 0x1) >> 0 );
	g.coinslot2.x0007 =
	((g.RawButtons0Dash & 0x2) << 2 ) |
	((g.RawButtons0     & 0x2) << 1 ) |
	((g.x0078           & 0x2) << 0 ) |
	((g.x0079			& 0x2) >> 1 );
}

static void sub_1d9a(void) {
	const static char data_1de0[][2] = {
		{1,1}, {1,2}, {1,3}, {1,4}, {1,6}, {2, 1}, {3, 1}, {4, 1},
	};
	
	
	g.coinslot1.nCoins   = data_1de0[g.JPCost & 0x7][0];
	g.coinslot1.nCredits = data_1de0[g.JPCost & 0x7][1];
	g.coinslot2.nCoins   = data_1de0[(g.JPCost & 0x38)>>3][0];
	g.coinslot2.nCredits = data_1de0[(g.JPCost & 0x38)>>3][1];
	g.ContinueCoin = (g.JPCost & 0x40) >> 6;
	g.DemoSound    = (g.JPCost & 0x80) >> 7;
}
void sub_1f9e(Coinslot *cs, char *a0) {
	if (cs->x0003) {
		if(--cs->x0003 != 15) {return;}
		g.x02db &= a0[0];
	} else {
		if (cs->x0001) {
			--cs->x0001;
			g.x02db |= a0[1];
			cs->x0003 = 30;
		}
	}
}

static void sub_1f5a(Coinslot *cs) {
	++cs->x0001;
	++cs->x0002;
	if (!g.ContinueCoin) {
		if (cs->x0002 < cs->nCoins) {
			return;
		}
	}
	g.NumberCredits += cs->nCredits;	// XXX should be BCD
	if (g.NumberCredits > 9)
		g.NumberCredits = 9;
	cs->x0002 = 0;
}
static void sub_1f1c(Coinslot *cs) {
	if (cs->x0000 == 0) {
		if (cs->x0007 != 3) {
			return;
		}
		++cs->x0000;
		cs->x0006 = 120;	// 2 seconds
	}
	if (cs->x0007 == 12) {
		++g.CoinsTaken;
		++g.SoundOutstanding;
		sub_1f5a(cs);
		cs->x0000 = 0;
	} else {
		if (--cs->x0006 == 0) {
			cs->x0000 = 0;
		}
	}
}
void sub_1ed0(void) {
	static const char data_1e76[]={0xfe, 0x01, 0xfd, 0x02};
	if (!g.InTestMode) {
		decode_start_service();
		sub_1f9e(&g.coinslot1, &data_1e76[0]);
		sub_1f1c(&g.coinslot1);
		sub_1f9e(&g.coinslot2, &data_1e76[2]);
		sub_1f1c(&g.coinslot2);
		
		if (g.StartServiceButtons == 3) {
			g.NumberCredits += 1;
			if (g.NumberCredits > 9) {
				g.NumberCredits = 9;
			} else {
				++g.CoinsTaken;
			}
			++g.SoundOutstanding;
		}
	}
}

void sub_1fe2(void) {
	if (g.SoundOutstanding) {
		--g.SoundOutstanding;
		coinsound();
		if (g.InDemo && g.FreePlay == 0 && g.NumberCredits) {
			QueueEffect(SL10 | 0x0, 0);
		}
	}
}
static void sub_6c68(void) {				// 6c68
	if (g.NumberCredits < 4) {
		if (g.JapanJumper) {
			QueueEffect((u16 []){
				SL04 | PUSH_1P_START, 
				SL04 | INSERT_ADDITIONAL,
				SL04 | PUSH_1POR2P_START,
				SL04 | PUSH_1POR2P_START,
			}[g.NumberCredits],0);
		} else {
			QueueEffect((u16 []){
			 	SL04 | PUSH_1P_START,
				SL04 | INSERT_ADDITIONAL,
				SL04 | PUSH_1P_START,
				SL04 | START1PORADD2P,
			}[g.NumberCredits],0);
		}
	} else {
		QueueEffect(SL04 | PUSH_1POR2P_START,0);
	}
}	

static void sub_6c38(void) {
	if(g.NumberCredits <= 1 && g.JapanJumper == FALSE) {
		QueueEffect(SL04 | PUSH_1P_START,0);
	} else {
		QueueEffect(SL04 | PUSH_1POR2P_START,0);
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
			QueueEffect(SL10 | 0, 0);
			fadenwait5(1);
			break;
		case 4:
			sub_6cc8();
			break;
		FATALDEFAULT;
	}
}
