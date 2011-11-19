/*
 *  actions_198a.c  - Group 2 objects
 *  GLUTBasics
 *
 *  Created by Ben on 16/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


#include "sf2types.h"
#include "sf2macros.h"
#include "sf2const.h"

#include "gstate.h"
#include "structs.h"
#include "player.h"

#include "particle.h"

#include "sprite.h"
//#include "main.h"

#include "lib.h"
#include "collision.h"

#include "sound.h"

#include "actions_198a.h"
#include "actions_198a_data.h"

extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;


short g_d7;		/* global for counter */

enum actions_198a {
	BLANKA_FISH,
	KEN_DRUMS,
	RYU_SIGNS,
	/* more todo */
};





static void _SMAct00(Object *obj) {			// 24a50
	/* big fish on Brazil level */
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			setaction_direct(obj, actlist_24cfe);
			break;
		case 2:
			check_rect_queue_draw(obj);
			break;
		case 4:
			clearpush_1174(obj);
			break;
		case 6:
			clearpush_1174(obj);
			break;
		FATALDEFAULT;
	}
	
}
static void _SMKenDrums(Object *obj) {				// 24a74
	
	Object *nobj;
	short i;
	/* kens drums */
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Step = obj->Flip;
			obj->HitBoxes = &data_24eba;
			setaction_direct(obj, actlist_24d54);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					CDCheckDecor(obj);
					if(obj->Energy < 0) { NEXT(obj->mode1) }
					break;
				case 2:
					if (obj->Timer2) {
						obj->Timer2--;
					} else {
						NEXT(obj->mode1);
						for(i=11;i>=0;i--) {
							if (nobj = AllocActor()) {
								nobj->exists = TRUE;
								nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
								nobj->SubSel = data_24c3c[RAND16];
								nobj->Step = obj->Step;
								nobj->ZDepth = obj->ZDepth;
								nobj->XPI = obj->XPI;
								nobj->YPI = obj->YPI + 45;
							}
						}
						if(nobj = AllocActor()) {
							nobj->exists = TRUE;
							nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
							nobj->SubSel = 5;
							nobj->UserByte = 0;
							nobj->Step = obj->Step;
							nobj->ZDepth = obj->ZDepth;
							nobj->XPI = obj->XPI - 16;
							nobj->YPI = obj->YPI + 40;
						}
						if(nobj = AllocActor()) {
							nobj->exists = TRUE;
							nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
							nobj->SubSel = 6;
							nobj->UserByte = 0;
							nobj->Step = obj->Step;
							nobj->ZDepth = obj->ZDepth;
							nobj->XPI = obj->XPI - 16;
						}
						if(nobj = AllocActor()) {
							nobj->exists = TRUE;
							nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
							nobj->SubSel = 5;
							nobj->UserByte = 1;
							nobj->Step = obj->Step;
							nobj->ZDepth = obj->ZDepth;
							nobj->XPI = obj->XPI - 0x30;
							nobj->YPI = obj->YPI + 0x38;
						}
						if(nobj = AllocActor()) {
							nobj->exists = TRUE;
							nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
							nobj->SubSel = 6;
							nobj->UserByte = 1;
							nobj->Step = obj->Step;
							nobj->ZDepth = obj->ZDepth;
							nobj->XPI = obj->XPI - 0x30;
							nobj->YPI = obj->YPI + 16;
						}
						setaction_direct(obj, actlist_24d70);
					}
					break;
				FATALDEFAULT;
			}
			check_onscreen_queue(obj);
			break;
		case 4:
			clearpush_1174(obj);
			break;
		case 6:
			clearpush_1174(obj);
			break;
		FATALDEFAULT;
	}
	
}
static void _SMRyuSigns(Object *obj) {		// 24c4e
	Object *nobj;
	short i;
	/* Ryu stage signs */
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Step = obj->Flip;
//XXX			obj->HitBoxes = data_24eda;
			setaction_direct(obj, actlist_24e1a);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					CDCheckDecor(obj);
					if(obj->Energy < 0) { NEXT(obj->mode1) }
					break;
				case 2:
					if (obj->Timer2) {
						obj->Timer2--;
					} else {
						NEXT(obj->mode1);
						for(i=3;i>=0;i--) {
							if (nobj = AllocActor()) {
								nobj->exists = TRUE;
								nobj->Sel = SF2ACT_0X13;
								nobj->SubSel = i;
								nobj->X = obj->X;
								nobj->Y = obj->Y;
							}
						}
						setaction_direct(obj, actlist_24e36);
					}
					break;
				FATALDEFAULT;
			}
			check_rect_queue_draw(obj);
		case 4:
			clearpush_1174(obj);
			break;
		FATALDEFAULT;
	}
}


