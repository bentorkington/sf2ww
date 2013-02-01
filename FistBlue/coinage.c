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
#include "sm.h"
#include "sound.h"
#include "coinage.h"
#include "actions.h"
#include "sound.h"
#include "task.h"
#include "effects.h"
#include "gfxlib.h"
#include "sf2io.h"
#include "sprite.h"

extern Game g;
extern struct executive_t Exec;


#define COIN_STATUS_1	0x1
#define COIN_STATUS_2	0x2
#define COIN_LOCK_1		0x4
#define COIN_LOCK_2		0x8

static void sub_dee(void) {
	g.CoinStatus = g.CoinStatus & ~(COIN_STATUS_1 | COIN_STATUS_2);
}
void decode_start_service(void) {	// 1e7a was swirlything
	g.StartServiceButtons =
		((g.RawButtons0Dash		& IPT_SERVICE) >> 2 ) |
		((g.RawButtons0			& IPT_SERVICE) >> 1 ) |
		((g.RawButtons0Dash2    & IPT_SERVICE) >> 0 ) |
		((g.RawButtons0Dash3	& IPT_SERVICE) << 1 );
	g.coinslot1.shifted_bits =
		((g.RawButtons0Dash		& IPT_COIN1) >> 0 ) |
		((g.RawButtons0			& IPT_COIN1) << 1 ) |
		((g.RawButtons0Dash2    & IPT_COIN1) << 2 ) |
		((g.RawButtons0Dash3	& IPT_COIN1) << 3 );
	g.coinslot2.shifted_bits =
		((g.RawButtons0Dash		& IPT_COIN2) >> 1 ) |
		((g.RawButtons0			& IPT_COIN2) >> 0 ) |
		((g.RawButtons0Dash2	& IPT_COIN2) << 1 ) |
		((g.RawButtons0Dash3	& IPT_COIN2) << 2 );
}

void decode_coincosts(void) {			// 1d9a
	const static char data_1de0[][2] = {
		{1,1}, {1,2}, {1,3}, {1,4}, {1,6}, {2, 1}, {3, 1}, {4, 1},
	};
	
	g.coinslot1.nCoins   = data_1de0[g.JPCost & JP_COSTMASK1][0];
	g.coinslot1.nCredits = data_1de0[g.JPCost & JP_COSTMASK1][1];
	g.coinslot2.nCoins   = data_1de0[(g.JPCost & JP_COSTMASK2)>>3][0];
	g.coinslot2.nCredits = data_1de0[(g.JPCost & JP_COSTMASK2)>>3][1];
	g.TwoCreditsToStart  = (g.JPCost & JP_CONTINUECOIN) >> 6;
	g.DemoSound          = (g.JPCost & 0x80) >> 7;
}
static void sub_1f9e(Coinslot *cs, const u8 *a0) {
	if (cs->debounce_timer) {
		if(--cs->debounce_timer == 15) {
			g.CoinStatus &= a0[0];
		}
	} else {
		if (cs->count_minor) {
			--cs->count_minor;
			g.CoinStatus |= a0[1];
			cs->debounce_timer = 30;
		}
	}
}

static void coin_accepted(Coinslot *cs) {		// 1f5a
	printf("coin_accepted\n");
	++cs->count_minor;
	++cs->count_major;
	if (!g.TwoCreditsToStart) {
		if (cs->count_major < cs->nCoins) {
			return;
		}
	}
	g.NumberCredits += cs->nCredits;	// should be BCD, but doesn't matter
	if (g.NumberCredits > 9)			// as NumberCredits is limited to 9
		g.NumberCredits = 9;
	cs->count_major = 0;
}
static void sub_1f1c(Coinslot *cs) {
	if (cs->x0000 == 0) {
		if (cs->shifted_bits == 0x3) {
			++cs->x0000;
			cs->holdoff_timer = 120;	// 2 seconds
		}
		return;
	}
	if (cs->shifted_bits == 0xc) {
		++g.CoinsTaken;
		++g.SoundOutstanding;
		coin_accepted(cs);
		cs->x0000 = 0;
	} else {
		if (--cs->holdoff_timer == 0) {
			cs->x0000 = 0;
		}
	}
}
void sub_1ed0(void) {			// 1ed0
	static const u8 data_1e76[]={
		~COIN_STATUS_1, COIN_STATUS_1,
		~COIN_STATUS_2, COIN_STATUS_2,
	};
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

void int_cb_coinage(void) {			// 1fe2
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
	if(g.TwoCreditsToStart) {
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
void check_coin_lockout(void) {		//dfc 
	if (g.NumberCredits >= 9) {
		g.CoinStatus &= 0xf3;
	} else {
		g.CoinStatus |= (COIN_LOCK_1 | COIN_LOCK_2);
	}
}

void task_creditscreen(void) {          /* 6b52 */
    Object *act;
	
    Exec.FadeOutComplete = FALSE;
    QueueEffect(LC0_DARK_ALL_DISABLE,1);
    while(Exec.FadeOutComplete == FALSE) { sf2sleep(1); }
    clear_scrolls();
    sf2sleep(1);
    g.CPS.Scroll1X   = 0x0;
    g.CPS.Scroll1Y = 0x100;
    LBResetState();
    sound_cq_f7_ff();
    palette_macro(0x10);
    if (act = AllocActor()) {
        act->exists = TRUE;
        act->Sel    = SF2ACT_SF2LOGO;
    }
    if (act = AllocActor()) {
        act->exists = TRUE;
        act->Sel    = SF2ACT_WWLOGO;
        act->SubSel = 1;
    }
    if (act = AllocActor()) {
        act->exists = TRUE;
        act->Sel    = SF2ACT_CAPCOMLOGO;
    }
	
    while(TRUE) {
		if (g.RawButtons0Dash & IPT_SERVICE_NOTOGGLE || (g.Debug & (!g.JPCost & 0x80))) {
            //init_test_menu();
            return;
        }
        if (0 == g.FreezeMachine) {
            switch (g.mode0) {
				case 0:
					g.mode0 +=2;
					g.NumberCreditsDash = g.NumberCredits;
					if (g.TwoCreditsToStart) {
						sub_6c68();			/* print 2 to start, 1 to cont */
					} else {
						sub_6c38();
					}
					break;
				case 2:
					g.mode0 +=2;
					QueueEffect(SL10 | 0x0, 0x0);
					fadenwait5(1);
					break;
				case 4:
					SMFreePlay();  /* only way out */
            }
			proc_actions();
			DSDrawAllMain();
            check_coin_lockout();
            sub_6c24();             /* update the "press 1P start" display */
        }
        debughook(0);		/* not correct value */
        sf2sleep(1);
    }
}

