/* demo logic */

#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"
#include "gstate.h"
#include "player.h"
#include "structs.h"

#include "demo.h"
#include "lib.h"
#include "gfxlib.h"
#include "particle.h"
#include "sprite.h"
#include "task.h"
#include "actions.h"
#include "sound.h"
#include "task.h"
#include "coinage.h"
#include "sm.h"

extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;
extern struct executive_t Exec;


static void sub_65a2(void) {
	Task *task = CURRENT_TASK;
	if (g.FreePlay) {
		SMFreePlay();		// check for start btns
	} else if (g.NumberCredits) {
		g.mode0 = g.timer0 =
		g.mode1 = g.timer1 =
		g.mode2 = g.timer2 =
		g.mode3 = g.timer3 =
		g.mode4 = g.timer4 =
		g.mode5 = g.timer5 = 0;
		ClearEffectQueue();
		die_top8();
		// sound sub_62ec();
		create_task(task_creditscreen, 2, 0, 0, 0);
		DIEFREE;
	}
}


static void SMDemo(void) {
	switch (g.mode0) {
		case 0:
			NEXT(g.mode0);
			g.CPS.Scroll1X = 0; g.CPS.Scroll1Y = 0x100; 
			LBResetState(); 
			break;
		case 0x2: SMdemo_titlefightanim(); break;
		case 0x4: SMdemo_fade_and_clear(); break;
		case 0x6: SMdemo_winnersusedrugs(); break;
		case 0x8: SMdemo_fade_and_clear(); break;
		//case 0xa: SMdemo_fight(); break;
		case 0xc: SMdemo_fade_and_clear(); break;
		//case 0xe: SMdemo_show_high_scores(); break;
		case 0x10: SMdemo_fade_and_clear(); break;
		case 0x12: 	
			g.mode0 = g.mode1 = g.mode2 = g.mode3 = g.mode4 = 0;
			g.x02dc = TRUE;
			die_top8();
			break;
		FATALDEFAULT;
	}
	check_coin_lockout();
}
void task_attractSequence(void) {
	g.InDemo = TRUE;
	g.CPS.DispEna = 0x12da;
	fadenwait1();
	sound_cq_f0f7();
	while (TRUE) {
		if (g.RawButtons0Dash & 0x40 || (g.Debug & (!g.JPCost & 0x80))) {
			//todo initTestMenu(); //207c
		} else if (!g.FreezeMachine) {
			SMDemo();
			check_coin_lockout();
			sub_65a2();		// exit if coins inserted
			//debughook(0);
		}
		sleep2();
	}
}



void SMdemo_fade_and_clear(void) {
    switch (g.mode1) {
    case 0:
        g.mode1 +=2;
        start_effect(0,3);
    case 2:
        if (g.x5d56) {
            g.mode0 += 2;
            g.mode1  = 0;
            clear_scrolls();
        }
    }
}
static void sub_6704(void) {
	Object *obj;
	if (obj=AllocActor()) {
		obj->exists = TRUE;
		obj->Sel = 0x12;
		obj->Scroll = 4;
		obj->XPI = 192;
		obj->YPI = 1956;
		obj->SubSel = (char []) {
			1,2,1,2,2,1,2,1,1,2,1,2,2,1,2,1
		}[RAND16];
	}
}

void SMdemo_titlefightanim(void) {	// 0x6650 mode is 2,0
    switch (g.mode1) {
		case 0:
			NEXT(g.mode1);
			g.Pause_9e1		= 0;
			g.Palette1		= 16;
			g.ActionLibSel	= 0;
			LBResetState();
			sub_6704();     /* setup street fighter logo animation */
			actionlibrary();
			palette_macro_10();
			GSInitForStage();
			g.CPS.DispEna = 0x079a;
			
			gstate_Scroll2.XPI =    0x0;
			gstate_Scroll2.YPI =  0x500;
			GSSetupScr2(&gstate_Scroll2);
			
			gstate_Scroll3.XPI =    0x0;
			gstate_Scroll3.YPI =  0x600;
			GSSetupScr3(&gstate_Scroll3);
			g.x02dc = TRUE;
			if(g.FreePlay) {
				QueueEffect(SL08 | INSERT_COIN, 0x101);
			} else {
				QueueEffect(SL08 | FREE_PLAY  , 0x101);
			}
			start_effect(2,3);
			queuesound(SOUND_DEMOMUSIC);
		case 2: if(g.Pause_9e1) {g.mode1 +=2;}
		case 4: 
			gstate_Scroll2.Y.full+= 0x00008000;    /*half, parallax scroll*/
			gstate_Scroll3.YPI   += 1;
			if (gstate_Scroll3.YPI > 0x700) {
				g.mode1 +=2;
				gstate_Scroll3.YPI = 0x700;
				g.timer3 = 180;      /* 180 (decimal) ticks */
				start_effect(0x0c1e, 3);
			}
		case 6:
			if (!g.timer2--) {
				g.mode1 +=2;
				g.Pause_9e1 = -1;
				g.timer2 = 750;
			}
		case 8:
			if (!g.timer2--) {
				g.mode0 +=2;
				g.mode1  =0;
				if (g.Version != VERSION_USA) { g.mode0 = 0x8; }
			}
    }
    proc_actions();
    draw_background();
	DSDrawAllMain();
}

void SMdemo_winnersusedrugs (void) {
#ifdef DEBUG
    printf("Winners don't use drugs!\n");
#endif
    g.mode0 +=2;
    /* winners don't port crap they don't need */
}


    