static void _SMAct03(Object *obj) {		// 24efa
	/* todo */
}
static void _SMAct04(Object *obj) {		// 27ea2 ID4 BONUS3
	/* todo */
}

void _SMAct05(Object *obj) {				// 24ff6
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->mode2 = 0;
			d1 = sub_24b52(obj);
			// redundant btst
			obj->HitBoxes = hitboxlist_25958;
			obj->Pool = 2;
			obj->Scroll = 0;
			obj->Step = obj->Flip;
			setaction_direct(obj, action_);
			// userdata ...
			
			
			
			break;
		case 2:
			// todo
			break;
		default:
			break;
			
			
	}
}


#pragma mark 06-The Car		

static int _car_check_P1(Object_G2 *obj){			// 25d94
	if (obj->UD.UDcar.p1.as > 0 && obj->UD.UDcar.p1.bs != obj->UD.UDcar.p1.as) {
		obj->UD.UDcar.p1.bs = obj->UD.UDcar.p1.as;
		return TRUE;
	}
	return FALSE;
}
static int _car_check_P2(Object_G2 *obj){			// 25da8
	if (obj->UD.UDcar.p2.as > 0 && obj->UD.UDcar.p2.bs != obj->UD.UDcar.p2.as) {
		obj->UD.UDcar.p2.bs = obj->UD.UDcar.p2.as;
		return TRUE;
	}
	return FALSE;
}
static void car_setaction(Object_G2 *obj) {//25dbc
	// XXX setaction_list(obj, data____, obj->UD.UDcar.h0091c + obj->UD.UDcar.h0092c);
}	
static void _car_disable_ply_shadow(Player *ply) {				// 26122
	if (ply->YPI != 40) {
		if (ply->Side) {
			g.Ply2Shadow.flag1 = FALSE;
		} else {
			g.Ply1Shadow.flag1 = FALSE;
		}
	}
}
static void _car_init_as_bs(CarPlayer *cp) {			// 25dcc
	cp->as = cp->bs = (char []){3,5,4,5,3,4,5,4,4,4,5,3,4,3,5,3}[sf2rand() & 15];
}
static int _check_platform(Player *plya3, Object_G2 *obj) {		//26096
	int d2;
	
	plya3->PlatformFallDir = 0;
	if (2*(0x48 + plya3->Size) <= (plya3->XPI - obj->XPI) + 0x48 + plya3->Size) {
		if (plya3->XPI - obj->XPI < 0) {
			d2 = (plya3->XPI - obj->XPI) + (plya3->Size + 0x48);
		} else {
			d2 = (plya3->XPI - obj->XPI) - (plya3->Size + 0x48);
		}
		if (ABS(d2)) {
			_car_disable_ply_shadow(plya3);
		}
		if ((obj->YPI + 48) - plya3->YPI >= 0) {
			if ( (ABS(d2) <= (obj->YPI + 48) - plya3->YPI) &&
				((obj->YPI + 48) - plya3->YPI <= 0x20)   &&
				plya3->DeltaY.full == 0 ) {
				plya3->YPI = obj->YPI + 0x30;
				plya3->OnPlatform = TRUE;
				return 1;
			} else {
				plya3->XPI += d2;
				if (d2 > 0) {
					plya3->PlatformFallDir = 1;
				} else if (d2 < 0) {
					plya3->PlatformFallDir = 2;
				}
			}
		}
	} 
	plya3->OnPlatform = FALSE;
	return FALSE;
}
static void _people_on_roof(Object_G2 *obj) {		// 25df2
	short x = g.CarOffX;
	short y = g.CarOffY;
	Object *newobj;
	
	obj->XPI += x;
	obj->YPI -= y;
	g.CarOffX = 0;
	g.CarOffY = 0;
	if (PLAYER1->exists && PLAYER1->Jumping == 0) {
		PLAYER1->XPI += x;
		PLAYER1->YPI -= y;
	}
	if (PLAYER2->exists && PLAYER2->Jumping == 0 && (obj->UD.UDcar.h0090c & 2)) {
		PLAYER2->XPI += x;
		PLAYER2->YPI -= y;
	}
	switch (g.ActiveHumans) {
		case 0:
			// can't happen
			break;
		case 1:
			if (obj->UD.UDcar.h0090c) {
				if (_check_platform(PLAYER1, obj) == FALSE) {
					obj->UD.UDcar.h0090c = 0;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 5;
						
					}
				}
			} else {
				if (_check_platform(PLAYER1, obj) != FALSE) {
					obj->UD.UDcar.h0090c = 1;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 2;
						
					}
				}
			}

			break;
		case 2:
			if (obj->UD.UDcar.h0090c) {
				if (_check_platform(PLAYER1, obj) == FALSE) {
					obj->UD.UDcar.h0090c = 0;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 5;
						
					}
				}
			} else {
				if (_check_platform(PLAYER1, obj) != FALSE) {
					obj->UD.UDcar.h0090c = 2;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 2;
						
					}
				}
			}
			
			break;
		case 3:
			switch (obj->UD.UDcar.h0090c) {
				case 0:
					/* todo 25f06 */
					break;
				default:
					break;
			}
			break;
		FATALDEFAULT;
	}
}
static void _ActSMCar(Object_G2 *obj) {			// The CarID 6, BONUS0
	/* userdata: */
	switch (obj->mode0) {
		case 0:
			memclear(&obj->UD.UDcar, sizeof (UDcar));
			g.x8abe = FALSE;
			// XXX obj->HitBoxes = data_25d6c;
			_car_init_as_bs(&obj->UD.UDcar.p1);
			_car_init_as_bs(&obj->UD.UDcar.p2);
			car_setaction(obj);
			break;
		case 2:
			_people_on_roof(obj);
			sub_7d99a(obj);			// collision detection			
			
			if (!obj->mode1) {
				if (obj->mode2) {
					switch (obj->mode3) {				//25a10
						case 0:							//25a22
							if (_car_check_P1(obj) < 0) {
								NEXT(obj->mode3);
								obj->UD.UDcar.h0091c++;
								_car_init_as_bs(&obj->UD.UDcar.p1);
							}
							if (_car_check_P2(obj) < 0) {
								NEXT(obj->mode2);
								obj->UD.UDcar.h0092c = 3;
							}
							break;
						case 2:							//25a4a
							if (_car_check_P1(obj) < 0) {
								NEXT(obj->mode3);
								++obj->UD.UDcar.h0091c;
							}
							if (_car_check_P2(obj) < 0) {
								NEXT(obj->mode2);
								obj->UD.UDcar.h0092c = 3;
							}
							break;
						case 4:							//25a6a
							if (_car_check_P2(obj) < 0) {
								NEXT(obj->mode2);
								obj->UD.UDcar.h0092c = 3;
								_car_init_as_bs(&obj->UD.UDcar.p1);
								_car_init_as_bs(&obj->UD.UDcar.p2);
							}
							break;
						FATALDEFAULT;
					}
				} else {
					switch (obj->mode3) {
						case 0:
							if (_car_check_P1(obj) < 0) {
								NEXT(obj->mode3);
								++obj->UD.UDcar.h0091c;
							}
							_car_init_as_bs(&obj->UD.UDcar.p1);
							break;
						case 2:
							if (_car_check_P1(obj) < 0) {
								NEXT(obj->mode3);
								++obj->UD.UDcar.h0091c;
								_car_init_as_bs(&obj->UD.UDcar.p1);
								_car_init_as_bs(&obj->UD.UDcar.p2);
							}
							break;
						case 4:
							if (_car_check_P1(obj) < 0) {
								NEXT(obj->mode1);
								obj->mode2 = 0;
								obj->mode3 = 0;
								obj->UD.UDcar.h0092c = 0;
								obj->UD.UDcar.h0091c = 6;
								_car_init_as_bs(&obj->UD.UDcar.p1);
							}
							break;
						FATALDEFAULT;
					}
				}

			} 
			else {
				// 25b02
				switch (obj->mode2) {
					case 0-16:							//25b26;
						if(_car_check_P1(obj) < 0) {
							NEXT(obj->mode2);
							if(++obj->UD.UDcar.h0091c == 15) {
								g.x8abe = TRUE;
							}
							_car_init_as_bs(&obj->UD.UDcar.p1);
							car_setaction(obj);
						}
						break;
					case 18:							//25b50;
						if(_car_check_P1(obj) < 0) {
							NEXT(obj->mode2);
							gstate_Scroll1.XPI += 256;
							obj->YPI -= 24;
							ActStartScreenWobble(obj);
							queuesound(0x2a);
							g.BonusComplete = TRUE;
							g.CarWasted     = TRUE;
						}
						
						break;
					case 20-22:
						/* nothing */
						break;
					FATALDEFAULT;
				}
			}
			check_rect_queue_draw((Object *)obj);
			break;
		case 4:
			NEXT(obj->mode0);
			break;
		case 6:
			clearpush_1174((Object *)obj);
			break;
		FATALDEFAULT;
	}
}
static void sub_25f06(Object_G2 *obj) {		// players on top of car
	Object *nobj;
	switch (obj->UD.UDcar.h0090c) {
		case 0:		// no players on me
			if (_check_platform(&g.Player1, obj)) {
				obj->UD.UDcar.h0090c = 1;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x2;
				}
			}
			else if (_check_platform(&g.Player2, obj)) {
				obj->UD.UDcar.h0090c = 2;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x2;
				}
			}
			break;
		case 1:		// player1 is on me
			if (_check_platform(&g.Player2, obj)) {
				obj->UD.UDcar.h0090c = 3;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x3;
				}
			} else if (!_check_platform(&g.Player1, obj)) {
				obj->UD.UDcar.h0090c = 0;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x5;
				}
			}				
			break;
		case 2:		// player2 is on me
			if (_check_platform(&g.Player1, obj)) {
				obj->UD.UDcar.h0090c = 3;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x3;
				}
			}
			else if (!_check_platform(&g.Player2, obj)) {
				obj->UD.UDcar.h0090c = 0;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x5;
				}
			}
			break;
		case 3:		// both players are on me, wait for both to come off;
			//todo
			break;
		FATALDEFAULT;
	}
}



