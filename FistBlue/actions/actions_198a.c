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

#include "lib.h"
#include "fightgfx.h"
#include "gfxlib.h"
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
	BLANKA_FISH = 0,
	KEN_DRUMS,
	RYU_SIGNS,
	ACT_B03,
	ACTB_DRUMFIRE,			// BONUS3 Collisions
	ACTB_DRUMS,				// BONUS2 Collisions
	ACTB_CAR,				// BONUS0 Collisions
	ACTB_GUILE_CRATE,
	ACT_B08,
	ACT_B09,				// BONUS1 Collisions
};


static void sub_25f06(Object_G2 *obj);
static void sub_2581a(Object *obj);



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
			obj->HitBoxes = &hitboxes_24eda;
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

static void sub_24fcc(Object *obja0, Object *obja6) {
	obja0->XPI = obja6->XPI;
	obja0->YPI = obja6->YPI + 64;
}

static Object *sub_24fc2(Object_G2 *obj) {
//	if (obj->UD.UDunknown.h0097c) {
//		sub_24fcc(obj->UD.UDunknown.h0098p, obj);
//	} else {
//		return;
//	}
}

static void sub_24fe0(Object_G2 *obj) {
//	if (obj->UD.UDunknown.h0097c) {
//		obj->UD.UDunknown.h0098p->mode0 = 4;
//		obj->UD.UDunknown.h0097 = FALSE;
//	}
}


static void sub_24f96(int argd0, Object *obj) {
	
}

const static Image image_24f6a={0, 0, 0x28, 0, 0, 0, };
const static CAFrame frame_24f52={0x8, 0, 0, &image_24f6a, 0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
	
const HitBox hitb_24f8e[] = {EMPTY_HITBOX, {0, 80, 16, 80}};

const struct hitboxes hitboxes_24f76 = {
	hitb_null,
	hitb_null,
	hitb_null,
	hitb_null,
	hitbact_null,
	hitb_24f8e,
};

static void _SMAct03(Object *obj) {		// 24efa
	Object *nobj;
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->HitBoxes = &hitboxes_24f76;
			setaction_direct(obj, &frame_24f52);
			break;
		case 2:
			CDCheckDecor(obj);
			if (obj->Energy < 0) {
				NEXT(obj->mode0);
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x18;
					nobj->SubSel = obj->SubSel;
				}
				check_rect_queue_draw(obj);
			}
			break;
		case 4:
		case 6:
			clearpush_1174(obj);
			break;
		FATALDEFAULT;
	}
}


static void _SMAct04(Object_G2 *obj) {		// 27ea2 ID4 BONUS3
	// Fires on top of drums
	const static char data_27f3e[]={1,1,2,2,3,3,3,3,3,3,3,3,3,3,2,2,1,1,-1,0};
	const static short data_27f72[]={
		0x64, 0x78, 0x8c, 0x78, 0x64, 0x78, 0x8c, 0x64,
		0x64, 0x78, 0x8c, 0x78, 0x8c, 0x78, 0x8c, 0x64
	};
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			NEXT(obj->mode1);
			obj->HitBoxes = &hitboxes_28284;	// null hitboxes
			obj->Pool = 4;
			obj->UD.UDbonus3.h00b0s = 0;
			if (obj->XPI < 192) {
				obj->Direction = 0;
			} else {
				obj->Direction = 1;
			}
			break;
		case 2:
			if (obj->XPI < 192) {
				obj->Direction = 0;
			} else {
				obj->Direction = 1;
			}
			if (obj->mode1) {
				sub_7d99a((Object *)obj);
				if (obj->mode2) {
					if ((obj->AnimFlags & 0x8000) == 0) {
						actiontick((Object *)obj);
						check_rect_queue_draw((Object *)obj);
						return;
					}
					obj->mode2 = 0;
					++obj->UD.UDbonus3.h00b0s;
				}
				NEXT(obj->mode2);
				if (data_27f3e[obj->UD.UDbonus3.h00b0s] < 0) {
					obj->mode1 = 0;
					obj->mode2 = 0;
					setaction_list((Object *)obj, actlist_27fa6, data_27f72[RAND16W/2]);
				} else {
					obj->LocalTimer = data_27f3e[obj->UD.UDbonus3.h00b0s];
					setaction_list((Object *)obj, actlist_27fa6, 0);
				}
			} else {
				// 27f92
				if (--obj->LocalTimer == 0) {
					NEXT(obj->mode1);
					obj->UD.UDbonus3.h00b0s = 0;
				}
			}
			break;
		case 4:
		case 6:
			clearpush_1174((Object *)obj);
		FATALDEFAULT;
	}
}



