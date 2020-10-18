/*
 *  drums.c
 *  MT2
 *
 *  Created by Ben on 25/11/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
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

#include "drums.h"

extern Game g;

#pragma mark ActB04 Fires on top of the drums

const HitBoxAct hitb_28290[] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

const struct hitboxes hitboxes_28284 = {
	(HitBox *)hitb_28290,
	(HitBox *)hitb_28290,
	(HitBox *)hitb_28290,
	(HitBox *)hitb_28290,
 	          hitb_28290,
	(HitBox *)hitb_28290,
};

#pragma mark ActB05 Burning Drums

const HitBox hitb_25964[] = {{0,0,0,0}, {0, 37, 29, 32}};
const HitBoxAct hitb_2596c[] = {
	{0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{0, 37, 29, 32, 2, 1, 0, 1, 0, 0, 0, 0 },
};

const struct hitboxes hitboxes_25958 = {
	hitb_25964,
	hitb_25964,
	hitb_25964,
	hitb_25964,
	hitb_2596c,
	hitb_25964,
};

void _SMAct04(Object_G2 *obj) {		// 27ea2 ID4 BONUS3
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
			if (obj->XPI < (SCREEN_WIDTH / 2)) {
				obj->Direction = 0;
			} else {
				obj->Direction = 1;
			}
			break;
		case 2:
			if (obj->XPI < (SCREEN_WIDTH / 2)) {
				obj->Direction = 0;
			} else {
				obj->Direction = 1;
			}
			if (obj->mode1) {
				CDBonusCollisionCheck((Object *)obj);
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
                    RHSetActionList((Object *)obj, RHCODE(0x27fa6), data_27f72[RAND16WD]);
				} else {
					obj->LocalTimer = data_27f3e[obj->UD.UDbonus3.h00b0s];
                    RHSetActionList((Object *)obj, RHCODE(0x27fa6), 0);
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
			clearpush_1174(obj);
			FATALDEFAULT;
	}
}



#pragma mark ActB05 BONUS2 Burning drums
static void sub_2581a(Object *obj) {
	if (((obj->YPI & 0xc0) >> 5) != 6) {
		obj->Pool = (obj->YPI >> 5);
	} else {
		obj->Pool = 4;
	}
}


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
	sub_2581a((Object *)obj);
	queuesound(0x3b);
	check_rect_queue_draw((Object *)obj);
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
	check_rect_queue_draw((Object *)obj);
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
	sub_257d8((Object *)obj);
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
	Object *p;
	if (obj->UD.UDbonus2.H0092) {
		p = obj->UD.UDbonus2.H0092;
		if (p->mode1) {
			sub_2549a(p, obj);
			return;
		}
	}
	if (obj->UD.UDbonus2.H0094) {
		p = obj->UD.UDbonus2.H0094;
		if (p->mode1) {
			sub_2549a(p, obj);
		}
	}
}

static int sub_254da (Object_G2 *obj) {		// sets %d0 and %d3
	//todo
    return -1;
}

static void sub_25670 (Object_G2 *obj) {
	//todo
}

static int sub_2529a (Object_G2 *obj) {
	//todo
    return -1;
}
static int sub_255b2(Object_G2 *obj) {
	//todo
    return -1;
}
static Object *sub_24fc2(Object_G2 *obj) {
	//	if (obj->UD.UDunknown.h0097c) {
	//		sub_24fcc(obj->UD.UDunknown.h0098p, obj);
	//	} else {
	//		return;
	//	}
    return NULL; // XXX
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
			obj->Scroll = SCROLL_2;
			obj->Step = obj->Flip;
            RHSetAction((Object *)obj, RHCODE(0x25834));
			
			// userdata ...
			
			//todo
			
			break;
		case 2:						// 250c4
			CDBonusCollisionCheck(obj);
			switch (obj->mode1) {
				case 0:
					switch (obj->mode2) {
						case 0:
							if (obj->UD.UDbonus2.h0096c) {
								NEXT(obj->mode2);
							}
							actiontick((Object *)obj);
							/* FALLTHRU */
						case 2:
							sub_25476(obj);
							if(sub_2529a(obj)<0) {
								obj->mode1 = 4;
							}
							sub_25670(obj);
							if (obj->SubSel < 3) {
								check_rect_queue_draw((Object *)obj);
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
							CATrajectory((Object *)obj);
							//todo sub_25650(obj);
							sub_2581a((Object *)obj);
							temp = sub_254da(obj);
							if (temp & 0x08) {
								NEXT(obj->mode2);
							} else if(check_ground_collision((Object *)obj)){
								obj->mode2 = 2;
								obj->YPI = g.GroundPlaneY;
							}
							sub_24fc2(obj);
							check_rect_queue_draw((Object *)obj);
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
			clearpush_1174(obj);
		default:
			break;
			
			
	}
}

