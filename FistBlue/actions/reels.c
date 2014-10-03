/*
 *  reels.c
 *  MT2
 *
 *  Created by Ben on 14/12/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */
#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"


#include "gstate.h"
#include "structs.h"
#include "player.h"

#include "particle.h"
#include "lib.h"
#include "gfxlib.h"
#include "sound.h"
#include "actions.h"
#include "gemu.h"
#include "sf2io.h"

#include "reels.h"
typedef struct UserData_Act23 UD23;
extern Game g;

void ActStartVomit(Player *ply) {			// 1e304
	UD23 *ud;
	
	Object *obj;
	if ((obj = AllocActor())) {
		ud = (UD23 *)&obj->UserData;
		obj->SubSel = 0;
		obj->flag1  = TRUE;
		obj->Sel    = SF2ACT_VOMIT;
		obj->UserByte  = sf2rand() & 1;		// orange or grey vomit?
		obj->Owner  = ply;
		ud->x0093   = ply->Side;
		ud->x008e   = &FreeActor;	//??
	}
}
void action_1e336(Player *ply) {
	UD23 *ud;
	
	Object *obj;
	char temp;
	
	static const char data_1e39c[] = {
		0,0,0,1,1,2,2,2,0,0,0,1,1,2,2,2,
		0,0,0,1,1,1,2,2,0,0,0,1,1,1,2,2,
	};
	
	if ((obj = AllocActor())) {
		ud =  (UD23 *)&obj->UserData;
		obj->SubSel = 2;
		temp = data_1e39c[RAND32];
		if (temp) {			// hmm, wierd, but checked.
			obj->UserByte = temp-1;
			obj->exists = TRUE;
			obj->Sel = SF2ACT_VOMIT;	
			obj->Owner = ply;
			ud->x0093 = ply->Side;
			ud->x008e = &FreeActor;
		}
	}
}
static void _CreateDizzyObject(Player *ply, short d2) {		// 1e3c8
	Object *obj;
	UD23 *ud;
	if ((obj=AllocActor())) {
		ud = (UD23 *)&obj->UserData;
		ud->x008c	= d2;
		obj->exists = TRUE;
		obj->Sel	= SF2ACT_VOMIT;
		obj->SubSel = 4;		
		obj->UserByte = ply->DizzySpell < 0x8c ? 0 : 1 ;	// 0=stars,1=birds
		obj->Owner = ply;
		obj->X.full = obj->Y.full = 0;
	}
}
void StartDizzyAnim(Player *ply) {				// 1e3bc
	_CreateDizzyObject(ply, 0);
	_CreateDizzyObject(ply, 8);
	_CreateDizzyObject(ply, 16);
}
void ActBlankaBiteBlood(Player *ply) {			//1e402 not just Blanka XXX
	Object *obj;
	if ((obj=AllocActor())) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_VOMIT;
		obj->SubSel = 6;
		obj->Owner = ply;
	}
}
static Player *sub_1e7ae(Object *obj) {		// 1e7ae
	char x,y;
	Player *ply = obj->Owner;	
	
	x = data_1e804[ply->FighterID][obj->SubSel/2].x;
	y = data_1e804[ply->FighterID][obj->SubSel/2].y;
	
	if (ply->Flip) {obj->XPI = ply->XPI - x;} else {obj->XPI = ply->XPI + x;}
	obj->YPI = ply->YPI + y;
	
	return ply;
}

static POINT16 Act23RandomSmallOffset(void) {		//1e888
	static const char data_1e8a6[] = {-2, 2, 0, -2, 2, 0, -2, 2 };
	
	POINT16 op;
	short temp = sf2rand();
	op.x = data_1e8a6[temp & 7];
	op.y = data_1e8a6[(temp >> 3) & 7];
	return op;
}