#pragma mark ActB05 BONUS2 Burning drums
static void sub_2525a(Object_G2 *obj) {
	if (obj->YPI >= 48) {
		obj->Pool = 2;
	} else {
		obj->Pool = 0;
	}
	obj->mode1 = 0;
	obj->mode2 = 2;
	obj->UD.UDbonus2.h0084c = 0;
	obj->VelX.full = 0;
	obj->VelY.full = 0;
	obj->AclX.full = 0;
	obj->AclY.full = 0;
	sub_2581a(obj);
	queuesound(0x3b);
	check_rect_queue_draw(obj);
}
static void sub_25220(Object_G2 *obj) {
	if (--obj->UD.UDbonus2.h009ac == 0) {
		if (obj->UD.UDbonus2.h0092c) {
			sub_2525a(obj);
			return;
		}
		obj->UD.UDbonus2.h009ac = 1;
	}
	obj->mode1 = 2;
	obj->mode2 = 0;
	obj->VelY.full = 0x200;
	queuesound(0x3b);
	check_rect_queue_draw(obj);
}
static void sub_25252(Object_G2 *obj) {
	if (obj->VelY.full > -0x500) {
		sub_2525a(obj);
	} else {
		sub_25220(obj);
	}
}

static void sub_257d8 (Object *obj) {			// 257d8 random force
	const static short data_257fa[] = {
		-32, -16, -16, 0, 0, 16, 16, 32,
		-32, -32, -16, 0, 0, 16, 32, 32
	};
	
	int temp = sf2rand();
	obj->VelX.full += data_257fa[(temp >> 1) & 0xf];
	obj->VelY.full += data_257fa[(temp >> 5) & 0xf];
}

static void sub_251f4(Object_G2 *obj) {
	obj->UD.UDbonus2.h009ac = 5;
	obj->mode1 = 2;
	obj->mode2 = 0;
	obj->UD.UDbonus2.h0092c = 0;
	obj->VelY.full = 0x0600;
	obj->AclX.full = 0;
	obj->AclY.full = 0x0030;
	sub_257d8(obj);
	check_rect_queue_draw((Object *)obj);
}
	
static void sub_2549a(Object *ply, Object_G2 *obj) {
	if (ply->XPI < obj->XPI) {
		obj->VelX.full = 0x0080;
	} else {
		obj->VelX.full = -0x0080;
	}
	sub_251f4(obj);
}


static void sub_257aa(Object_G2 *obj, Player *ply) {
	if (ply->exists) {
		if(ABS(obj->XPI - ply->XPI) - ply->Size >= 29) {
			if (ply->OppXDist > ABS(obj->XPI - ply->XPI) - ply->Size - 29) {
				ply->OppXDist = ABS(obj->XPI - ply->XPI) - ply->Size - 29;
			}
		} else {
			ply->OppXDist = 0;
		}

	}
}

static void sub_257a0(Object_G2 *obj) {
	sub_257aa(obj, PLAYER1);
	sub_257aa(obj, PLAYER2);
}

static void sub_25476(Object_G2 *obj) {
	char *p;
	if (obj->UD.UDbonus2.H0092) {
		p = obj->UD.UDbonus2.H0092;
		if (p[3]) {
			sub_2549a(p, obj);
			return;
		}
	}
	if (obj->UD.UDbonus2.H0094) {
		p = obj->UD.UDbonus2.H0094;
		if (p[3]) {
			sub_2549a(p, obj);
		}
	}
}