#pragma mark Act07
static void sub_272c6(Object *obj) {/* todo */}

#pragma mark Act08

static int _Act08PlatformCheck(Player *plya3, Object *obj) {		// 277d4
	short d2;
	plya3->PlatformFallDir = 0;
	
	if (2*(0x48 + plya3->Size) <= (plya3->XPI - obj->XPI) + 0x48 + plya3->Size) {
		if (plya3->Side) {
			g.Ply2Shadow.exists = FALSE;
		} else {
			g.Ply1Shadow.exists = FALSE;
		}
		
		if (plya3->XPI - obj->XPI < 0) {
			d2 = (plya3->XPI - obj->XPI) + (plya3->Size + 0x48);
		} else {
			d2 = (plya3->XPI - obj->XPI) - (plya3->Size + 0x48);
		}
		if (obj->YPI >= plya3->YPI) {
			if (obj->YPI - plya3->YPI < 0x20 && plya3->DeltaY.full == 0) {
				plya3->YPI = obj->YPI;
				plya3->OnPlatform = TRUE;
				return TRUE;
			}
			plya3->XPI += d2;
			if (d2 > 0) {
				plya3->PlatformFallDir = 1;
			} else if (d2 < 0) {
				plya3->PlatformFallDir = 2;
			}			
		}
	} 
	plya3->OnPlatform = FALSE;
	return FALSE;
}
static void _SMAct08(Object *obj) {		// 274e4
	Object *nobj;
	short tmpx, tmpy;
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			ud->h0080c = 0;
			if (nobj = AllocActor()) {
				nobj->exists = TRUE;
				nobj->Sel = 6;
				nobj->XPI = 192;
				nobj->YPI = 48;
				obj->Owner = nobj;
			}
			break;
		case 2:
			tmpx = g.CarOffX;
			tmpy = g.CarOffY;
			g.CarOffX = 0;
			g.CarOffY = 0;
			nobj = obj->Owner;
			obj->YPI -= tmpy;
			nobj->XPI += tmpx;
			nobj->YPI -= tmpy;
			if (g.Player1.exists && !g.Player1.Jumping) {
				g.Player1.XPI += tmpx;
				g.Player1.YPI -= tmpy;
			}
			if (g.Player2.exists && !g.Player2.Jumping) {
				g.Player2.XPI += tmpx;
				g.Player2.YPI -= tmpy;
			}
			switch (g.ActiveHumans) {
				case 0:
					/* nothing, can't happen */
					break;
				case 1:
					if (ud->h0080c) {
						if (_Act08PlatformCheck(&g.Player1, obj)) {
							ud->h0080c = 1;
							if (nobj = AllocActor()) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x2;
							}
						}
					} else {
						if(_Act08PlatformCheck(&g.Player1, obj) == 0) {
							ud->h0080c = 0;
							if (nobj = AllocActor()) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x5;
							}
						}
					}
					break;
				case 2:
					if (ud->h0080c) {
						if (_Act08PlatformCheck(&g.Player2, obj)) {
							ud->h0080c = 2;
							if (nobj = AllocActor()) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x2;
							}
						}
					} else {
						if(_Act08PlatformCheck(&g.Player2, obj) == 0) {
							ud->h0080c = 0;
							if (nobj = AllocActor()) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x5;
							}
						}
					}
					break;
				case 3:		// both of them
					switch (ud->h0080c) {
						case 0:
							if (_Act08PlatformCheck(&g.Player1, obj)) {
								ud->h0080c = 1;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x2;
								}
							}
							if (_Act08PlatformCheck(&g.Player2, obj)) {
								ud->h0080c = 2;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x2;
								}
							}
							break;
						case 1:
							if (_Act08PlatformCheck(&g.Player2, obj)) {
								ud->h0080c = 3;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x3;
								}								
							} else if (_Act08PlatformCheck(&g.Player1, obj)==0) {
								ud->h0080c = 0;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x5;
								}
							}
							break;
						case 2:
							if (_Act08PlatformCheck(&g.Player1, obj)) {
								ud->h0080c = 3;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x3;
								}								
							} else if (_Act08PlatformCheck(&g.Player2, obj)==0) {
								ud->h0080c = 0;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x5;
								}
							}
							break;
						case 3:
							if (_Act08PlatformCheck(&g.Player1, obj) == 0 &&
								_Act08PlatformCheck(&g.Player2, obj) == 0) {
								ud->h0080c = 0;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x5;
								}
							} else if (_Act08PlatformCheck(&g.Player2, obj) == 0) {
								ud->h0080c = 1;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x4;
								}
							} else if (_Act08PlatformCheck(&g.Player1, obj) == 0) {
								ud->h0080c = 2;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x4;
								}
								
							}
							break;
						FATALDEFAULT;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 4:
		case 6:
			clearpush_1174(obj);
			break;
		FATALDEFAULT;
	}
}