static void Act23SMVomit(Object *obj) {					// 1e43a
	UD23 *ud = (UD23 *)obj->UserData;
	POINT16 op;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			setaction_list(obj, actlist_1e8d6, obj->SubSel + obj->UserByte);
			obj->Flip = obj->Owner->Flip;
			ud->PlyX = obj->Owner->XPI;
			ud->PlyY = obj->Owner->YPI;
			obj->LocalTimer = 30;
			obj->Pool = 0;
			sub_1e7ae(obj);
			op = Act23RandomSmallOffset();
			obj->XPI += op.x;
			obj->YPI += op.y;
			break;
		case 2:
			if (ud->x0093) { obj->Pool = 10; } else { obj->Pool = 8; }
			
			// move the vomit with the player
			obj->XPI += (obj->Owner->XPI - ud->PlyX);
			ud->PlyX = obj->Owner->XPI;
			obj->YPI += (obj->Owner->YPI - ud->PlyY);
			ud->PlyY = obj->Owner->YPI;
			
			if ((obj->AnimFlags & 0xff) == 0) {
				if (--obj->LocalTimer == 0) {
					NEXT(obj->mode0);
				}
			} else {
				NEXT(obj->mode0);
			}
			actiontick(obj);
			if (obj->SubSel == 0) {
				if (--ud->x0092) {
					check_rect_queue_draw(obj);
				} else {
					ud->x0092 = 3;
				}
				
			} else {
				check_rect_queue_draw(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
			FATALDEFAULT;
	}
}	

static void sub_1e59a(Object *obj) {		// birds and stars?
	UD23 *ud = (UD23 *) &obj->UserData;
	
	Player *ply;
	short temp2;
    
    static const short data_1e622[][4] = {
        {0, -0x800, -10,    0,},
        {-39,  0x300,   8,  320,},
        {26,  0x500,   6, -320},
    };
	static const short data_1e6f4[] = { 7, 0xe, 0xb, 0xe};

	switch (obj->mode0) {
		case 0:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->LocalTimer = 1;
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode0);
						setaction_list(obj, actlist_1e8d6, obj->SubSel + obj->UserByte);
						ply = sub_1e7ae(obj);
						obj->Flip = ply->Flip;
						ud->PlyX = ply->XPI;
						ud->PlyY = ply->YPI;
						ud->ObjX = obj->XPI;
						ud->ObjY = obj->YPI;
						obj->XPI      += data_1e622[ud->x008c / 8][0];
						obj->VelX.full = data_1e622[ud->x008c / 8][1];
						obj->YPI      += data_1e622[ud->x008c / 8][2];
						obj->VelY.full = data_1e622[ud->x008c / 8][3];
					}
					break;
					FATALDEFAULT;
			}
			break;
		case 2:				// 1e63a
			ply = obj->Owner;
			if (ply->DizzyStun == 0 || (ply->mode1 != PLSTAT_REEL && ply->mode1 != PLSTAT_TUMBLE) ) {
				NEXT(obj->mode0);	/* no longer dizzy */
			}
			obj->VelX.full += obj->XPI < ud->ObjX ? 0x100 : -0x100;
			obj->VelY.full += obj->YPI < ud->ObjY ?  0x40 : -0x40;
			
			temp2 = ply->XPI - ud->PlyX;
			obj->XPI += temp2;
			ud->ObjX += temp2;
			ud->PlyX = ply->XPI;
			
			temp2 = ply->YPI - ud->PlyY;
			obj->YPI += temp2;
			ud->ObjY += temp2;
			ud->PlyY = ply->YPI;
			
			obj->Flip = ply->VelX.full < 0 ? 0 : 1;
			obj->Pool = obj->Flip * 4;
			
			setaction_list(obj, actlist_1e8d6, ((obj->Flip * 2) + 4 + obj->UserByte));
			CAApplyVelocity(obj);		/* bottom half of calc-trajectory */
			if (obj->UserByte == 0) {
				obj->Draw1 = TRUE;
				obj->Draw2.part.integer = data_1e6f4[(g.tick & 0x6) / 2];
			}
			check_rect_queue_draw(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
			FATALDEFAULT;
	}
}
static void _act23_animate(Object *obj) {		// 1e77e
	UD23 *ud = (UD23 *) &obj->UserData;
	
	if (obj->AnimFlags & 0xff) {
		NEXT(obj->mode0);
	}
	actiontick(obj);
	if(--ud->x0092) {
		check_rect_queue_draw(obj);
	} else {
		ud->x0092 = 3;
	}
}

static void Act23SMBlood(Object *obj) {		// 1e6fc not actually blood, just a star
	POINT16 op;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Flip = obj->Owner->Flip;
			obj->XPI  = obj->Owner->XPI;
			obj->YPI  = obj->Owner->YPI;
			if (obj->Owner->FighterID == FID_BLANKA) {
				obj->XPI += obj->Flip ? 32 : -32 ;		
				obj->YPI += 69;
			} else {
				obj->XPI += obj->Flip ? 37 : -37 ;
				obj->YPI += 82;
			}
			obj->LocalTimer = 30;
			obj->Pool		= 0;
			op = Act23RandomSmallOffset();
			obj->XPI += op.x;
			obj->YPI += op.y;
			setaction_list(obj, actlist_1e8d6, 8);		/* star */
			break;
		case 2:
			_act23_animate(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}


static void sub_1e84c(Object *obj) {			// 1e84c blood
	UD23 *ud = (UD23 *) &obj->UserData;
	POINT16 op;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Flip ^= 1;
			op = Act23RandomSmallOffset();
			obj->XPI += op.x;
			obj->YPI += op.y;
			ud->x0092 = 3;
			setaction_direct(obj, actlist_1eb42);
			/* FALL THRU */
		case 2:
			_act23_animate(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
		default:
			break;
	}
}
void action_1e420(Object *obj) {		/* obj in %a6 */
	switch (obj->SubSel) {
		case 0:							//	teeth, blood
		case 2:							//  orange and grey vomit
			Act23SMVomit(obj);
			break;
		case 4:
			sub_1e59a(obj);				// animated star, bird
			break;
		case 6:							// small star, bird
			Act23SMBlood(obj);
			break;
		case 8:							// blood again
			sub_1e84c(obj);			
			break;
			FATALDEFAULT;
	}
}