void _SMAct05(Object_G2 *obj) {				// 24ff6 Act05 Bonus2
	int temp;
	int d1;
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->mode2 = 0;
			d1 = sub_255b2(obj);
			// redundant btst
			obj->HitBoxes = &hitboxes_25958;
			obj->Pool = 2;
			obj->Scroll = 0;
			obj->Step = obj->Flip;
			//setaction_direct(obj, action_25834);
			
			// userdata ...
			
			//todo
			
			break;
		case 2:						// 250c4
			sub_7d99a(obj);
			switch (obj->mode1) {
				case 0:
					switch (obj->mode2) {
						case 0:
							if (obj->UD.UDbonus2.h0096c) {
								NEXT(obj->mode2);
							}
							actiontick(obj);
							/* FALLTHRU */
						case 2:
							sub_25476(obj);
							if(sub_2529a(obj)<0) {
								obj->mode1 = 4;
							}
							sub_25670(obj);
							if (obj->SubSel < 3) {
								check_rect_queue_draw(obj);
							}
							break;
						FATALDEFAULT;
					}
					break;
				case 2:
					switch (obj->mode2) {
						case 0:
							obj->UD.UDbonus2.h0084c = 1;
							temp = sub_2529a(obj);
							if (temp < 0) {
								obj->mode1 = 4;
							} else if (temp > 0) {
								obj->UD.UDbonus2.h009ac = 5;
								obj->VelY.full = 0x400;
								obj->AclX.full = 0;
								if (obj->Direction) {
									obj->VelX.full = 0x80;
								} else {
									obj->VelX.full = -0x80;
								}
							}
							CATrajectory(obj);
							sub_25650(obj);
							sub_2581a(obj);
							temp = sub_254da(obj);
							if (temp & 0x08) {
								NEXT(obj->mode2);
							} else if(check_ground_collision(obj)){
								obj->mode2 = 2;
								obj->YPI = g.GroundPlaneY;
							}
							sub_24fc2(obj);
							check_rect_queue_draw(obj);
							break;
						case 2:
							
						default:
							break;
					}
					break;
				case 4:
					//251d2
					break;
				FATALDEFAULT;
			}
			sub_257a0(obj);
			break;
		case 4:
		case 6:
			clearpush_1174((Object *)obj);
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
	// XXX setaction_list(obj, data____, obj->UD.UDcar.HitsTaken + obj->UD.UDcar.h0092c);
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
	if (PLAYER2->exists && PLAYER2->Jumping == 0 && (obj->UD.UDcar.PeopleOnMe & 2)) {
		PLAYER2->XPI += x;
		PLAYER2->YPI -= y;
	}
	switch (g.ActiveHumans) {
		case 0:
			// can't happen
			break;
		case 1:
			if (obj->UD.UDcar.PeopleOnMe) {
				if (_check_platform(PLAYER1, obj) == FALSE) {
					obj->UD.UDcar.PeopleOnMe = 0;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 5;
						
					}
				}
			} else {
				if (_check_platform(PLAYER1, obj) != FALSE) {
					obj->UD.UDcar.PeopleOnMe = 1;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 2;
						
					}
				}
			}

			break;
		case 2:
			if (obj->UD.UDcar.PeopleOnMe) {
				if (_check_platform(PLAYER1, obj) == FALSE) {
					obj->UD.UDcar.PeopleOnMe = 0;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 5;
						
					}
				}
			} else {
				if (_check_platform(PLAYER1, obj) != FALSE) {
					obj->UD.UDcar.PeopleOnMe = 2;
					if (newobj = AllocActor()) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 2;
						
					}
				}
			}
			
			break;
		case 3:
			sub_25f06(obj);
			break;
		FATALDEFAULT;
	}
}
static void _ActSMCar(Object_G2 *obj) {			// The CarID 6, BONUS0
	switch (obj->mode0) {
		case 0:
			memclear(&obj->UD.UDcar, sizeof (UDcar));
			g.x8abe = FALSE;
			obj->HitBoxes = &hitboxes_25d6c;
			_car_init_as_bs(&obj->UD.UDcar.p1);
			_car_init_as_bs(&obj->UD.UDcar.p2);
			car_setaction(obj);
			break;
		case 2:
			_people_on_roof(obj);
			sub_7d99a((Object *)obj);			// collision detection			
			
			if (!obj->mode1) {
				if (obj->mode2) {
					switch (obj->mode3) {				//25a10
						case 0:							//25a22
							if (_car_check_P1(obj) < 0) {
								NEXT(obj->mode3);
								obj->UD.UDcar.HitsTaken++;
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
								++obj->UD.UDcar.HitsTaken;
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
								++obj->UD.UDcar.HitsTaken;
							}
							_car_init_as_bs(&obj->UD.UDcar.p1);
							break;
						case 2:
							if (_car_check_P1(obj) < 0) {
								NEXT(obj->mode3);
								++obj->UD.UDcar.HitsTaken;
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
								obj->UD.UDcar.HitsTaken = 6;
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
							if(++obj->UD.UDcar.HitsTaken == 15) {
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
	switch (obj->UD.UDcar.PeopleOnMe) {
		case 0:		// no players on me
			if (_check_platform(&g.Player1, obj)) {
				obj->UD.UDcar.PeopleOnMe = 1;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x2;
				}
			}
			else if (_check_platform(&g.Player2, obj)) {
				obj->UD.UDcar.PeopleOnMe = 2;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x2;
				}
			}
			break;
		case 1:		// player1 is on me
			if (_check_platform(&g.Player2, obj)) {
				obj->UD.UDcar.PeopleOnMe = 3;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x3;
				}
			} else if (!_check_platform(&g.Player1, obj)) {
				obj->UD.UDcar.PeopleOnMe = 0;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x5;
				}
			}				
			break;
		case 2:		// player2 is on me
			if (_check_platform(&g.Player1, obj)) {
				obj->UD.UDcar.PeopleOnMe = 3;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x3;
				}
			}
			else if (!_check_platform(&g.Player2, obj)) {
				obj->UD.UDcar.PeopleOnMe = 0;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x5;
				}
			}
			break;
		case 3:		// both players are on me, wait for both to come off;
			if (!_check_platform(&g.Player1, obj)) {
				if (!_check_platform(&g.Player2, obj)) {
					obj->UD.UDcar.PeopleOnMe = 0;
					if (nobj = AllocActor()) {
						nobj->exists = TRUE;
						nobj->Sel = 0x32;
						nobj->SubSel = 0x6;
					}					
				} else {
					obj->UD.UDcar.PeopleOnMe = 2;
					if (nobj = AllocActor()) {
						nobj->exists = TRUE;
						nobj->Sel = 0x32;
						nobj->SubSel = 0x4;
					}
				}

			} else {
				if (!_check_platform(&g.Player2, obj)) {
					obj->UD.UDcar.PeopleOnMe = 1;
					if (nobj = AllocActor()) {
						nobj->exists = TRUE;
						nobj->Sel = 0x32;
						nobj->SubSel = 0x4;
					}
					
				}
			}
			break;
		FATALDEFAULT;
	}
}

