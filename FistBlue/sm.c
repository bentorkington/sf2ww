/* sf2 sm.c  State Managers */

#include	"sf2types.h"
#include    "sf2macros.h"
#include    "sf2const.h"

#include	"gstate.h"
#include    "structs.h"
#include	"player.h"

#include	"particle.h"
#include	"lib.h"
#include    "sm.h"
#include	"rules.h"
#include	"gfxlib.h"
#include	"fightgfx.h"
#include	"sprite.h"
#include	"actions.h"
#include	"sound.h"
#include	"playerselect.h"

#include "gemu.h"

#include <stdio.h>
#define DEBUG_SM(string)        \
		printf ("SM:");			\
		puts(string);			\
		



extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;

static short sub_7e86(void);

u16 data_BonusTimes[3][2] = {{0x40, 0x28}, {0x20, 0x00}, {0x40, 0x28}};

static void gamemode_init_round (void);


/* 7880 State for fight in progress */
void gamemode_fightmain (void) {
    if(check_if_new_player()) {
        redraw_fight_dsk();
        fightstuff();
    } else {
        bumpdifficulty_08();
        sub_8e8e();           /* check for timeout */
        if(g.RoundComplete) {
            g.mode2    += 2;
            g.RoundCnt++;
            draw_peacesigns();
            all_actions_sprites();
        } else {
            redraw_fight_dsk();
            fightstuff();
        }
    }
}
static void game_mode_26(void) {	// 7aea
	/* XXX */
}
static void sub_7db6() {
	g.mode1 = 0xc;
	g.mode2 = 0x4;
	g.mode3 = 0;
}
static void sub_7d5c(void) {
	ClearEffectQueue();		// 21c2
	die_top8();
	fadenwait3();
	sub_7db6();
}
static void CSPlayerContinueSound(void){		// 7d76 Player Continue Sound
	static const short CSContinueSounds[]={		// 7d86
		SOUND_SHORYUKEN, SOUND_OINK, SOUND_CANNONBALL, SOUND_GINK,
		SOUND_HUA, SOUND_CHUNLI_WIN, SOUND_LAUGH_MALE, SOUND_ELEPHANT,
	};
	soundsting(CSContinueSounds[g.BattleLoser]);
}
static void sub_7c50(void) {
	//7c50
	NEXT(g.mode1);
	g.mode2 = 0;
	g.mode3 = 0;
	if(sub_7e86()==0) {
		g.x02eb = TRUE;
		start_effect(LC0_LIGHT_ALL_ENABLE, 3);
		soundsting(SOUND_CONTINUE);
	}
}	


