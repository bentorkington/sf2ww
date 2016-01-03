/*
 *  actions_198a.c  - Group 2 objects
 *  GLUTBasics
 *
 *  Created by Ben on 16/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


#include "sf2.h"

#include "gstate.h"
#include "structs.h"
#include "player.h"

#include "particle.h"

#include "sprite.h"

#include "lib.h"
#include "fightgfx.h"
#include "gfxlib.h"
#include "collision.h"
#include "coll_bonus.h"

#include "sound.h"

#include "actions_198a.h"
#include "actions_198a_data.h"

extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;


short g_d7;		/* global for counter */



static void sub_25f06(Object_G2 *obj);



static void _SMAct00(Object_G2 *obj) {			// 24a50
	/* big fish on Brazil level */
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
            RHSetAction((Object *)obj, RHCODE(0x24cfe));
			break;
		case 2:
			check_rect_queue_draw((Object *)obj);
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
static void _SMKenDrums(Object_G2 *obj) {				// 24a74
	
	Object *nobj;
	short i;
	/* kens drums */
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Step = obj->Flip;
			obj->HitBoxes = &data_24eba;
            RHSetAction((Object *)obj, RHCODE(0x24d54));
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
							if ((nobj = AllocActor())) {
								nobj->exists = TRUE;
								nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
								nobj->SubSel = data_24c3c[RAND16];
								nobj->Step = obj->Step;
								nobj->ZDepth = obj->ZDepth;
								nobj->XPI = obj->XPI;
								nobj->YPI = obj->YPI + 45;
							}
						}
						if((nobj = AllocActor())) {
							nobj->exists = TRUE;
							nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
							nobj->SubSel = 5;
							nobj->UserByte = 0;
							nobj->Step = obj->Step;
							nobj->ZDepth = obj->ZDepth;
							nobj->XPI = obj->XPI - 16;
							nobj->YPI = obj->YPI + 40;
						}
						if((nobj = AllocActor())) {
							nobj->exists = TRUE;
							nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
							nobj->SubSel = 6;
							nobj->UserByte = 0;
							nobj->Step = obj->Step;
							nobj->ZDepth = obj->ZDepth;
							nobj->XPI = obj->XPI - 16;
						}
						if((nobj = AllocActor())) {
							nobj->exists = TRUE;
							nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
							nobj->SubSel = 5;
							nobj->UserByte = 1;
							nobj->Step = obj->Step;
							nobj->ZDepth = obj->ZDepth;
							nobj->XPI = obj->XPI - 0x30;
							nobj->YPI = obj->YPI + 0x38;
						}
						if((nobj = AllocActor())) {
							nobj->exists = TRUE;
							nobj->Sel = SF2ACT_USAKEN_DRUMFRAGS;
							nobj->SubSel = 6;
							nobj->UserByte = 1;
							nobj->Step = obj->Step;
							nobj->ZDepth = obj->ZDepth;
							nobj->XPI = obj->XPI - 0x30;
							nobj->YPI = obj->YPI + 16;
						}
                        RHSetAction((Object *)obj, RHCODE(0x24d70));
					}
					break;
				FATALDEFAULT;
			}
			check_onscreen_queue((Object *)obj);
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
static void _SMRyuSigns(Object_G2 *obj) {		// 24c4e
	Object *nobj;
	short i;
	/* Ryu stage signs */
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Step = obj->Flip;
			obj->HitBoxes = &hitboxes_24eda;
            RHSetAction((Object *)obj, RHCODE(0x24e1a));
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
							if ((nobj = AllocActor())) {
								nobj->exists = TRUE;
								nobj->Sel = SF2ACT_0X13;
								nobj->SubSel = i;
								nobj->X = obj->X;
								nobj->Y = obj->Y;
							}
						}
                        RHSetAction((Object *)obj, RHCODE(0x24e36));
					}
					break;
				FATALDEFAULT;
			}
			check_rect_queue_draw((Object *)obj);
            break;
		case 4:
			clearpush_1174(obj);
			break;
		FATALDEFAULT;
	}
}

static void sub_24fcc(Object *obja0, Object *obja6) {
	obja0->XPI = obja6->XPI;
	obja0->YPI = obja6->YPI + 64;
}


static void sub_24fe0(Object_G2 *obj) {
//	if (obj->UD.UDunknown.h0097c) {
//		obj->UD.UDunknown.h0098p->mode0 = 4;
//		obj->UD.UDunknown.h0097 = FALSE;
//	}
}


