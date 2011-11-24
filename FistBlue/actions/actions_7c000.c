/*
 *  actions_7c000.c
 *  GLUTBasics
 *
 *  Created by Ben on 31/12/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"


#include "structs.h"
#include "player.h"

#include "particle.h"
#include "lib.h"
#include "gfxlib.h"
#include "actions.h"
#include "actions_7c000.h"

extern struct game g;

static void sub_7cdf8(void);


static void sub_7cd3c(Object *obj) {
	switch (obj->mode2) {
		case 0:
			NEXT(obj->mode2);
			obj->LocalTimer = 8;
			obj->x001f = 2;
			sub_7cd84();
			sub_58c0(obj->UserData[0]);		/* byte */
			break;
		case 2:
			if (--obj->LocalTimer == 0) {
				obj->LocalTimer = 8;
				if (--obj->x001f < 0) {
					NEXT(obj->mode0);
				} else {
					sub_7cd84();
				}
			}
			break;
		FATALDEFAULT;
	}
}

static void sub_7c90a(Object *obj) {
	Player *ply;
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			ud->x0082 = -1;			/* word */
			action_162c6();
			action_16340();
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					if(--obj->LocalTimer == 0) {
						action_162c6();
						action_16340();
					} else {
						ply=ud->x0088;
						if (ply->x02a5) {
							NEXT(obj->mode1);
							sub_7cdf8();
							action_1631e();
						}
					}
					break;
				case 2:
					if (g.Pause_9e1) {
						NEXT(obj->mode1);
					}
					break;
				case 4:
					sub_7cd3c(obj);
					break;
				default:
					break;
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		default:
			break;
	}

}




static void sub_7cdf8(void) {
	Object *obj;
	if (obj=AllocActor()) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_0X31;
		obj->SubSel = 0;
	}
}

static void sub_7ce12(void) {
	Object *obj;
	if (obj =AllocActor()) {
		obj->exists = TRUE;
		obj->Sel = SF2ACT_0X31;
		obj->SubSel = 1;
	}
}