static void game_mode_28(void) {	// 7af0
	Object *obj;
	Player *ply;
	
	switch (g.mode2) {
		case 0:
			NEXT(g.mode2);
			g.FadeBusy = TRUE;
			QueueEffect(SL0C | SL0C_FO_CLW, 3);
			break;
		case 2:
			if (g.FadeBusy==0) {
				NEXT(g.mode2);
				sound_cq_f0f7();
				LBResetState();
				g.Palette1 = 0x11;
				palette_from_game();
				g.CPS.Scroll2X = 0;
				g.CPS.Scroll2Y = 0;
				g.CPS.Scroll1X = 0;
				g.CPS.Scroll1Y = 0;
				gstate_Scroll2.XPI = 0;
				gstate_Scroll2.YPI = 0;
				gstate_Scroll3.XPI = 0;
				gstate_Scroll3.YPI = 0;
				if (g.x0a50) {
					// 7b90
					g.mode2 = 0xa;
					g.Player1.Alive = FALSE;
					g.Player2.Alive = FALSE;
					if (g.AllowContinue || g.Debug_0x31e) {
						sub_7c50();
					}
				} else {
					g.NewChallengerWait = FALSE;
					action_draw_ports();
					action_print_chant();		/* do the chant */
					start_effect(LC0_LIGHT_ALL_ENABLE, 3);
					soundsting(SOUND_VICTORY);
				}

			}
		case 4:
			if (g.x02e0==0) {
				NEXT(g.mode2)
				g.timer3 = 0xb4;
			}
			break;
		case 6:
			set_waitmode();
			if (g.timer3-- == 0) {
				NEXT(g.mode2);
				g.WaitMode = FALSE;
				die_top8();
				g.FadeBusy = TRUE;
				QueueEffect(SL0C | SL0C_FO_CLW, 3);
			} else {
				Player *winner;
				Player *loser;
				
				if (g.PlyLostToPly) {
					winner = PLAYER1;
					loser  = PLAYER2;
				} else {
					winner = PLAYER2;
					loser  = PLAYER1;
				}
				if (g.NewPlayers & 1 << winner->Side) {
					loser->Alive = FALSE;
					g.PlyLostToComp = winner->Side;
				}
				if(check_if_new_player()) {
					GEMU_CLEAR_OBJECT_72;
					DSDrawAllMain();
				}
			}
			break;
		case 8:
			if (g.FadeBusy==0) {
				NEXT(g.mode2);
				LBResetState();
				if (g.PlyLostToPly) {
					g.Player2.Alive = FALSE;
				} else {
					g.Player1.Alive = FALSE;
				}
				g.PlyLostToComp = g.PlyLostToPly;
				if (g.AllowContinue==0) {
					sub_7c50();
				}
			}
		case 0xa:
			if (g.ContinueBits & 1 << g.PlyLostToComp ) {
				NEXT(g.mode2)
				g.WaitMode = FALSE;
				g.timer2 = 8;
				obj = AllocActor();
				obj->exists = TRUE;
				obj->Sel = SF2ACT_0X1B;
				obj->Scroll = 0;
				action_1606c(obj);
				if (g.Version == VERSION_JAP) {
					g.x5dfe.exists = TRUE;
					action_b8aa(obj);
				}
				start_effect(LC0_LIGHT_ALL_ENABLE, 2);
				proc_actions();
				DSDrawAll_Hira();		// a drawsprites
				soundsting(SOUND_CONTINUE);				
			}
		case 0xc:
			if (g.timer2-- == 0) {
				NEXT(g.mode2);
				g.NewChallengerWait = 0;
			} else {
				proc_actions();
				DSDrawAll_Hira();
			}
			break;
		case 0xe:
			if (g.PlyLostToComp) {
				ply = PLAYER2;		// XXX never read
			} else {
				ply = PLAYER1;
			}
			if (g.ContinueBits ) {
				// 7d56
				if (g.NewPlayers) {
					sub_7d5c();
				} else {
					proc_actions();
					DSDrawAll_Hira();
				}
			} else {
				// 7d12
				if (g.x02a5) {
					/* Player has continued, play sound and restart from world screen */
					NEXT(g.mode2);
					g.timer3 = 64;
					CSPlayerContinueSound();			/* make continue sound */
					proc_actions();
					DSDrawAll_Hira();
				} else if (g.NewPlayers) {
					sub_7d5c();
				} else {
					/* Continue timer ran out */
					NEXT(g.mode1);
					g.mode2 = 0;
					g.mode3 = 0;
					g.x02eb = TRUE;
					if (g.BattleLoser == FID_CHUN_LI) {
						soundsting(SOUND_KO_FEMALE);
					} else {
						soundsting(SOUND_KO_MALE);
					}
					proc_actions();
					DSDrawAll_Hira();
				}
			}
			break;
		FATALDEFAULT;
	}
}

static void sub_7dca(void) {		// 7dca game mode 2,A
	switch (g.mode2) {
		case 0:
			NEXT(g.mode2);
			g.x09fe = 0;
			break;
		case 2:
			if (g.PlayersOnline==0) {
				NEXT(g.mode2);
			}
			break;
		case 4:
			if (g.x09fe != 2) {
				NEXT(g.mode2);
				g.timer3 = 150;
				print_libtextgfx(GAME_OVER);
				soundsting(SOUND_GAME_OVER);
			}
			break;
		case 6:
			if(g.timer3--==0) {
				g.x02eb = 0;
				g.mode0=g.mode1=g.mode2=g.mode3=g.mode4 = 0;
				task_kill(3);
				//todo:		create_task(attractsequence, 1, 0, 0, 0);
				task_die();
			}
			break;
		FATALDEFAULT;
	}
	
}