static void sub_24f96(int argd0, Object *obj) {
	
}

const HitBox hitb_24f8e[] = {EMPTY_HITBOX, {0, 80, 16, 80}};

const struct hitboxes hitboxes_24f76 = {
	hitb_null,
	hitb_null,
	hitb_null,
	hitb_null,
	hitbact_null,
	hitb_24f8e,
};

static void _SMAct03(Object_G2 *obj) {		// 24efa
	Object *nobj;
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->HitBoxes = &hitboxes_24f76;
            RHSetAction((Object *)obj, RHCODE(0x24f52));
			break;
		case 2:
			CDCheckDecor(obj);
			if (obj->Energy < 0) {
				NEXT(obj->mode0);
				if ((nobj = AllocActor())) {
					nobj->exists = TRUE;
					nobj->Sel = 0x18;
					nobj->SubSel = obj->SubSel;
				}
				check_rect_queue_draw((Object *)obj);
			}
			break;
		case 4:
		case 6:
			clearpush_1174(obj);
			break;
		FATALDEFAULT;
	}
}


//in drums.c
void _SMAct04(Object_G2 *obj);
void _SMAct05(Object_G2 *obj);
#pragma mark 06-The Car		
// in car.c
void _ActSMCar(Object_G2 *obj);

#pragma mark Act07 Guiles's Crate
static void _SMAct07(Object_G2 *obj) {			// 272c6
	static const char data_27384[] = {
		0, 4, 2, 3, 0, 1, 3, 2, 1, 4, 3, 2, 0, 1, 4, 2
	};
	int i;
	Object *nobj;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Flip = obj->Step;
			obj->Energy = 0;
			obj->HitBoxes = &hitboxes_274c4;
			obj->Pool = 0;
            RHSetAction((Object *)obj, RHCODE(0x273a2));
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					CDCheckDecor(obj);
					if (obj->Energy < 0) {
						NEXT(obj->mode1);
					}
					break;
				case 2:
					if (--obj->Timer2 == 0) {
						NEXT(obj->mode1);
						for (i=0x13; i>=0; --i) {
							if ((nobj=AllocActor())) {
								nobj->exists = TRUE;
								nobj->Sel = 0x1c;
								nobj->SubSel = data_27384[RAND16];
								nobj->ZDepth = obj->ZDepth;
								nobj->XPI = obj->XPI;
								nobj->YPI = obj->YPI + 0x2d;
							}
						}
                        RHSetAction((Object *)obj, RHCODE(0x27404));
					}
					break;
				case 4:
					actiontick((Object *)obj);
					break;
				FATALDEFAULT;
			}
			check_rect_queue_draw((Object *)obj);
			break;
		case 4:
		case 6:
			clearpush_1174(obj);
		FATALDEFAULT;
	}
}

#pragma mark Act08