static void sub_921e(void) {
	if (g.x8ab9 <= 5) {
		sub_90c8();
	}
}
static void sub_922c(void) {
	sub_bcd_8(1, &g.x8ab9);
	sub_528a();
	if (g.x8ab9 == 0) {
		g.TimeOut           = TRUE;
		g.NewChallengerWait = TRUE;
	}
}
static void sub_91c8(void) {
	int tmp = 0;
	if (g.RoundResult) {
		wait_for_ply_PSFinishedParticipating();
	} else if (g.TimeOut) {
		add_bcd_8(g.x8a60[0], &tmp);
		add_bcd_8(g.x8a60[1], &tmp);
		if (tmp == 0x20) {
			g.BonusComplete = TRUE;
			print_libtextgfx(0x1d);
		}
		if (g.Player1.BonusScore == g.Player2.BonusScore) {
			g.RoundResult = -1;
		} else if (g.Player1.BonusScore > g.Player2.BonusScore) {
			g.RoundResult = 1;
		} else {
			g.RoundResult = 2;
		}
		print_bonusremaining();
	} else if (g.x8ab9 <= 5) {
		sub_90c8();
	}
}

static void sub_27862(Object *obj) {/* todo */}

static void sub_2581a(Object *obj) {
	if (((obj->YPI & 0xc0) >> 5) != 6) {
		obj->Pool = (obj->YPI >> 5);
	} else {
		obj->Pool = 4;
	}
}