static void sub_7eb4(void) {		// 7eb4 game mode 2,C
	switch (g.mode2) {
		case 0:
			g.timer2 = 0x78;
			g.timer3 = 0x8;
			g.timer4 = 0;
			action_1ab8a();		/* clear top object */
			QueueEffect(0x1c19, 0);
			QueueEffect(0x1c1a, 0);
			soundsting(SOUND_CHALLENGER);
			break;
		case 2:
			if (g.timer2-- == 0) {
				NEXT(g.mode2);
				ClearEffectQueue();		/* at 21c2 */
				die_top8();
				g.FadeBusy = TRUE;
				QueueEffect(0xc1c, 0x100);	/* FADE_CLEAR_ALL */
			} else {
				if(g.timer3--==0) {
					g.timer3 = 8;
					g.timer4 ^= 1;
					if(g.timer4) {
						QueueEffect(SL1C | HERE_COMES_2, 0);
						QueueEffect(SL1C | CHALLENGER, 0);
					} else {
						QueueEffect(SL1C | HERE_COMES, 0);
						QueueEffect(SL1C | CHALLENGER, 0);
					}
				}
			}
			break;
		case 4:
			if(g.FadeBusy==0) {
				NEXT(g.mode2);
				LBResetState();
			}
			break;
		case 6:
			SM_player_select();
			if (g.PlayerSelectDone) {
				g.mode1 = 4;
				g.mode2 = 0;
				g.mode3 = 0;
			}
			break;
		FATALDEFAULT;
	}
}

void SM_game (void) {			/* 76e0 */
    switch (g.mode0) {
    case 0:
        g.mode0 += 2;
        g.TwoHumans = (g.PlayersOnline == 0x3 ? 0 : 1);  
        /* copy protection not included */
        sound_cq_f0f7();
        LBResetState();
        newgame();
        clear_scrolls();
        break;
    case 2:
        switch (g.mode1) {
        case 0:
            SM_player_select();
            if(g.PlayerSelectDone) { g.mode1+=2; }
            break;
        case 2:
            g.mode1 += 2; 
            break;
        case 4:
            game_mode_24();   /* most of gameplay */
            break;
        case 6:
            game_mode_26();   /* game complete */
            break;
        case 8:
            game_mode_28();   /* wait for continue */
            break;
        case 0xa:
            sub_7dca();
            break;
        case 0xc:
            sub_7eb4();
            break;
        }
    }
}

