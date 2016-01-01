/*
 *  car.c
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

#include "sound.h"

#include "actions_198a.h"
#include "actions.h"

#include "car.h"
#include "coll_bonus.h"

extern Game g;
extern GState gstate_Scroll1;

const HitBox hitb_25d86[] = {
	{ 0,  0, -56, 64}, {22, 25, 56, 64}, {22, 25, 0, 25}, {61, 22, 48, 46},};
const HitBoxAct hitb_25d78[] = {0,0,0,0,0,0,0,0,0,0,0,0};

const struct hitboxes hitboxes_25d6c = {
	hitb_25d86,
	hitb_25d86,
	(const struct hitbox *)hitb_25d78,
	(const struct hitbox *)hitb_25d78,
	hitb_25d78,
	(const struct hitbox *)hitb_25d78,
};	


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
    RHSetActionList((Object *)obj, RHCODE(0x25b8a), obj->UD.UDcar.HitsTaken + obj->UD.UDcar.h0092c);
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
static void sub_25f06(Object_G2 *obj) {		// players on top of car
	Object *nobj;
	switch (obj->UD.UDcar.PeopleOnMe) {
		case 0:		// no players on me
			if (_check_platform(&g.Player1, obj)) {
				obj->UD.UDcar.PeopleOnMe = 1;
				if ((nobj = AllocActor())) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x2;
				}
			}
			else if (_check_platform(&g.Player2, obj)) {
				obj->UD.UDcar.PeopleOnMe = 2;
				if ((nobj = AllocActor())) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x2;
				}
			}
			break;
		case 1:		// player1 is on me
			if (_check_platform(&g.Player2, obj)) {
				obj->UD.UDcar.PeopleOnMe = 3;
				if ((nobj = AllocActor())) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x3;
				}
			} else if (!_check_platform(&g.Player1, obj)) {
				obj->UD.UDcar.PeopleOnMe = 0;
				if ((nobj = AllocActor())) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x5;
				}
			}				
			break;
		case 2:		// player2 is on me
			if (_check_platform(&g.Player1, obj)) {
				obj->UD.UDcar.PeopleOnMe = 3;
				if ((nobj = AllocActor())) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x3;
				}
			}
			else if (!_check_platform(&g.Player2, obj)) {
				obj->UD.UDcar.PeopleOnMe = 0;
				if ((nobj = AllocActor())) {
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
					if ((nobj = AllocActor())) {
						nobj->exists = TRUE;
						nobj->Sel = 0x32;
						nobj->SubSel = 0x6;
					}					
				} else {
					obj->UD.UDcar.PeopleOnMe = 2;
					if ((nobj = AllocActor())) {
						nobj->exists = TRUE;
						nobj->Sel = 0x32;
						nobj->SubSel = 0x4;
					}
				}
				
			} else {
				if (!_check_platform(&g.Player2, obj)) {
					obj->UD.UDcar.PeopleOnMe = 1;
					if ((nobj = AllocActor())) {
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
					if ((newobj = AllocActor())) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 5;
						
					}
				}
			} else {
				if (_check_platform(PLAYER1, obj) != FALSE) {
					obj->UD.UDcar.PeopleOnMe = 1;
					if ((newobj = AllocActor())) {
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
					if ((newobj = AllocActor())) {
						newobj->exists = TRUE;
						newobj->Sel    = 0x32;
						newobj->SubSel = 5;
						
					}
				}
			} else {
				if (_check_platform(PLAYER1, obj) != FALSE) {
					obj->UD.UDcar.PeopleOnMe = 2;
					if ((newobj = AllocActor())) {
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

void _ActSMCar(Object_G2 *obj) {			// The CarID 6, BONUS0
	switch (obj->mode0) {
		case 0:
            NEXT(obj->mode0);
			memclear(&obj->UD.UDcar, sizeof (UDcar));
			g.x8abe = FALSE;
			obj->HitBoxes = &hitboxes_25d6c;
			_car_init_as_bs(&obj->UD.UDcar.p1);
			_car_init_as_bs(&obj->UD.UDcar.p2);
			car_setaction(obj);
			break;
		case 2:
			_people_on_roof(obj);
			CDBonusCollisionCheck(obj);			// collision detection
			
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
							ActStartScreenWobble();
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
			clearpush_1174(obj);
			break;
			FATALDEFAULT;
	}
}





/*
 00025b80                                 00 22 00 5a 00 92  |NuNuT...Nu.".Z..|
 00025b90  00 3e 00 76 00 ae 00 ca  00 e6=01 02 01 1e 01 3a  |.>.v...........:|
 00025ba0  01 56 01 72 01 8e 01 aa  01 c6 00 00 00 08 80 00  |.V.r............|
 00025bb0  00 02 61 44 01 02 00 00  00 00 00 00 00 00 00 00  |..aD............|
 00025bc0  00 00 00 00 00 02 5b ac  00 08 80 00 00 02 62 6e  |......[.......bn|
 00025bd0  01 02 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
 00025be0  00 02 5b c8 00 08 80 00  00 02 63 98 01 02 00 00  |..[.......c.....|
 00025bf0  00 00 00 00 00 00 00 00  00 00 00 00 00 02 5b e4  |..............[.|
 00025c00  00 08 80 00 00 02 64 c2  01 02 00 00 00 00 00 00  |......d.........|
 00025c10  00 00 00 00 00 00 00 00  00 02 5c 00 00 08 80 00  |..........\.....|
 00025c20  00 02 65 ec 01 02 00 00  00 00 00 00 00 00 00 00  |..e.............|
 00025c30  00 00 00 00 00 02 5c 1c  00 08 80 00 00 02 67 16  |......\.......g.|
 00025c40  01 02 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
 00025c50  00 02 5c 38 00 08 80 00  00 02 68 40 03 00 00 00  |..\8......h@....|
 00025c60  00 00 00 00 00 00 00 00  00 00 00 00 00 02 5c 54  |..............\T|
 00025c70  00 08 80 00 00 02 69 6a  03 00 00 00 00 00 00 00  |......ij........|
 00025c80  00 00 00 00 00 00 00 00  00 02 5c 70 00 08 80 00  |..........\p....|
 00025c90  00 02 6a 94 03 00 00 00  00 00 00 00 00 00 00 00  |..j.............|
 00025ca0  00 00 00 00 00 02 5c 8c  00 08 80 00 00 02 6b be  |......\.......k.|
 00025cb0  03 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
 00025cc0  00 02 5c a8 00 08 80 00  00 02 6c e8 03 00 00 00  |..\.......l.....|
 00025cd0  00 00 00 00 00 00 00 00  00 00 00 00 00 02 5c c4  |..............\.|
 00025ce0  00 08 80 00 00 02 6e 12  03 00 00 00 00 00 00 00  |......n.........|
 00025cf0  00 00 00 00 00 00 00 00  00 02 5c e0 00 08 80 00  |..........\.....|
 00025d00  00 02 6f 3c 03 00 00 00  00 00 00 00 00 00 00 00  |..o<............|
 00025d10  00 00 00 00 00 02 5c fc  00 08 80 00 00 02 70 66  |......\.......pf|
 00025d20  03 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
 00025d30  00 02 5d 18 00 08 80 00  00 02 71 90 03 00 00 00  |..].......q.....|
 00025d40  00 00 00 00 00 00 00 00  00 00 00 00 00 02 5d 34  |..............]4|
 00025d50  00 08 80 00 00 02 72 ba  03 00 00 00 00 00 00 00  |......r.........|
 00025d60  00 00 00 00 00 00 00 00  00 02 5d 50 00 18 00 18  |..........]P....|
 00025d70  00 0c 00 0c 00 0c 00 0c  00 00 00 00 00 00 00 00  |................|
 00025d80  00 00 00 00 00 00 00 00  c8 40 16 19 38 40 16 19  |.........@..8@..|
 00025d90  00 19 3d 16 30 2e 00 80  6b 0c b0 6e 00 82 67 06  |..=.0...k..n..g.|


*/