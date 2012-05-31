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
#include "playerselect.h"
#include "collision.h"
#include "fightgfx.h"
#include "rules.h"
#include "effects.h"

extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;
extern struct executive_t Exec;

void SMdemo_fight(void);


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
		task_die();
	}
}

static void SMdemo_show_high_scores(void) {		//6aaa
	switch (g.mode1) {
		case 0:
			NEXT(g.mode1);
			g.timer1    = 0xb4;
			g.Palette1  = 0x10;
			palette_macro(0x10);
			GSInitForStage();
			gstate_Scroll2.XPI = 0x200;
			gstate_Scroll2.YPI = 0x700;
			GSSetupScr2(&gstate_Scroll2);
			
			gstate_Scroll3.XPI = 0x200;
			gstate_Scroll3.YPI = 0x700;
			GSSetupScr3(&gstate_Scroll3);
			
			QueueEffect(SL10 | 0x2, 0);
			QueueEffect(0x02, 3);
			queuesound(0x14);
			break;
		case 2:
			if (--g.timer1 == 0) {
				NEXT(g.mode0);
			}
			break;
		FATALDEFAULT;
			
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
		case 0xa: SMdemo_fight(); break;
		case 0xc: SMdemo_fade_and_clear(); break;
		case 0xe: SMdemo_show_high_scores(); break;
		case 0x10: SMdemo_fade_and_clear(); break;
		case 0x12: 	
			g.mode0 = g.mode1 = g.mode2 = g.mode3 = g.mode4 = 0;
			g.DemoStarted = TRUE;
			die_top8();
			break;
		FATALDEFAULT;
	}
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



void SMdemo_fade_and_clear(void) {		// 6618
    switch (g.mode1) {
    case 0:
        g.mode1 +=2;
        start_effect(0,3);
    case 2:
        if (Exec.FadeOutComplete) {
            g.mode0 += 2;
            g.mode1  = 0;
            clear_scrolls();
        }
    }
}
static void sub_6704(void) {		// SF2 logo, spinning and scaling
	Object *obj;
	if (obj=AllocActor()) {
		obj->exists = TRUE;
		obj->Sel    = 0x12;
		obj->Scroll = SCROLL_3;
		obj->XPI    = 192;
		obj->YPI    = 1956;
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
			palette_macro(0x10);
			GSInitForStage();
			g.CPS.DispEna = 0x079a;
			
			gstate_Scroll2.XPI =    0x0;
			gstate_Scroll2.YPI =  0x500;
			GSSetupScr2(&gstate_Scroll2);
			
			gstate_Scroll3.XPI =    0x0;
			gstate_Scroll3.YPI =  0x600;
			GSSetupScr3(&gstate_Scroll3);
			
			g.DemoStarted = TRUE;
			if(g.FreePlay) {
				QueueEffect(SL08 | INSERT_COIN, 0x101);
			} else {
				QueueEffect(SL08 | FREE_PLAY  , 0x101);
			}
			start_effect(2,3);
			queuesound(SOUND_DEMOMUSIC);
			break;
		case 2: if(g.Pause_9e1) {g.mode1 += 2;}
			break;
		case 4: 
			gstate_Scroll2.Y.full+= 0x00008000;    /*half, parallax scroll*/
			gstate_Scroll3.YPI   += 1;
			if (gstate_Scroll3.YPI > 0x700) {
				g.mode1 +=2;
				gstate_Scroll3.YPI = 0x700;
				g.timer3 = 180;      /* 180 (decimal) ticks */
				start_effect(0x0c1e, 3);
			}
			break;
		case 6:
			if (!g.timer2--) {
				g.mode1 +=2;
				g.Pause_9e1 = -1;
				g.timer2 = 750;
			}
			break;
		case 8:
			if (!g.timer2--) {
				g.mode0 +=2;
				g.mode1  =0;
				if (g.Version != VERSION_USA) { g.mode0 = 0x8; }
			}
			break;
		FATALDEFAULT;
    }
    proc_actions();
    draw_background();
	DSDrawAllMain();
}

void SMdemo_winnersusedrugs (void) {		
	switch (g.mode1) {
		case 0:
			NEXT(g.mode1);
			g.timer2 = 0xb4;
			LBResetState();
			actionlibrary();
			palette_macro(0x10);
			GSInitForStage();
			g.CPS.DispEna = 0x6da;
			gstate_Scroll2.XPI = 0;
			gstate_Scroll2.YPI = 0x700;
			GSSetupScr2(&gstate_Scroll2);
			start_effect(0x2, 3);
			break;
		case 2:
			if (--g.timer2 == 0) {
				NEXT(g.mode0);
				g.mode1 = 0;
			}
			break;
		FATALDEFAULT;
	}
}

static void setupdemofight(void) {		// 69e2
	do {
		g.timer4 = gstate_Scroll2.XPI;
		GSMain();
	} while (g.timer4 != gstate_Scroll2.XPI);
	NEXT(g.mode3);
	g.mode4 = 0;
	//todo  create_task(konami_94888, 5, 0, 0, 0);
	setstagemusic();
	g.PreRoundAnim = TRUE;
	start_effect(0x02, 3);
}
static void sub_6a54(void) {
	NEXT(g.mode0);
	g.mode1 = g.mode2 = g.mode2 = 0;
	g.x0302 = FALSE;
	sound_cq_f0f7();
	die_top8();
	//task_kill(5);		// kill the konami watcher
}	
static void sub_6a78(void) {
	if (g.TimeWarpTimer) {
		if (--g.TimeWarpTimer == 0) {
			g.FlagTimeWarp = FALSE;	
		}
		if (--g.TimeWarpSlowdown != 0) {
			--g.libsplatter;
			return;
		}
		g.TimeWarpSlowdown = g.TimeWarpSlowdown2;
	}
	proc_all_actions();
	CDCheckPlayers();
	sub_7e4dc();
}
static void sub_6964(void) {			// 6964 demo fight sm
	switch (g.mode3) {
		case 0:
			NEXT(g.mode3);
			init_fight_vars();
			break;
		case 2:
			switch (g.mode4) {
				case 0:
					NEXT(g.mode4);
					LBResetState();
					LBInitPlayers();
					break;
				case 2:
					NEXT(g.mode4);
					palettes_nextlevel();
					set_shadow_pen();
				case 4:
					NEXT(g.mode4);
					g.TimeRemainBCD = 0x99;
					g.TimeRemainTicks = 60;
					init_fightgfx();
					print_timeremaining();
					action_start_22();
					setup_stage_actions();
					break;
				case 6:
					if (g.GSInitComplete) {
						NEXT(g.mode4);
						set_initial_positions();
					} else {
						GSMain();
					}
					break;
				case 8:
					setupdemofight();
					break;
				FATALDEFAULT;
			}
			break;
		case 4:
			if (g.PreRoundAnim == FALSE) {
				NEXT(g.mode3);
				g.mode4 = 0;
			}
			proc_actions();
			sub_7e4dc();
			break;
		case 6:
			if (--g.DemoFightTimer == 0) {
				sub_6a54();
				return;
			} else {
				LBCheckRoundResult();
				if (g.RoundComplete) {
					sub_6a54();
					return;
				}
				redraw_fight_dsk();
				sub_6a78();
			}
			break;
		FATALDEFAULT;
	}
}

void SMdemo_fight(void) {			// 6826
	static const u16 data_910c4[] = {
		0x0000, 0x004C, 0x0001, 0x000B, 0x0000, 0x0007, 0x0001, 0x000B, 
		0x0000, 0x000E, 0x0004, 0x000D, 0x0000, 0x0020, 0x0008, 0x0011, 
		0x0000, 0x0018, 0x0002, 0x0010, 0x0000, 0x000D, 0x0004, 0x000C, 
		0x0000, 0x0009, 0x0001, 0x000B, 0x0000, 0x0005, 0x0008, 0x000E, 
		0x0000, 0x0046, 0x0010, 0x0009, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
	};
	static const u16 data_91114[] = {
		0x0000, 0x003B, 0x0001, 0x000C, 0x0000, 0x000A, 0x0001, 0x000A, 
		0x0000, 0x001B, 0x0004, 0x000E, 0x0000, 0x0011, 0x0002, 0x000D, 
		0x0000, 0x0028, 0x0004, 0x0009, 0x0000, 0x0011, 0x0001, 0x000C, 
		0x0000, 0x0014, 0x0001, 0x0009, 0x0000, 0x0064, 0x0010, 0x0002, 
		0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
	};
	static const u16 data_91164[] = {
		0x0000, 0x002D, 0x0009, 0x000A, 0x0008, 0x0001, 0x0000, 0x000E, 
		0x0009, 0x0007, 0x0001, 0x0003, 0x0000, 0x0027, 0x0002, 0x000A, 
		0x0000, 0x000F, 0x0004, 0x000D, 0x0000, 0x002F, 0x0010, 0x0005, 
		0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
		0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
	};
	static const u16 data_911b4[] = {
		0x0000, 0x0049, 0x0001, 0x0001, 0x0009, 0x0006, 0x0000, 0x000E, 
		0x0001, 0x0001, 0x0009, 0x0004, 0x0001, 0x0004, 0x0000, 0x001A, 
		0x0002, 0x000F, 0x0000, 0x000F, 0x0004, 0x000F, 0x0000, 0x001A, 
		0x0008, 0x0013, 0x0000, 0x0007, 0x0001, 0x000E, 0x0000, 0x0010, 
		0x0004, 0x000E, 0x0000, 0x000D, 0x0002, 0x000C, 0x0000, 0x0008, 
		0x0002, 0x0008, 0x0000, 0x000B, 0x0002, 0x000A, 0x0000, 0x000D, 
		0x0010, 0x0007, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
	};

	static const u16 data_91224[] = {
		0x0000, 0x0061, 0x0001, 0x0008, 0x0000, 0x0013, 0x0001, 0x0008, 
		0x0000, 0x0019, 0x0008, 0x0006, 0x0000, 0x007B, 0x0002, 0x0009, 
		0x0000, 0x000F, 0x0001, 0x000A, 0x0000, 0x004B, 0x0010, 0x0007, 
		0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
		0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
	};
	static const u16 data_91274[]= {
		0x0000, 0x004A, 0x0001, 0x0008, 0x0000, 0x0019, 0x0001, 0x0009, 
		0x0000, 0x001E, 0x0001, 0x0008, 0x0000, 0x001F, 0x0008, 0x0007, 
		0x0000, 0x0011, 0x0002, 0x0008, 0x0000, 0x0039, 0x0001, 0x0008, 
		0x0000, 0x0014, 0x0002, 0x0008, 0x0000, 0x000D, 0x0004, 0x0009, 
		0x0000, 0x005B, 0x0010, 0x000B, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
	};
	static const u16 data_912c4[] = {
		0x0000, 0x0042, 0x0001, 0x0009, 0x0000, 0x000B, 0x0001, 0x0009, 
		0x0000, 0x0014, 0x0001, 0x0009, 0x0000, 0x0010, 0x0008, 0x0006, 
		0x0000, 0x0011, 0x0002, 0x000A, 0x0000, 0x000C, 0x0002, 0x0009, 
		0x0000, 0x0033, 0x0100, 0x000D, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
		0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 
	};
	static const u16 data_91314[] = {
		0x0000, 0x0037, 0x0001, 0x0008, 0x0000, 0x0017, 0x0001, 0x0007, 
		0x0000, 0x001C, 0x0001, 0x0009, 0x0000, 0x001C, 0x0002, 0x0009, 
		0x0000, 0x002A, 0x0008, 0x000A, 0x0000, 0x0020, 0x0002, 0x000A, 
		0x0000, 0x0023, 0x0002, 0x0005, 0x0000, 0x0055, 0x0010, 0x000C, 
		0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 	
	};
		
		
	static const u16 *joyscripts_p1[] = {
		data_910c4,
		data_91114,
		data_91164,
		data_911b4,
	};
	static const u16 *joyscripts_p2[] = {
		data_91224,
		data_91274,
		data_912c4,
		data_91314,
	};
	
	switch (g.mode1) {
		case 0:
			switch (g.mode2) {
				case 0:
					NEXT(g.mode2);
					
					if (g.JPCost & 0xc0) {
						// todo @6884
					} else {
						g.DemoJoyP1=joyscripts_p1[g.DemoStageIndex];
						g.DemoJoyP2=joyscripts_p2[g.DemoStageIndex];
					}
					++g.DemoStageIndex;
					g.DemoStageIndex &= 3;
					g.DemoFightTimer = 0x708;
					g.DemoJoyP1Timer = 1;
					g.DemoJoyP2Timer = 1;
					g.FastEndingFight = FALSE;
					g.x0302 = TRUE;
					g.Player1.SelectComplete = FALSE;
					g.Player2.SelectComplete = FALSE;
					g.Player1.Side = 0;
					g.Player2.Side = 1;
					g.Player1.Opponent = PLAYER2;
					g.Player2.Opponent = PLAYER1;
					g.Player1.Continuing = FALSE;
					g.Player2.Continuing = FALSE;
					g.Player2.Alive = 2;
					g.Player1.Alive = 2;
					g.Player2.BlinkerMode0 = 0;
					g.Player1.BlinkerMode0 = 0;
					g.Player1.Human = TRUE;
					g.Player2.Human = TRUE;
					break;
				case 2:
					// 691c
					NEXT(g.mode1);
					g.mode2 = 0;
					g.x0318 = 0;
					sound_cq_f0f7();
					LBResetState();
					newgame();
					clear_scrolls();
					break;
				FATALDEFAULT;
			}
			break;
		case 2:
			// 6938
			LBGetInputs();
			LBDecodeInputs();
			++g.libsplatter;		// x001d
			switch (g.mode2) {
				case 0:
					SM_player_select();
					if (g.PlayerSelectDone) {
						NEXT(g.mode2);
					}
					break;
				case 2:
					sub_6964();
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}
    