static void draw_world_map(void) {		//856c mode 2,4,2
	Object *obj;
	set_waitmode();
		
	switch (g.mode3) {
		case 0:
			NEXT(g.mode3);
			DEBUG_SM("world map");
			g.timer4			= 60;
			g.Pause_9e1			= 0;
			g.NewChallengerWait = FALSE;
			g.CPS.DispEna		= 0x12da;
			g.Palette1			= 0x11;
			palette_macro_11();
			GSInitForStage();
			g.CPS.Scroll2X		= 0;
			g.CPS.Scroll2Y		= 0;

			gstate_Scroll3.XPI = 0x0;
			gstate_Scroll3.YPI = 0x700;
			GSSetupScr3(&gstate_Scroll3);	// draw the world map background

			g.CPS.Scroll1X = 0;
			g.CPS.Scroll1Y = 0;
			
			g.ActionLibSel = 0x11;
			actionlibrary();		/* 82d0e */
			
			if (g.Player1.Human) {
				PrintPlayerPic(PLAYER1, g.Player1.Side, g.Player1.FighterID);
			}
			if (g.Player2.Human) {
				PrintPlayerPic(PLAYER2, g.Player2.Side, g.Player2.FighterID);
			}
			start_effect(LC0_LIGHT_ALL_ENABLE, 0x1);
			break;
		case 2:
			switch (g.mode4) {
				case 0:
					if (g.OnBonusStage || g.CurrentStage == 8 ) {
						NEXT(g.mode4);
						check_if_new_player();
					} else if (g.Pause_9e1 == 0) {
						check_if_new_player();
					} else if (obj = AllocActor()) {
						obj->exists = TRUE;
						obj->Sel = SF2ACT_0X2E;
						synth_plane_setup(obj, g.CurrentStage, g.LastFightStage);
						NEXT(g.mode4);
						check_if_new_player();
					}
					break;
				case 2:
					if (g.Pause_9e1 < 0) {
						NEXT(g.mode4);	
						g.timer3 = 60;
					}
					check_if_new_player();
					break;
				case 4:
					if (--g.timer3 == 0) {
						NEXT(g.mode3);
						g.mode4 = 0;
					}
					check_if_new_player();
					break;
				FATALDEFAULT;
			}
			proc_actions();
			DSDrawAllMain();
			break;
		case 4:			// clear screen, next mode
			switch (g.mode4) {
				case 0:
					NEXT(g.mode4);
					g.FadeBusy = TRUE;
					QueueEffect(SL1C | CHALLENGER_2, 0x3);
					break;
				case 2:
					if (g.FadeBusy == FALSE) {
						NEXT(g.mode2);
						g.mode3		= 0;
						g.mode4		= 0;
						g.WaitMode	= FALSE;
						LBResetState();
					}
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}

}
void game_mode_24 (void) {		// 0x7786
    switch (g.mode2) {
        case 0:
			// Initialize 
			DEBUG_SM("init_fight_vars");
            init_fight_vars();
			NEXT(g.mode2);
            break;
        case 2:		
			// Show the world map and plane flight between battles
			if ( g.OnBonusStage || g.FirstFight ) {
                g.mode2 +=2;
                g.FirstFight = 0;
            } else {
                draw_world_map();
            }
            break;
			
        case 4: 
			// Show the VS screen unless bonus stage
			
            if(g.OnBonusStage) {
                g.mode2 += 2;            /* no vs screen for the bonus rounds */
            } else {
                gamemode_vs_screen();
            }
            break;
        case 6:   gamemode_init_round();          break;	// Init players and scene
        case 8:   gamemode_prefightanim(); break;	
        case 0xa: gamemode_fightmain();    break;
        case 0xc: 
            redraw_fight_dsk();
            gamemode_postfightanim();
			break;
        case 0xe:                   /* 0x78fc */
            proc_round_result();    /*sets next game mode*/
            bumpdifficulty_02();
            bumpdifficulty_03();
            bumpdifficulty_04();
            bumpdifficulty_05();
            bumpdifficulty_06();
            break;
        case 0x10:  gamemode_24G();    break;		// Fade out
        case 0x12:  gamemode_24I();    break;
		FATALDEFAULT;
    }
}

/* 86e8 game mode 2,4,4: VS screen */
void gamemode_vs_screen (void) {
    set_waitmode();
    switch(g.mode3) {
		case 0:
			DEBUG_SM("vs_screen");
			g.mode3   +=2;
			g.timer3		= VS_SCREEN_DELAY;
			g.CPS.DispEna	= 0x12da;
			g.Palette1		= 0x11;
			LBResetState();
			g.ActionLibSel	= 0x12;
			actionlibrary();			/* action 39x2 draw player names large */
			GSInitForStage();			   		  /* set up the scroll params */
			gstate_Scroll3.XPI  = 0x200;
			gstate_Scroll3.YPI  = 0x700;
			g.CPS.Scroll2X        = 0x0;
			
			g.CPS.Scroll2Y        = 0x0;
			GSSetupScr3(&gstate_Scroll3);
			actionlib_draw_portraits();
			palette_from_game();
			/* CPS anti-tampering @ 0x8756 not included */
			queuesound(SOUND_VS_SCREEN);
			start_effect(LC0_LIGHT_ALL_ENABLE,3); 
			// state of scroll3 is trashed here, system will crash if gstate_entry() is called.
			gstate_Scroll3.mode0 = 0x41;				
			gstate_Scroll3.mode1 = 0x42;  
			break;
		case 2:
			if(--g.timer3==0) {
				g.mode3 += 2;
				g.FadeBusy=TRUE;
				start_effect(0x0c1c, 0x3);
			} else {
				proc_actions();
				DSDrawAllMain();
				check_if_new_player();
			}
			break;
			
		case 4:   /* case 4 87c8 */
			if(!g.FadeBusy) {
				g.mode2 +=   2;      /* Go to 2,4,6 */
				g.mode3  =   0;
				g.WaitMode = 0;
			}
			
			break;
		FATALDEFAULT;
    }
}

/* 0x77dc game state 2,4,6,. Set up the stages, palettes, and fighter positions.*/
static void gamemode_init_round (void) {
    switch (g.mode3) {
		case 0:
			DEBUG_SM("init_round");
			NEXT(g.mode3);
			LBResetState();
			LBInitPlayers();		/* set player difficulties, initial energy */
			break;
		case 2:
			NEXT(g.mode3);
			palettes_nextlevel();
			set_shadow_pen();    /* set Object Pal 10 for current stage */
			break;
		case 4:
			NEXT(g.mode3);
			g.TimeRemainBCD   = 0x99;   
			g.TimeRemainTicks = 0x28;
			
			if(g.OnBonusStage) {
				g.TimeRemainBCD   = data_BonusTimes[g.CurrentStage - 0xc][0];
				g.TimeRemainTicks = data_BonusTimes[g.CurrentStage - 0xc][1];
			}
			init_fightgfx();        
			print_timeremaining();
			action_start_22();          /* start the Round X ... anim */
			setup_stage_actions(g.CurrentStage);
			break;
		case 6:
			if(g.GSInitComplete) {
				NEXT(g.mode3);
				set_initial_positions();
			} else {
				GSMain();
			}
			break;
		case 8:
			do {
				g.timer3 = gstate_Scroll2.X.part.integer;
				GSMain();
			} while(g.timer3 != gstate_Scroll2.X.part.integer);
			NEXT(g.mode2);		/* Go to 2,4,8 */
			g.mode3 =0;
			break;
		FATALDEFAULT;
    }
}            

 /* 0x87dc mode 2,4,8 */
void gamemode_prefightanim (void){     
    switch(g.mode3) {
        case 0: {
			DEBUG_SM("SM: PreFightAnim");
            g.mode3  +=2;
            setstagemusic();
            g.PreRoundAnim = TRUE;
            g.x0305        = 1;  /* flags for mbison cape */
            g.x0306        = 1;  /* cape thrown */
            
            start_effect(LC0_LIGHT_ALL_ENABLE, 3);
        }
        case 2: {
            check_if_new_player();
			g.PreRoundAnim = FALSE;
        }
			break;
		FATALDEFAULT;
    }
    if(!g.PreRoundAnim) {
        g.mode2 +=2;
        g.mode3 = 0;
    }
    proc_all_actions();
    DSDrawAllMain();
}

static void SM_setnextfight() {	/* 0x8a38 next fight */
	DEBUG_SM("SM: SetNextFight");
	NEXT(g.mode2);
	g.mode3 = g.mode4 = 0;
	g.WaitMode = FALSE;
}

static void SM_game_postanim_8(void) {
	Object *obj;
	set_waitmode();
	switch (g.mode4) {
		case 0:
			NEXT(g.mode4);
			g.timer4 = 0x3;
			g.CanSpeedUpScoreCount = FALSE;		/* u8 */
			g.x8ab2 = 0;			/* u16 */
			if(g.HumanWinner == 0) {
				g.timer4 = 0xb4;
			}
			break;
		case 2:
		case 6:
		case 0xa:
			if(g.timer4-- == 0) {
				NEXT(g.mode4);
				if(g.HumanWinner == 0) {SM_setnextfight();}
			}
			break;
		case 4:
			if(obj=AllocActor()) {
				INITOBJ(obj, SF2ACT_SCORECOUNTER, 0);
				print_libtextgfx(TIME);
				NEXT(g.mode4);
				g.timer4  = 30;
			}
			break;
		case 8:
			if(obj=AllocActor()) {
				INITOBJ(obj, SF2ACT_SCORECOUNTER, 2);
				print_libtextgfx(VITAL);
				NEXT(g.mode4);
				g.timer4 = 0x32;
			}
			break;
		case 0xc:
			if(obj=AllocActor()){
				INITOBJ(obj, SF2ACT_SCORECOUNTER, 4);
				print_libtextgfx(BONUS);
				NEXT(g.mode4);
			}
			break;
		case 0xe:
			if(g.x8ab4 == 0) {		/* u8 */
				NEXT(g.mode4);
				g.timer4 = 0x78;
			}
			break;
		case 0x10:
			if(--g.timer4 == 0) {
				SM_setnextfight();
			}
			break;
		FATALDEFAULT;
	}
}

void gamemode_postfightanim (void) {
	Object *obj;
	short temp;
	
    if(g.OnBonusStage) {       /* 0x8a4a */
        switch(g.mode3) {
            case 0:
				DEBUG_SM("SM: PostFightAnim");
                queuesound(0x30f9); /* fade out */
                if(g.CurrentStage != 0xd && g.TimeOut ) {  /* barrels */
                    g.mode3 += 2;
                    g.x0ae7  = TRUE;
                    g.timer3 = 0xb4;
                    print_libtextgfx(TIME_OVER);
                } else {
                    g.mode3  = 4;
                    if(!g.BonusComplete) {		
                        g.mode3 = 6;
                        g.x0ae7 = TRUE;
                    }
                }
				break;
            case 2: 
                if(--g.timer3==0){
                    g.mode3=6;
                    print_libtextgfx(LIBTEXT_ERASE | TIME_OVER);
                }
				break;
            case 4: 	/* 8ac6 */
				switch (g.mode4) {
					case 0:
						NEXT(g.mode4);
						g.timer4 = 0x40;
						sub_4794();			/* difficulty something */
						break;
					case 2:
						/* 8ae8 */
						if(g.timer4-- == 0) {
							NEXT(g.mode4);
							g.Pause_9e1 = 0;
							print_libtextgfx(LIBTEXT_ERASE + PERFECT);
							if (g.RoundResult < 0) {
								/* 8b26 */
								if (obj=AllocActor()) {
									INITOBJ(obj, 0x3f, 0);
								}
								if (obj=AllocActor()) {
									obj->exists=TRUE;
									obj->Sel = SF2ACT_0X3F;
									obj->SubSel = 1;
								}
							} else {
								/* 8b04 */
								if (g.RoundResult == 1) {
									if (obj=AllocActor()) {
										obj->exists = TRUE;
										obj->Sel = SF2ACT_0X3F;
										obj->SubSel = 0;
									}
								} else {
									if (obj=AllocActor()) {
										obj->exists = TRUE;
										obj->Sel = SF2ACT_0X3F;
										obj->SubSel = 1;
									}
								}

							}
						break;
					case 4:
						/* 8b58 */
							temp = g.RoundResult < 0 ? 2 : 1;
							if (g.Pause_9e1 == temp) {
								NEXT(g.mode4);
								g.timer4 = SF2ACT_BISONCAPE;
							}
						break;
					case 6:
						/* 8b78 */
						if (g.timer4-- == 0) {
							NEXT(g.mode3);
							g.mode4 = 0;
						}
						break;
					FATALDEFAULT;
				}
            }
			break;
            case 6: 				/* 8b88 */
				set_waitmode();
				switch  (g.mode4) {
					case 0:
						if (g.ActiveHumans == 3 && g.BonusComplete && g.RoundResult >= 0) {
							if (obj = AllocActor()) {
								obj->exists = TRUE;
								obj->Sel = SF2ACT_SPEAK_WINLOSE;	/* "You Win / Lose " */
								if (g.RoundResult & 0x1) {
									obj->SubSel = g.Player1.FighterID;
								} else {
									obj->SubSel = g.Player2.FighterID;
								}
								NEXT(g.mode4);
								g.timer3 = 0x3c;
								g.KillAct48 = FALSE;
							}
						} else {
							NEXT(g.mode4);/* 8bf2 */
						}
						break;
					case 2:
						/* 8bfa */
						if (g.timer3-- == 0) {
							g.KillAct48 = TRUE;			
							NEXT(g.mode4);
						}
						break;
					case 4:
						/* 8c0c */
						NEXT(g.mode2);
						g.Pause_9e1 = 0;
						if (obj=AllocActor()) {
							obj->exists = TRUE;
							obj->Sel = SF2ACT_0X40;
							obj->SubSel = 0x1;
						}
						break;
					case 6:
						if (g.Pause_9e1) {
							NEXT(g.mode4);
							g.Pause_9e1 = 0;
							if (obj=AllocActor()) { INITOBJ(obj,SF2ACT_0X40,1) }
						}
						break;
					case 8:
						/* 8c56 */
						if (g.Pause_9e1) {
							NEXT(g.mode4);
							g.timer4 = 0x3c;
						}
						break;
					case 10:
						/* 8c68 */
						if (g.timer4-- == 0) {
							NEXT(g.mode2);
							g.mode3 = g.mode4 = 0;
							g.WaitMode = FALSE;
						}
						break;
					FATALDEFAULT;
						
				}
				break;
        }
        all_actions_sprites();
    } else {
		
		switch (g.mode3) {
			case 0:
				DEBUG_SM("SM: PostFightAnim");
				if(g.TimeOut) {
					g.mode3 += 2;
					g.timer3 = 0xf0;
					print_libtextgfx(TIME_OVER);
				} else {
					g.mode3 = 4;
					g.WaitMode = FALSE;
				}
				break;
			case 2:
				if(g.timer3-- == 0) {
					g.mode3 += 2;
					g.WaitMode = FALSE;
					print_libtextgfx(LIBTEXT_ERASE | TIME_OVER) ;
				}
				break;
			case 4:
				/* 88a8 */
				if(g.RoundResult < 0) {
					g.mode3 += 2;
					g.timer3 = 0xb4;
					if(g.TimeResult<0) {
						print_libtextgfx(DOUBLE_KO);	
					} else {
						print_libtextgfx(DRAW_GAME);
					}
				} else {
					/* 0x88ca */
					g.mode3		+= 2;
					g.timer3	= 60;
					g.KillAct48 = FALSE;
					if(obj=AllocActor()) {
						obj->exists = TRUE;
						obj->Sel = SF2ACT_SPEAK_WINLOSE;		/* you win / lose */
						if(g.ActiveHumans == 3) {
							obj->SubSel = g.WinningFighter;
						} else if (g.HumanWinner) {
							obj->SubSel = 8;
						} else {
							obj->SubSel = 9;
						}
					}
					queuesound(0x30f9);	/* fade sound out */
				}
				break;
			case 6:
				/* 0x8914 */
				if(g.timer3-- == 0) {
					queuesound(0x30f9);
				} else {
					if(g.RoundResult < 0) {
						if(g.TimeResult < 0 ) {
							print_libtextgfx(LIBTEXT_ERASE | DOUBLE_KO);
						} else {
							print_libtextgfx(LIBTEXT_ERASE | DRAW_GAME);
						}
						g.mode2   += 2;
						g.mode3    = 0;
						g.WaitMode = FALSE;
					} else {
						g.mode3 = 8;
						g.KillAct48 = TRUE;
						g.WaitMode  = FALSE;
					}
				}
				break;
			case 8:
				SM_game_postanim_8();
				break;
			FATALDEFAULT;
		}
		if(g.TimeWarpTimer) {
			g.TimeWarpTimer--;		
			if(--g.TimeWarpSlowdown) {return;}
			g.TimeWarpSlowdown = g.TimeWarpSlowdown2;
		} 
		all_actions_sprites();
		
    }
}

static short sub_7e86(void) {
	if (g.NewPlayers) {
		g.mode1=0xc;
		g.mode2=0x6;
		g.mode3=0;
		g.mode4=0;
		g.mode5=0;
		g.WaitMode=FALSE;
		soundsting(SOUND_CHALLENGER);	/* new challenger */
		return 1;
	}
	return 0;
}

/* 7916 */
void gamemode_24G (void) {		//7916
    switch (g.mode3) {
    case 0:
			DEBUG_SM("gamemode_24G");
        g.mode3 +=2;
        g.FadeBusy = TRUE;
        start_effect(0xc1c, 3);
        break;
    case 2:
        if(g.FadeBusy == 0) {
            g.mode3 +=2;
            sound_cq_f0f7();
            g.timer3 = 5;
            g.NewChallengerWait = FALSE;
            LBResetState();
        }
        break;
    case 4:
        if (--g.timer3 == 0) {
            g.mode2 = 6;     /* Go to 2,4,6, game continues 1P */
            g.mode3 = 0;
            sub_7e86();         /* new challenger stuff */
        }
        break;
	FATALDEFAULT;
    }
}

void gamemode_24I (void) {		// 7970
    switch(g.mode3) {
		case 0:
			g.mode3 +=2;
			if(g.OnBonusStage == 0 && g.ActiveHumans == BOTH_HUMAN) {
				g.LevelCursor++;
				sub_4720();		/* some bumpdifficulty */
			}
			g.NoLoser = g.OnBonusStage;       /* don't show the chant screen for bonus stg */
			if (g.SkipEnding == 0) {
				sub_2b7c();
				if(g.CurrentStage == 0x10) {
					/* Bison has been beaten, so no chant */
					
					g.mode1 += 2;     /* Go to mode 2,6.*/
					g.BisonBeater = g.BattleWinner;
					g.BisonBeatSide = g.BattleWinSide;
					g.ClockingPly = g.BattleWinSide == 0 ? PLAYER1 : PLAYER2;
					if (g.SkipEnding || g.NotUsed == 0) {
						GEMU_CLEAR_OBJECT_72;       
					}
					g.BattleOver    = TRUE;
					g.x02ec         = 1;
					g.Player1.Alive = FALSE;
					g.Player2.Alive = FALSE;
					g.mode2			= 0;
					g.mode3			= 0;
				}					
			}    
			break;
		case 2:
			g.mode3 +=2;
			g.FadeBusy = TRUE;
			start_effect(0xc1c, 3);
			break;
		case 4:
			if (g.FadeBusy == 0) {

				g.mode3 +=2;
				LBResetState();
				sound_cq_f0f7();
				g.NewChallengerWait = FALSE;
				if(g.NoLoser == 0) {
					g.CPS.Scroll2X = 0;
					g.CPS.Scroll2Y = 0;
					g.Palette1 = 0x11;
					palette_from_game();
					queuesound(SOUND_VICTORY);
					action_draw_ports();
					action_print_chant();
					start_effect(LC0_LIGHT_ALL_ENABLE,3);
				} else {
					g.mode3 +=2;
					g.timer3 =5;
					g.WaitMode = 0;
				}
			}
			break;
		case 6:
			if(g.TextEffectBusy == FALSE) {
				g.mode3 += 2;
				g.timer3 = 180;
			}
			break;
		case 8:
			set_waitmode();
			g.timer3--;
			if(g.timer3 == 0) {
				g.mode3 +=2;
				die_top8();
				g.FadeBusy = TRUE;
				QueueEffect(SL1C | CHALLENGER_2,3);
			} else if(check_if_new_player()) {
				GEMU_CLEAR_OBJECT_72;             /* clear the "Press Start" message */
				DSDrawAllMain();
			}
			break;
		case 0xa:
			set_waitmode();
			if(g.FadeBusy == 0) {
				g.mode3 += 2;
				g.timer3 = 5;
			}
			break;
		case 0xc:
			g.timer3--;
			if(g.timer3 == 0) {
				g.mode2=0;       /* go to mode 2,4,0. */
				g.mode3=0;
				LBResetState();
			} else {
				sub_7e86();
			}
			break;
		FATALDEFAULT;
    }
}

 
void task_initmachine (void) {		// 639e
	static const u16 data_645e[3]={ 
		SL04 | SL4_VERSION_JAP,
		SL04 | SL4_VERSION_USA,
		SL04 | SL4_VERSION_ETC,
	};
	
    switch(g.mode0) {
		case 0:
			g.mode0 +=2;
			g.randSeed1 = 0x01;       /* initial random seed */
			g.randSeed2 = 0xc3; 
			g.WaitMode	= 0;
			break;
		case 2:
			g.mode0 +=2;
#ifdef COINAGE
			coincosts();
#endif
			decode_difficulty();
			decode_params();
			g.InDemo = TRUE;
			g.x02b8  = 0x100;
			palette_scr1_19();
#ifdef COINAGE
			check_coin_lockout();
#endif
			break;
		case 4:
			g.mode0  +=2;
			g.timer0 = 180;
			QueueEffect(LC0_LIGHT_ALL_ENABLE, 5);
			QueueEffect(data_645e[g.Version],0x0100); 
			break;
		case 6:
			if(g.TextEffectBusy == 0) {g.mode0 += 2;}
			break;
		case 8:
		case 0xe:
			g.timer0--;
			if(g.timer0 == 0) {
				g.mode0   += 2;
				g.WaitMode = 0;
				fadenwait1();
			} else {
				startup_impatience();
			}
			break;
		case 0xa:
			g.mode0 +=2;
			g.timer0 = 60;
			QueueEffect(LC0_LIGHT_ALL_ENABLE,5);
			QueueEffect((short []){
				SL04 | COPYRIGHT_JAP,
				SL04 | COPYRIGHT_USA,
				SL04 | COPYRIGHT_ETC,
			}[g.Version], 0x101);
			
			break;
		case 0xc:
			if(g.TextEffectBusy == 0) {
				g.WaitMode = 0;
				g.mode0   += 2;
			}
			startup_impatience();
			break;
		case 0x10:
			RESET_MODES;
			//if(g.RawButtons0Dash & BUTTON_TESTMENU || g.GameMode == 1) {
			//    create_task(&task_test_mode,7,0, 0, 0);
			//    task_die();
			//} 
			/* not included: copy protection with CPS1B */
			
			panic(1);
			//create_task(task_attract_sequence, 1, 0,0,0);
			
			break;
			FATALDEFAULT;
    }
}


