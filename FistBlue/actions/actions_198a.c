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





static void sub_24a50(Object *obj) {
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
static void sub_24a74(Object *obj) {
	
	Object *nobj;
	short i;
	/* kens drums, these seem to not clearpush afterward */
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



static void sub_24c4e(Object *obj) {
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

static void sub_24efa(Object *obj) {/* todo */}
static void sub_27ea2(Object *obj) {/* todo */}
static void sub_24ff6(Object *obj) {/* todo */}

#pragma mark The Car		

static int sub_25d94(Object_G2 *obj){
	if (obj->UD.UDcar.p1.as > 0 && obj->UD.UDcar.p1.bs != obj->UD.UDcar.p1.as) {
		obj->UD.UDcar.p1.bs = obj->UD.UDcar.p1.as;
		return TRUE;
	}
	return FALSE;
}
static int sub_25da8(Object_G2 *obj){
	if (obj->UD.UDcar.p2.as > 0 && obj->UD.UDcar.p2.bs != obj->UD.UDcar.p2.as) {
		obj->UD.UDcar.p2.bs = obj->UD.UDcar.p2.as;
		return TRUE;
	}
	return FALSE;
}
static void car_setaction(Object_G2 *obj) {//25dbc
	// XXX setaction_list(obj, data____, obj->UD.UDcar.h0091c + obj->UD.UDcar.h0092c);
}	

static void sub_25dcc(CarPlayer *cp) {
	cp->as = cp->bs = (char []){3,5,4,5,3,4,5,4,4,4,5,3,4,3,5,3}[sf2rand() & 15];
}

static int car_check_platform(Player *ply) {		// 26096
	ply->PlatformFallDir = 0;
	
	// todo
}

static void sub_24df2(Object_G2 *obj) {
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
				if (car_check_platform(PLAYER1) == FALSE) {
					obj->UD.UDcar.h0090c = 0;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 5;
						
					}
				}
			} else {
				if (car_check_platform(PLAYER1) != FALSE) {
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
				if (car_check_platform(PLAYER1) == FALSE) {
					obj->UD.UDcar.h0090c = 0;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 5;
						
					}
				}
			} else {
				if (car_check_platform(PLAYER1) != FALSE) {
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
	
	

static void sub_25984(Object_G2 *obj) {
	/* userdata: */
	
	
	switch (obj->mode0) {
		case 0:
			memclear(&obj->UD.UDcar, sizeof (UDcar));
			g.x8abe = FALSE;
			// XXX obj->HitBoxes = data_25d6c;
			sub_25dcc(&obj->UD.UDcar.p1);
			sub_25dcc(&obj->UD.UDcar.p2);
			car_setaction(obj);
			break;
		case 2:
			//sub_25df2(obj);
			//sub_7d99a(obj);			// collision detection			
			
			if (!obj->mode1) {
				if (obj->mode2) {
					switch (obj->mode3) {		// 25a10
						case 0:
							//sub_25a22();
							if (sub_25d94(obj) < 0) {
								NEXT(obj->mode3);
								obj->UD.UDcar.h0091c++;
								sub_25dcc(&obj->UD.UDcar.p1);
							}
							if (sub_25da8(obj) < 0) {
								NEXT(obj->mode2);
								obj->UD.UDcar.h0092c = 3;
							}
							break;
						case 2:
							//sub_25a4a();
							if (sub_25d94(obj) < 0) {
								NEXT(obj->mode3);
								++obj->UD.UDcar.h0091c;
							}
							if (sub_25da8(obj) < 0) {
								NEXT(obj->mode2);
								obj->UD.UDcar.h0092c = 3;
							}
							break;
						case 4:
							//sub_25a6a();
							if (sub_25da8(obj) < 0) {
								NEXT(obj->mode2);
								obj->UD.UDcar.h0092c = 3;
								sub_25dcc(&obj->UD.UDcar.p1);
								sub_25dcc(&obj->UD.UDcar.p2);
							}
							break;
						FATALDEFAULT;
					}
				} else {
					switch (obj->mode3) {
						case 0:
							if (sub_25d94(obj) < 0) {
								NEXT(obj->mode3);
								++obj->UD.UDcar.h0091c;
							}
							sub_25dcc(&obj->UD.UDcar.p1);
							break;
						case 2:
							if (sub_25d94(obj) < 0) {
								NEXT(obj->mode3);
								++obj->UD.UDcar.h0091c;
								sub_25dcc(&obj->UD.UDcar.p1);
								sub_25dcc(&obj->UD.UDcar.p2);
							}
							break;
						case 4:
							if (sub_25d94(obj) < 0) {
								NEXT(obj->mode1);
								obj->mode2 = 0;
								obj->mode3 = 0;
								obj->UD.UDcar.h0092c = 0;
								obj->UD.UDcar.h0091c = 6;
								sub_25dcc(&obj->UD.UDcar.p1);
							}
							break;
						FATALDEFAULT;
					}
				}

			} 
			else {
				// 25b02
				switch (obj->mode2) {
					case 0-16:
						//25b26();
						if(sub_25d94(obj) < 0) {
							NEXT(obj->mode2);
							if(++obj->UD.UDcar.h0091c == 15) {
								g.x8abe = TRUE;
							}
							sub_25dcc(&obj->UD.UDcar.p1);
							car_setaction(obj);
						}
						break;
					case 18:
						//25b50();
						if(sub_25d94(obj) < 0) {
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
	

static void sub_272c6(Object *obj) {/* todo */}
static void sub_274e4(Object *obj) {/* todo */}
static void sub_27862(Object *obj) {/* todo */}



void actions_198a(void) {			/* 249fa */
	short i;
	
	debughook(1);
	g_d7=15;
	for(i=0; i<16; i++) {
		if(g.Objects2[i].exists == 0) {break;}
		switch (g.Objects2[i].Sel) {
			case BLANKA_FISH:
				sub_24a50(&g.Objects2[i]);
				break;
			case KEN_DRUMS:
				sub_24a74(&g.Objects2[i]);
				break;
			case RYU_SIGNS:
				sub_24c4e(&g.Objects2[i]);
				break;
			case 3:
				sub_24efa(&g.Objects2[i]);
				break;
			case 4:
				sub_27ea2(&g.Objects2[i]);
				break;
			case 5:
				sub_24ff6(&g.Objects2[i]);
				break;
			case 6:		// the Car
				sub_25984(&g.Objects2[i]);
				break;
			case 7:
				sub_272c6(&g.Objects2[i]);
				break;
			case 8:
				sub_274e4(&g.Objects2[i]);
				break;
			case 9:
				sub_27862(&g.Objects2[i]);
				break;
			default:
				printf("action 198a id %d not implemented\n", g.Objects2[i].Sel);
				break;
		}
		g_d7--;
	}
}

