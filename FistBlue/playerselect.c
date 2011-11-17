/*
 *  playerselect.c
 *  GLUTBasics
 *
 *  Created by Ben on 15/02/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

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
#include	"actions.h"
#include	"sound.h"
#include	"sprite.h"

#include "playerselect.h"

extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;


#pragma mark Player Select
static void check_for_new_players(void) {		// 843e
	Player *a3, *a4;
	short new_fighter_id;
	if (g.NewPlayers) {
		g.PLSL.ChallengerDelay = 4;
		if (g.NewPlayers & 1) {
			a3 = PLAYER2;
			a4 = PLAYER1;
			new_fighter_id = FID_RYU;
		} else {
			a3 = PLAYER1;
			a4 = PLAYER2;
			new_fighter_id = FID_KEN;
		}
		if (a4->Continuing) {
			a4->Continuing = FALSE;
			new_fighter_id = a4->FighterSelect;
		}
		if (a3->FighterID == new_fighter_id) {		
			new_fighter_id++;
			new_fighter_id &= 7;
		}
		a4->FighterID       = new_fighter_id;
		a4->Human           = TRUE;
		g.NewPlayers        = 0;
		g.PLSL.TimerCourse  = 0x20;	// 20 seconds BCD
		g.PLSL.TimerFine	= 60;
		queuesound(SOUND_CHALLENGER);
	}
}
static void timer_tick(void) {		//8498
	if (!g.PLSL.TimeExpired) {	
		if (--g.PLSL.TimerFine == 0) {
			g.PLSL.TimerFine = 60;
			if (g.PLSL.TimerCourse == 0) {
				g.PLSL.TimeExpired = TRUE;
			} else {
				sub_bcd_8(1, &g.PLSL.TimerCourse);
			}
		}
	}
}
static void update_player_display(Player *ply) {		// 8352
	print_libtextgfx(ply->Side ? 0x18 : 0x17);
	PrintPlayerPic(ply, ply->Side, ply->FighterID);
}
static int _update_cursor_vert(u16 d6) {		// 8554
	int d2 = 0;
	if (d6 & 2) {
		d2 = -4;
	}
	if (d6 & 4) {
		d2 = 4;
	}
	return d2;
}
static int _update_cursor_horiz(u16 d6){		// 853c
	int d1 = 0;
	if (d6 & 1) {
		d1 = 1;
	}
	if (d6 & 2) {
		d1 = -1;
	}
	return d1;
}
static u16 _update_ply_cursor(Player *ply, Player *plb) {		// 84c6 ply in %a4, plb in %a3
	u16 d6 = (~ply->JoyDecodeDash.full) & ply->JoyDecode.full;
	
	int d1,d2;
	short newFighterID = ply->FighterID;
	short oldFighterID = newFighterID;
	
	d1=_update_cursor_horiz(d6);
	d2=_update_cursor_vert(d6);
	
	if ((ply->FighterID & 4) == ((newFighterID + d1) & 4)) {
		newFighterID += d1;
	}
	if ((newFighterID + d2 >= 0) && (newFighterID + d2) <8) {
		newFighterID += d2;
	}
	if (plb->Human && plb->FighterID == newFighterID) {
		newFighterID = oldFighterID;	// not same as orig
	}
	ply->FighterID = newFighterID;
	return d6;
}
static void _player_selected(Player *ply /* %a4 */) {			// 8306
	queuesound(SOUND_PLAYERSELECT);
	if (g.PLSL.PLSLNewPlayers & (1 << ply->Side)) {
		g.PLSL.x5dc8 = ply->FighterID;
	}
	ply->SelectDelayTmr = 45;
	
	if (g.ActiveHumans == 0) {
		++g.UsageCount[ply->FighterID];
	}
	if (!g.PlayersSelected) {
		g.PlayersSelected = TRUE;
		g.PlayersSelectedDash = ply->FighterID;
		copy_level_table(ply->FighterID);	
	}
	update_player_display(ply);
}
static void _check_cursor_changes(Player *ply_a4, Player *ply_a3) {		// 82c6
	u16 joydecode = _update_ply_cursor(ply_a4, ply_a3);
	if (!g.PLSL.TimeExpired) {
		if (g.ActiveHumans == 0) {
			if (g.PLSL.ChallengerDelay) {
				--g.PLSL.ChallengerDelay;
				joydecode &= BUTTON_MASK;
				if (joydecode) {
					_player_selected(ply_a4);
				} else {
					update_player_display(ply_a4);
				}
			} else {
				if ((~g.RawButtons0 & g.RawButtons0Dash) & (ply_a4->Side ? 0x20 : 0x10)) {
					_player_selected(ply_a4);
				} else {
					joydecode &= BUTTON_MASK;
					if (joydecode) {
						_player_selected(ply_a4);
					} else {
						update_player_display(ply_a4);
					}					
				}
			}
			return;			
		}
	}
	update_player_display(ply_a4);
}
static void _check_player(Player *pla, Player *plb) {		// 829e pla is %a4
	if (g.PLSL.IntroDelay == 0 && pla->SelectComplete == FALSE) {
		if (pla->SelectDelayTmr == 0) {
			_check_cursor_changes(pla, plb);
			return;
		} else {
			if(--pla->SelectDelayTmr == 0) {
				pla->SelectComplete = TRUE;
			}
		}
	}
	update_player_display(pla);
}