#pragma mark Act07 Guiles's Crate
static void _SMAct07(Object *obj) {			// 272c6
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
			setaction_direct(obj, actlist_273a2);
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
							if (nobj=AllocActor()) {
								nobj->exists = TRUE;
								nobj->Sel = 0x1c;
								nobj->SubSel = data_27384[RAND16];
								nobj->ZDepth = obj->ZDepth;
								nobj->XPI = obj->XPI;
								nobj->YPI = obj->YPI + 0x2d;
							}
						}
						setaction_direct(obj, actlist_27404);
					}
					break;
				case 4:
					actiontick(obj);
					break;
				FATALDEFAULT;
			}
			check_rect_queue_draw(obj);
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
			// XXX think we forgot AllocActor here?
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
					if (obj->UD.UDactB08.h0080c) {
						if (_Act08PlatformCheck(&g.Player1, obj)) {
							obj->UD.UDactB08.h0080c = 1;
							if (nobj = AllocActor()) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x2;
							}
						}
					} else {
						if(_Act08PlatformCheck(&g.Player1, obj) == 0) {
							obj->UD.UDactB08.h0080c = 0;
							if (nobj = AllocActor()) {
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
							if (nobj = AllocActor()) {
								nobj->exists = TRUE;
								nobj->Sel = 0x32;
								nobj->SubSel = 0x2;
							}
						}
					} else {
						if(_Act08PlatformCheck(&g.Player2, obj) == 0) {
							obj->UD.UDactB08.h0080c = 0;
							if (nobj = AllocActor()) {
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
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x2;
								}
							}
							if (_Act08PlatformCheck(&g.Player2, obj)) {
								obj->UD.UDactB08.h0080c = 2;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x2;
								}
							}
							break;
						case 1:
							if (_Act08PlatformCheck(&g.Player2, obj)) {
								obj->UD.UDactB08.h0080c = 3;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x3;
								}								
							} else if (_Act08PlatformCheck(&g.Player1, obj)==0) {
								obj->UD.UDactB08.h0080c = 0;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x5;
								}
							}
							break;
						case 2:
							if (_Act08PlatformCheck(&g.Player1, obj)) {
								obj->UD.UDactB08.h0080c = 3;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x3;
								}								
							} else if (_Act08PlatformCheck(&g.Player2, obj)==0) {
								obj->UD.UDactB08.h0080c = 0;
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
								obj->UD.UDactB08.h0080c = 0;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x5;
								}
							} else if (_Act08PlatformCheck(&g.Player2, obj) == 0) {
								obj->UD.UDactB08.h0080c = 1;
								if (nobj = AllocActor()) {
									nobj->exists = TRUE;
									nobj->Sel = 0x32;
									nobj->SubSel = 0x4;
								}
							} else if (_Act08PlatformCheck(&g.Player1, obj) == 0) {
								obj->UD.UDactB08.h0080c = 2;
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
			clearpush_1174((Object *)obj);
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
	unsigned char tmp = 0;
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

static void sub_27bda(Object_G2 *obj) {
	Object *nobj;
	int i;
	if (nobj = AllocActor()) {
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
		if (nobj = AllocActor()) {
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

static int sub_27b4c(Object_G2 *obj) {				// 27b4c
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
static void sub_2581a(Object *obj) {
	if (((obj->YPI & 0xc0) >> 5) != 6) {
		obj->Pool = (obj->YPI >> 5);
	} else {
		obj->Pool = 4;
	}
}


static void sub_27862(Object_G2 *obj) {			// 27862 Act09 BONUS1
	const static char data_278c2[] = {
		0, 1, 2, 0, 1, 2, 0, 3, 0, 1, 2, 0, 1, 2, 0, 0
	};
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
			obj->Energy = data_278c2[RAND16W/2];
			obj->EnergyDash = obj->Energy;
			break;
		case 2:
			CDCheckDecor((Object *)obj);
			sub_7d99a((Object *)obj);
			if (obj->Energy < 0) {
				sub_27912(obj);
				return;
			} else if (obj->Energy != obj->EnergyDash) {
				// 2793e
				obj->EnergyDash = obj->Energy;
				obj->UD.UDbonus1.Accel.full = 0xffffc000;	
				obj->Flip = 0;
				if (obj->Direction) {
					obj->UD.UDbonus1.H009c.part.integer = 2;
				} else {
					obj->UD.UDbonus1.H009c.part.integer = -2;
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
						if (obj->x002e != 0) {
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
							sub_278ac(obj);
							obj->Pool = 0;
							setaction_list((Object *)obj, actlist_27c54, 0);
							check_rect_queue_draw((Object *)obj);
						
						}
						break;
					case 2:					// 27a00
						obj->XPI += obj->UD.UDbonus1.H009c.part.integer;
						if (obj->XPI >= 0xa0 && obj->XPI <0xe0 ) {
							NEXT(obj->mode1);
							obj->UD.UDbonus1.Velocity.full = 0;
							obj->UD.UDbonus1.Accel.full = 0xffffc0000;
						}
						actiontick(obj);
						check_rect_queue_draw(obj);
						break;
					case 4:					// 27a34
						obj->XPI += obj->UD.UDbonus1.H009c.part.integer;
						sub_27b4c(obj);
						if (obj->XPI < 0xa0) {
							NEXT(obj->mode1);
							g.x8a64[0] = 1;
							obj->UD.UDbonus1.Velocity.full = 0x0004c000;
							temp = (sf2rand() & 1) + obj->SubSel;
							obj->UD.UDbonus1.H009c.full = (u16[]){0xfffe8000,0xffff8000,0x00008000,0x00018000}[temp];
						}
						actiontick(obj);
						check_rect_queue_draw(obj);
						break;
					case 6:										// 27a8a
						if(obj->mode2) {
							obj->X.full += obj->UD.UDbonus1.H009c.full;
							if(sub_27b4c(obj)<0 && obj->YPI < 40) {
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
							actiontick(obj);
							check_rect_queue_draw(obj);
						} else {
							obj->X.full += obj->UD.UDbonus1.H009c.full;
							if(sub_27b4c(obj)<0) {
								NEXT(obj->mode2);
								obj->Pool = 2;
								obj->exists = 1;
							}
							actiontick(obj);   
							check_rect_queue_draw(obj);
						}

						break;
					case 8:
						// 27b08
						break;
					case 10:
						// 27b34
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
	


//void sub_24f0e(Object *obj) {
//	NEXT(obj->mode0);
//	setaction_direct(obj, action_24f52);
//}

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


void actions_198a(void) {			/* 249fa */
	short i;
	
	debughook(1);
	g_d7=15;
	for(i=0; i<16; i++) {
		if(g.Objects2[i].exists == 0) {break;}
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