void sub_24f0e(Object *obj) {
	NEXT(obj->mode0);
	setaction_direct(obj, action_24f52);
}

void sub_24f22(Object *obj) {
	Object *nobj;
	
	CDCheckDecor(obj);
	if (obj->Energy < 0) {
		NEXT(obj->mode0);
		if (nobj = AllocActor()) {
			nobj->exists = TRUE;
			nobj->Sel = 0x18;
			nobj->SubSel = obj->SubSel;
		}
	}
	check_rect_queue_draw(obj);
}

// Subsel    BonusX
//  4			3
//  5			2
//	6			0
//  9           1


void actions_198a(void) {			/* 249fa */
	short i;
	
	debughook(1);
	g_d7=15;
	for(i=0; i<16; i++) {
		if(g.Objects2[i].exists == 0) {break;}
		switch (g.Objects2[i].Sel) {
			case BLANKA_FISH:
				_SMAct00(&g.Objects2[i]);
				break;
			case KEN_DRUMS:
				_SMKenDrums(&g.Objects2[i]);
				break;
			case RYU_SIGNS:
				_SMRyuSigns(&g.Objects2[i]);
				break;
			case 3:
				_SMAct03(&g.Objects2[i]);
				break;
			case 4:						// BONUS3 Collisions
				_SMAct04(&g.Objects2[i]);
				break;
			case 5:						// BONUS2 Collisions
				_SMAct05(&g.Objects2[i]);
				break;
			case 6:		// the Car		// BONUS0 Collisions
				_ActSMCar(&g.Objects2[i]);
				break;
			case 7:
				sub_272c6(&g.Objects2[i]);
				break;
			case 8:
				_SMAct08(&g.Objects2[i]);
				break;
			case 9:						// BONUS1 Collisions
				sub_27862(&g.Objects2[i]);
				break;
			default:
				printf("action 198a id %d not implemented\n", g.Objects2[i].Sel);
				break;
		}
		g_d7--;
	}
}

