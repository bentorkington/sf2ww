/*
 *  act02_bicycleriders.c
 *  MT2
 *
 *  Created by Ben on 5/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */

#include "sf2.h"

#include "particle.h"
#include "sound.h"
#include "structs.h"
#include "lib.h"
#include "gstate.h"

#include "act02_bicycleriders.h"

extern Game g;
extern int g_d7;
extern GState gstate_Scroll2;

static void _create_bycycle(Object *obj, Object *nobj1, Object *nobj2) {		// d2ba
	struct UserData_Act2 *ud   = (struct UserData_Act2 *)&obj->UserData;
	struct UserData_Act2 *udn1 = (struct UserData_Act2 *)&nobj1->UserData;
		
	ud->x0080++;
	nobj1->exists = TRUE;
	nobj1->Sel    = SF2ACT_0X02;
	nobj1->SubSel = 1;
	nobj1->Owner  = (Player *)obj;
	udn1->x0082   = nobj2;		/* compat: short pointer on CPS to pointer */
	
	nobj2->exists = TRUE;
	nobj2->Sel    = SF2ACT_0X02;
	nobj2->SubSel = -1;				// shadow
	nobj2->Owner  = (Player *)obj;
	
	if (sf2rand() & 1) {
		nobj1->Step = 1;
		nobj2->Step = 1;
	}
	nobj1->Draw1 = TRUE;
	nobj2->Draw1 = TRUE;
	
    short palette = RHWordOffset(0xd326, RAND8WD);

    nobj1->Draw2.part.integer = palette;
	nobj2->Draw2.part.integer = palette;
}
static void _update_bicycle_shadow(Object *obj) {				// d3de
	struct UserData_Act2 *ud = (struct UserData_Act2 *)&obj->UserData;
	
	if (obj->SubSel >= 0) {
		ud->x0082->XPI = obj->XPI-8;
		if (obj->Step) {
			ud->x0082->XPI += 16;
		}
		ud->x0082->YPI = obj->YPI + 64;
	}
}

void action_2(Object *obj) {				//d240 Bicycle people
	Object *nobj1, *nobj2;
	struct UserData_Act2 *ud = (struct UserData_Act2 *)&obj->UserData;
	
    const VECT16 data_d3ae[] = {
        {-0x200, 0}, {0x200, 0}
    };
	    
	if(obj->SubSel) {
		/* d33a */
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				obj->Pool   = 6;
				obj->Scroll = SCROLL_2;
				obj->ZDepth = 64;	
				obj->Flip   = obj->Step;
				obj->XPI    = gstate_Scroll2.XPI;
				obj->XPI   += obj->Step ? -80 : 464;
				obj->YPI    = 64;
				obj->Path   = (const VECT16 *)data_d3ae;
				if (obj->SubSel < 0) {
                    RHSetActionList(obj, RHCODE(0xd6e8), RAND8);
				} else {		// shadow
                    RHSetAction(obj, RHCODE(0xd438));
				}
				break;
			case 2:
				/* d3b6 */
				if(obj->SubSel >= 0) {
					if(((g.libsplatter + g_d7) & 7) == 0) {
						die_if_offscreen(obj);	/* check if still on screen */
					}
					update_motion(obj);
					_update_bicycle_shadow(obj);
					actiontick(obj);
				}
				check_rect_queue_draw(obj);
				break;
			case 4:
			case 6:		//object went offscreen
				/* d412 */
				if (obj->SubSel >= 0) {
					struct UserData_Act2 *udowner = (struct UserData_Act2 *)&obj->Owner->UserData;
					--udowner->x0080;
					ud->x0082->mode0 = 6;
				}
				FreeActor(obj);
				break;
				
				FATALDEFAULT;
		}		
	} else {		/* I am the controller object */
		switch (obj->mode0) {
			case 0:
				NEXT(obj->mode0);
				ud->x0080 = 0;
				break;
			case 2:
				/* d266 */
				if(obj->mode1) {
					/* d292 */
					if(--obj->LocalTimer == 0) {
						if (ud->x0080 < 4 && g.FreeLayer3 >= 2) {
							nobj1 = AllocActor();
							nobj2 = AllocActor();
							_create_bycycle(obj,nobj1, nobj2);
						}
						obj->mode1 = 0;
					}
				} else {
					NEXT(obj->mode1);
					obj->LocalTimer = (u16 []){60,300,300,420,420,420,420,300}[RAND8W];
				}
				break;
			case 4:
			case 6:
				FreeActor(obj);
				break;
				FATALDEFAULT;
		}
	}
}