void SM_player_select(void) {		//7fc4
	Player *a3, *a4;
	short d0, d1;
	Object *obj;
	
	//printf("fatal: SM_player_select not done()\n");
	// XXX 
	//NEXT(g.mode1);
	//return;
	static const u16 data_80be[16]={
		0x0ee7, 0x0ff0, 0x0f00, 0x006d, 0x0eef, 0x00af, 0x0060, 0x0777, 
		0x004a, 0x0fc7, 0x0fa6, 0x0f85, 0x0e64, 0x0d43, 0x0c02, 0x0000, 
	};
	switch (g.PLSL.mode0) {
		case 0:
			switch (g.PLSL.mode1) {
				case 0:
					NEXT(g.PLSL.mode1);
					g.FirstFight				= TRUE;
					g.Player1.SelectDelayTmr	= 0;
					g.Player2.SelectDelayTmr	= 0;
					g.NewChallengerWait			= TRUE;
					g.PLSL.IntroDelay			= 30;
					g.PlayerSelectDone			= FALSE;
					g.PLSL.ChallengerDelay      = 0;
					g.Pause_9e1					= 0;
					g.CPS.DispEna				= 0x12da;		// xx
					g.Palette1					= 17;
					palette_from_game();	// XXX check that's 0x1644
					GSInitForStage();
					g.CPS.Scroll2X				= 0;
					g.CPS.Scroll2Y				= 0;
					gstate_Scroll3.XPI		= 0;
					gstate_Scroll3.YPI		= 0x700;
					GSSetupScr3(&gstate_Scroll3);
					g.CPS.Scroll1X				= 0;
					g.CPS.Scroll1Y				= 0x100;
					g.PLSL.TimerCourse			= 32;
					g.PLSL.TimerFine			= 60;		// XXX maybe split 2x char
					g.ActionLibSel				= 1;
					gemu_setpalette(PALETTE_0C, data_80be);
					soundsting(SOUND_PLAYERSELECTSCR);
					cqsave(0x1c16, 0);
					actionlibrary();    // 82d0e
					/* CPS does a short busy-sleep here */
					/* 0x8094 maybe palette comparison here */
					if (0) {
						cqsave(0x2878, 0);		//suicide
					}
					break;
				case 2:				//80de
					NEXT(g.PLSL.mode1);
					if (g.NewPlayers) {
						if (g.NewPlayers == 3) {
							//8164 Two Human Players
							if (g.Player1.Continuing && g.Player2.Continuing) {
								//81ba
								g.PLSL.PLSLNewPlayers = 1;
								g.NewPlayers = 0;
								g.Player1.FighterID = g.Player1.FighterSelect;
								g.Player2.FighterID = g.Player2.FighterSelect;
								if (g.Player1.FighterID == g.Player2.FighterID) {
									g.Player2.FighterID++;
									g.Player2.FighterID &= 0x7;
								}
								g.Player1.Human = TRUE;
								g.Player2.Human = TRUE;
							} else {
								// 8170
								if (g.Player1.Continuing) {
									a3 = PLAYER2;
									a4 = PLAYER1;
									d1 = 4;
								} else {
									a3 = PLAYER1;
									a4 = PLAYER2;
									d1 = 0;
								}
								a4->FighterID = a4->FighterSelect;
								if (a4->FighterID == d1) {
									d1 = ( d1 + 1 ) & 7;
								}
								a3->FighterID = d1;
								g.NewPlayers &= ~(1 << a3->Side);
								g.PLSL.PLSLNewPlayers = g.NewPlayers;
								g.NewPlayers = 0;
								a3->Human = TRUE;
								a4->Human = TRUE;
							}
						} else {
							//811e
							if (g.NewPlayers & 1) {
								a3 = PLAYER2;
								a4 = PLAYER1;
								d1 = 0;
							} else {
								a3 = PLAYER1;
								a4 = PLAYER2;
								d1 = 4;
							}
							if (a4->Continuing) {
								a4->Continuing = 0;
								d1 = a4->FighterSelect;
							}
							if (a3->FighterID == d1) {
								d1 += 1;
								d1 &= 7;
							}
							a4->FighterID = d1;
							a4->Human = TRUE;
							g.PLSL.PLSLNewPlayers = g.NewPlayers;
							g.NewPlayers = 0;
						}
					} else {	// if(g.NewPlayers)
						//80e8
						if (g.Player1.Human) {
							g.Player1.FighterID = FID_RYU;
						}
						if (g.Player2.Human) {
							g.Player2.FighterID = FID_KEN;
						}
						if (g.PlayersOnline == 3) {
							give_100_points(0);
							give_100_points(1);
						}
					}
					break;
				case 4:		// 81f0
					NEXT(g.PLSL.mode0);
					g.PLSL.mode1 = 0;
					g.NewChallengerWait = 0;
					start_effect(LC0_LIGHT_ALL_ENABLE, 1);
					break;
				FATALDEFAULT;
			}
			break;
		case 2:
			if (g.PLSL.IntroDelay) {	// Brief delay where players can't select
				--g.PLSL.IntroDelay;
			}
			check_for_new_players();
			timer_tick();
			
			if (g.Player1.Human) {
				g.PLSL_FIDSave = g.Player1.FighterID;
				_check_player(PLAYER1, PLAYER2);
				if (g.PLSL_FIDSave != g.Player1.FighterID) {
					soundsting(SOUND_CURSORMOVE);
				}
			}
			if (g.Player2.Human) {
				g.PLSL_FIDSave = g.Player2.FighterID;
				_check_player(PLAYER2, PLAYER1);
				if (g.PLSL_FIDSave != g.Player2.FighterID) {
					soundsting(SOUND_CURSORMOVE);
				}
			}
			d0 = g.Player1.Human ? g.Player1.SelectComplete : TRUE;
			d1 = g.Player2.Human ? g.Player2.SelectComplete : TRUE;
			if (d0 & d1) {
				NEXT(g.PLSL.mode0);
			}
			proc_actions();
			DSDrawAll_176();		/* a DrawAll() */
			break;
		case 4:
			set_waitmode();
			switch (g.PLSL.mode1) {
				case 0:
					NEXT(g.PLSL.mode1);
					sub_2b7c();
					if (g.OnBonusStage) {
						g.Pause_9e1 = -1;
					} else {
						if (obj = AllocActor()) {
							obj->exists      = TRUE;
							obj->Sel         = SF2ACT_0X2E;			// Plane
							obj->UserData[1] = g.LevelCursor;
							obj->UserData[0] = g.PlayersSelectedDash;
							if (g.PLSL.PLSLNewPlayers) {
								obj->UserData[0] = g.PLSL.x5dc8;
							}
						}
					}
					break;
				case 2:
					if (g.Pause_9e1 < 0) {
						NEXT(g.PLSL.mode1);
						g.PLSL.ExitDelay = 60;		//short
					}
					proc_actions();
					DSDrawAll_176();
					break;
				case 4:
					if (--g.PLSL.ExitDelay == 0) {
						g.WaitMode = 0;
						NEXT(g.PLSL.mode0);
						g.PLSL.mode1 = 0;
					}
					proc_actions();
					DSDrawAll_176();
					break;
				FATALDEFAULT;
			}
			break;
		case 6:
			g.PlayerSelectDone = TRUE;
			if (g.InDemo == 0) {
				// todo some funkyfeatures here @0x8408
				g.Player1.x02ae = g.Player1.Human;
				/* some redundant code removed */
				g.Player2.x02ae = g.Player2.Human;
				fadenwait2();
			}
			break;
		FATALDEFAULT;
	}
	
	
}


