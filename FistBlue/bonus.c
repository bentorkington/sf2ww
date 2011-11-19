/*
 *  bonus.c
 *  MT2
 *
 *  Created by Ben on 19/11/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include "sf2const.h"
#include "sf2types.h"
#include "sf2macros.h"

#include "structs.h"

#include "lib.h"
#include "collision.h"
#include "bonus.h"



static void sub_2581a(Object *obj) {
	if (((obj->YPI & 0xc0) >> 5) != 6) {
		obj->Pool = (obj->YPI >> 5);
	} else {
		obj->Pool = 4;
	}
}



void sub_24f0e(Object *obj) {
	NEXT(obj->mode0);
	//todo setaction_direct(obj, action_24f52);
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

void sub_24ff6(Object *obj) {
	switch obj->mode0 {
	case 0:
		NEXT(obj->mode0);
		obj->mode2 = 0;
		d1 = sub_24b52(obj);
		// redundant btst
		obj->HitBoxList = hitboxlist_25958;
		obj->Pool = 2;
		obj->Scroll = 0;
		obj->Step = obj->Flip;
		setaction_direct(obj, action_);
		// userdata ...
		
		
		
		break;
	case 2:
		// todo
	
	
	}
}

static void sub_26122(Player *ply) {
	if (ply->YPI != 40) {
		if (ply->Side) {
			g.Ply2Shadow.flag1 = FALSE;
		} else {
			g.Ply1Shadow.flag1 = FALSE;
		}
	}
}

static int _check_platform(Player *plya3, Object *obj) {		//26096
	ply->PlatformFallDir = 0;		//005b
	if (2*(0x48 + plya3->Size) <= (plya3->XPI - obj->XPI) + 0x48 + plya3->Size) {
		if (plya3->XPI - obj->XPI < 0) {
			d2 = (plya3->XPI - obj->XPI) + (plya3->Size + 0x48);
		} else {
			d2 = (plya3->XPI - obj->XPI) - (plya3->Size + 0x48);
		}
		if (ABS(d2)) {
			sub_26122(plya3);
		}
		if ((obj->YPI + 48) - plya3->YPI >= 0) {
			if ((ABS(d2) <= (obj->YPI + 48) - plya3->YPI) &&
				((obj->YPI + 48) - plya3->YPI) <= 0x20)   &&
				plya3->DeltaY.full == 0 ) {
					plya3->YPI = obj->YPI + 0x30;
					plya3->OnPlatform = TRUE;
					return 1;
			} else {
				ply->XPI += d2;
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


static void sub_24f06(Object *obj) {		// drunk, probably wrong
	switch (ud->h0090c) {
		case 0:
			if (_check_platform(&g.Player1, obj)) {
				ud->h0090c = 1;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x2;
				}
			}
			else if (_check_platform(&g.Player2, obj)) {
				ud->h0090c = 2;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x2;
				}
			}
			break;
		case 1:
			if (_check_platform(&g.Player2, obj)) {
				ud->h0090c = 3;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x3;
				}
			} else if (_check_platform(&g.Player1, obj)) {
				ud->h0090c = 0;
				if (nobj = AllocActor()) {
					nobj->exists = TRUE;
					nobj->Sel = 0x32;
					nobj->SubSel = 0x5;
				}
			}				
			break;
		case 2:
			
					   
		default:
			break;
	}
}