static int _Act08PlatformCheck(Player *plya3, Object_G2 *obj) {		// 277d4
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
static void _SMAct08(Object_G2 *obj) {		// 274e4
	Object *nobj;
	short tmpx, tmpy;
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->UD.UDactB08.h0080c = 0;
			if ((nobj = AllocActor())) {
				nobj->exists = TRUE;
				nobj->Sel = 6;
				nobj->XPI = (SCREEN_WIDTH / 2);
				nobj->YPI = 48;
				obj->Owner = (Player *)nobj;
			}
			break;
		case 2:
			tmpx = g.CarOffX;
			tmpy = g.CarOffY;
			g.CarOffX = 0;
			g.CarOffY = 0;
			// XXX think we forgot AllocActor here?
			nobj = (Object *)obj->Owner;
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
					if (obj->UD.UDactB08.h0080c) {
						if (_Act08PlatformCheck(&g.Player1, obj)) {
							obj->UD.UDactB08.h0080c = 1;
							if ((nobj = AllocActor())) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x2;
							}
						}
					} else {
						if(_Act08PlatformCheck(&g.Player1, obj) == 0) {
							obj->UD.UDactB08.h0080c = 0;
							if ((nobj = AllocActor())) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x5;
							}
						}
					}
					break;
				case 2:
					if (obj->UD.UDactB08.h0080c) {
						if (_Act08PlatformCheck(&g.Player2, obj)) {
							obj->UD.UDactB08.h0080c = 2;
							if ((nobj = AllocActor())) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x2;
							}
						}
					} else {
						if(_Act08PlatformCheck(&g.Player2, obj) == 0) {
							obj->UD.UDactB08.h0080c = 0;
							if ((nobj = AllocActor())) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x5;
							}
						}
					}
					break;
				case 3:		// both of them
					switch (obj->UD.UDactB08.h0080c) {
						case 0:
							if (_Act08PlatformCheck(&g.Player1, obj)) {
								obj->UD.UDactB08.h0080c = 1;
								if ((nobj = AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x2;
								}
							}
							if (_Act08PlatformCheck(&g.Player2, obj)) {
								obj->UD.UDactB08.h0080c = 2;
								if ((nobj = AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x2;
								}
							}
							break;
						case 1:
							if (_Act08PlatformCheck(&g.Player2, obj)) {
								obj->UD.UDactB08.h0080c = 3;
								if ((nobj = AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x3;
								}								
							} else if (_Act08PlatformCheck(&g.Player1, obj)==0) {
								obj->UD.UDactB08.h0080c = 0;
								if ((nobj = AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x5;
								}
							}
							break;
						case 2:
							if (_Act08PlatformCheck(&g.Player1, obj)) {
								obj->UD.UDactB08.h0080c = 3;
								if ((nobj = AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x3;
								}								
							} else if (_Act08PlatformCheck(&g.Player2, obj)==0) {
								obj->UD.UDactB08.h0080c = 0;
								if ((nobj = AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x5;
								}
							}
							break;
						case 3:
							if (_Act08PlatformCheck(&g.Player1, obj) == 0 &&
								_Act08PlatformCheck(&g.Player2, obj) == 0) {
								obj->UD.UDactB08.h0080c = 0;
								if ((nobj = AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x5;
								}
							} else if (_Act08PlatformCheck(&g.Player2, obj) == 0) {
								obj->UD.UDactB08.h0080c = 1;
								if ((nobj = AllocActor())) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x4;
								}
							} else if (_Act08PlatformCheck(&g.Player1, obj) == 0) {
								obj->UD.UDactB08.h0080c = 2;
								if ((nobj = AllocActor())) {
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
void sub_91c8(void) {
	unsigned char tmp = 0;
	if (g.RoundResult) {
		wait_for_ply_PSFinishedParticipating();
	} else if (g.TimeOut) {
		add_bcd_8(g.x8a60[0], &tmp);
		add_bcd_8(g.x8a60[1], &tmp);
		if (tmp == 0x20) {
			g.BonusComplete = TRUE;
			DrawTileText(TILETEXT_PERFECT);
		}
		if (g.Player1.BonusScore == g.Player2.BonusScore) {
			g.RoundResult = ROUNDRESULT_DRAW;
		} else if (g.Player1.BonusScore > g.Player2.BonusScore) {
			g.RoundResult = ROUNDRESULT_P1_WINS;
		} else {
			g.RoundResult = ROUNDRESULT_P2_WINS;
		}
		print_bonusremaining();
	} else if (g.x8ab9 <= 5) {
		sub_90c8();
	}
}

static void sub_27bda(Object_G2 *obj) {
	Object *nobj;
	int i;
	if ((nobj = AllocActor())) {
		nobj->exists = TRUE;
		nobj->Sel = 0x3d;
		nobj->XPI = obj->XPI;
		nobj->YPI = obj->YPI;
		nobj->SubSel = 1;
		nobj->UserByte = 0;
		nobj->Step = obj->Flip;
		nobj->Flip = obj->Flip;
	}
	for(i=5; i>= 0; --i) {
		if ((nobj = AllocActor())) {
			nobj->exists = TRUE;
			nobj->Sel = 0x3d;
			nobj->XPI = obj->XPI;
			nobj->YPI = obj->YPI;
			nobj->SubSel = 1;
			nobj->UserByte = 1;
			nobj->Step = obj->Flip;
			nobj->Flip = obj->Flip;
		}
	}
	NEXT(obj->mode0);
}

static void sub_27912(Object_G2 *obj) {
	NEXT(obj->mode0);
	obj->Energy = 0;
	obj->EnergyDash = 0;
	obj->Flip = obj->Direction;
	obj->Step = obj->Direction;
	check_rect_queue_draw((Object *)obj);
}

#pragma mark Act09 Bonus1 Barrels

static int _barrel_gravity(Object_G2 *obj) {				// 27b4c
	obj->Y.full += obj->UD.UDbonus1.Velocity.full;
	obj->UD.UDbonus1.Velocity.full += obj->UD.UDbonus1.Accel.full;
	return obj->UD.UDbonus1.Velocity.full;
}

static int ACTB09_ply_distance(Player *ply, Object_G2 *obj) {		// 27b96
	int tmp;
	tmp = ABS(ply->XPI - obj->XPI) - 18;
	if (tmp < 0) {
		tmp = 0;
	}
	return tmp;
}
	
static void sub_27b5e(Object_G2 *obj) {			// 27b5e
	if (obj->mode0 == 2) {
		obj->UD.UDbonus1.h0098s = 0x0100;
		obj->UD.UDbonus1.h009as = 0x0100;
	} else {
		if (g.Player1.exists) {
			obj->UD.UDbonus1.h0098s = ACTB09_ply_distance(PLAYER1, obj);
		}
		if (g.Player2.exists) {
			obj->UD.UDbonus1.h009as = ACTB09_ply_distance(PLAYER2, obj);
		}
	}
}

static void _init_barrel_hitpoints(Object_G2 *obj) {     // 278ac
    const static char data_278c2[] = {
        0, 1, 2, 0, 1, 2, 0, 3, 0, 1, 2, 0, 1, 2, 0, 0
    };

    obj->Energy     = data_278c2[RAND16WD];
    obj->EnergyDash = obj->Energy;
}

static void sub_27862(Object_G2 *obj) {			// 27862 Act09 BONUS1
	int temp;
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			// todo init first three ints;
			obj->UD.UDbonus1.h008cc = 0;
			obj->UD.UDbonus1.h008dc = 0;
			obj->UD.UDbonus1.h008ec = 0;
			obj->UD.UDbonus1.h008fc = 0;
			obj->UD.UDbonus1.h0098s = 0x0100;
			obj->UD.UDbonus1.h009as = 0x0100;
			obj->HitBoxes = &hitboxes_27e6a;
			obj->Pool = 0;
			obj->Flip = 0;
            _init_barrel_hitpoints(obj);
			break;
		case 2:
			CDCheckDecor(obj);
			CDBonusCollisionCheck(obj);
			if (obj->Energy < 0) {
				sub_27912(obj);
				return;
			} else if (obj->Energy != obj->EnergyDash) {
				// 2793e
				obj->EnergyDash = obj->Energy;
				obj->UD.UDbonus1.Accel.full = 0xffffc000;	

                if (obj->Direction) {
					obj->UD.UDbonus1.H009c.part.integer = 2;
                    obj->Flip = 0;
				} else {
					obj->UD.UDbonus1.H009c.part.integer = -2;
                    obj->Flip = 1;
				}
                
				if (obj->mode1 >= 8) {
					//2798a
					obj->mode1 = 8;
					obj->UD.UDbonus1.H009c.full = 0x00030000;
				} else {
					obj->mode1 = 6;
					obj->mode2 = 2;
					obj->UD.UDbonus1.H009c.full = 0x00040000;
				}
				actiontick((Object *)obj);
				check_rect_queue_draw((Object *)obj);
			} else {
				sub_27b5e(obj);
				switch (obj->mode1) {
					case 0:			// 279a0
						if (obj->x002e != 0) {      // wait unti we'e told to start
							obj->YPI = 0x00d6;
							if (g.x8ab8 & 1) {
								obj->XPI = 0x180;
								obj->SubSel = 0;
								obj->UD.UDbonus1.H009c.part.integer = -2;
								obj->Flip   = 1;
							} else {
								obj->XPI    = 0;
								obj->SubSel = 2;
								obj->UD.UDbonus1.H009c.part.integer = 2;
								obj->Flip = 0;
							}
							NEXT(obj->mode1);
							_init_barrel_hitpoints(obj);
							obj->Pool = 0;
                            RHSetActionList((Object *)obj, RHCODE(0x27c54), 0);
							check_rect_queue_draw((Object *)obj);
						}
						break;
					case 2:					// 27a00
						obj->XPI += obj->UD.UDbonus1.H009c.part.integer;
						if (obj->XPI >= 0xa0 && obj->XPI <0xe0 ) {      // fall through the gap
							NEXT(obj->mode1);
							obj->UD.UDbonus1.Velocity.full = 0;
							obj->UD.UDbonus1.Accel.full = 0xffffc000;
						}
						actiontick((Object *)obj);
						check_rect_queue_draw((Object *)obj);
						break;
					case 4:					// 27a34
						obj->XPI += obj->UD.UDbonus1.H009c.part.integer;
						_barrel_gravity(obj);
						if (obj->YPI < 0xa0 && obj->YPI >= 0) {
							NEXT(obj->mode1);       // bounce off the silly trampoline
							g.x8a64[0] = 1;
							obj->UD.UDbonus1.Velocity.full = 0x0004c000;
							temp = (sf2rand() & 1) + obj->SubSel;
							obj->UD.UDbonus1.H009c.full = (u32[]){0xfffe8000,0xffff8000,0x00008000,0x00018000}[temp];
						}
						actiontick((Object *)obj);
						check_rect_queue_draw((Object *)obj);
						break;
					case 6:										// 27a8a
						if(obj->mode2) {
							obj->X.full += obj->UD.UDbonus1.H009c.full;
							if(_barrel_gravity(obj)<0 && obj->YPI < 40) {
								NEXT(obj->mode1);
								obj->mode2 = 0;
								if (sf2rand() & 4) {
									obj->UD.UDbonus1.H009c.full = 0x00020000;
									obj->Flip = 0;
								} else {
									obj->UD.UDbonus1.H009c.full = 0xfffe0000;
									obj->Flip = 1;
								}
								obj->UD.UDbonus1.Velocity.full = 0x00028000;
							}
							actiontick((Object *)obj);
							check_rect_queue_draw((Object *)obj);
						} else {
							obj->X.full += obj->UD.UDbonus1.H009c.full;
							if(_barrel_gravity(obj)<0) {
								NEXT(obj->mode2);
								obj->Pool = 2;
								obj->exists = 1;
							}
							actiontick((Object *)obj);   
							check_rect_queue_draw((Object *)obj);
						}

						break;
					case 8:
						// 27b08
                        obj->X.full += obj->UD.UDbonus1.H009c.full;
                        if(_barrel_gravity(obj) < 0 && obj->YPI < 40 && obj->YPI >= 0) {
                            NEXT(obj->mode1);
                            RHSetActionList((Object *)obj, RHCODE(0x27c54), 3);
                        }
                        actiontick((Object *)obj);
                        check_rect_queue_draw((Object *)obj);
						break;
					case 10:
						// 27b34
                        if (obj->flag1) {       // still on screen?
                            obj->XPI += obj->UD.UDbonus1.H009c.part.integer;
                            actiontick((Object *)obj);
                            check_rect_queue_draw((Object *)obj);
                        } else {
                            NEXT(obj->mode0);
                        }
						break;
					FATALDEFAULT;
				}
			}
			break;
		case 4:
			sub_27bda(obj);
			break;
		case 6:
			// 27bb0
			sub_922c();
			obj->mode0 = 2;
			obj->mode1 = 0;
			obj->mode2 = 0;
			obj->UD.UDbonus1.h0084c = 0;
			obj->x002e = 0;
			obj->x002f = 0;
			obj->YPI = 0x100;
			obj->exists = 2;
			break;
		FATALDEFAULT;
	}
}
	


void sub_24f0e(Object *obj) {
	NEXT(obj->mode0);
    RHSetAction(obj, RHCODE(0x24f52));
}

void sub_24f22(Object_G2 *obj) {
	Object *nobj;
	
	CDCheckDecor(obj);
	if (obj->Energy < 0) {
		NEXT(obj->mode0);
		if ((nobj = AllocActor())) {
			nobj->exists = TRUE;
			nobj->Sel = 0x18;
			nobj->SubSel = obj->SubSel;
		}
	}
	check_rect_queue_draw((Object *)obj);
}



void actions_198a(void) {			/* 249fa */
	short i;
	
	debughook(1);
	g_d7=15;
	for(i=0; i<16; i++) {
		if(g.Objects2[i].exists == 0) {continue;}
		switch (g.Objects2[i].Sel) {
			case BLANKA_FISH:		_SMAct00(&g.Objects2[i]);		break;
			case KEN_DRUMS:			_SMKenDrums(&g.Objects2[i]);	break;
			case RYU_SIGNS:			_SMRyuSigns(&g.Objects2[i]);	break;
			case 3:					_SMAct03(&g.Objects2[i]);		break;
			case ACTB_DRUMFIRE:		_SMAct04(&g.Objects2[i]);		break;
			case ACTB_DRUMS:		_SMAct05(&g.Objects2[i]);		break;
			case ACTB_CAR:			_ActSMCar(&g.Objects2[i]);		break;
			case 7:					_SMAct07(&g.Objects2[i]);		break;
			case 8:					_SMAct08(&g.Objects2[i]);		break;
			case 9:					sub_27862(&g.Objects2[i]);		break;
			FATALDEFAULT;
		}
		g_d7--;
	}
}

