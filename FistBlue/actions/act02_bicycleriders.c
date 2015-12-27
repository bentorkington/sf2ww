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


const Image image_d714 = {
    2, 0x0000, 0x32, 0, 0,
    { 0x84a3, 0x84b3,  }
};
const CAFrame actlist_d6f8[] = {
	{ 8, 0x80, 0x0, &image_d714, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_d6f8[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_d73e = {
    2, 0x0000, 0x32, 0, 0,
    { 0x84a0, 
		0x84b0,  }
};
const CAFrame actlist_d722[] = {
	{ 8, 0x80, 0x0, &image_d73e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_d722[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_d768 = {
    2, 0x0000, 0x32, 0, 0,
    { 0x84a1, 0x84b1,  }
};
const CAFrame actlist_d74c[] = {
	{ 8, 0x80, 0x0, &image_d768, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_d74c[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_d792 = {
    2, 0x0000, 0x32, 0, 0,
    { 0x84a2, 0x84b2,  }
};
const CAFrame actlist_d776[] = {
	{ 8, 0x80, 0x0, &image_d792, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_d776[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_d7bc = {
    2, 0x0000, 0x32, 0, 0,
    { 0x84c0, 0x84d0,  }
};
const CAFrame actlist_d7a0[] = {
	{ 8, 0x80, 0x0, &image_d7bc, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_d7a0[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_d7e6 = {
    2, 0x0000, 0x32, 0, 0,
    { 0x84b5, 
		0x84c5,  }
};
const CAFrame actlist_d7ca[] = {
	{ 8, 0x80, 0x0, &image_d7e6, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_d7ca[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_d810 = {
    2, 0x0000, 0x32, 0, 0,
    { 0x84b6, 0x84c6,  }
};
const CAFrame actlist_d7f4[] = {
	{ 8, 0x80, 0x0, &image_d810, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_d7f4[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
/* image d714 */

const CAFrame *actlist_d6e8[]={
	actlist_d6f8, 	actlist_d722, 	actlist_d74c, 	actlist_d776, 	
	actlist_d7a0, 	actlist_d7ca, 	actlist_d7f4, 	actlist_d6f8, 
};

const Image image_d4cc = {
    26, 0x0000, 0xb, 0, 0,
    { 0x0000, 0x0000, 0x0000, 0x84e0, 0x84f0, 0x0000, 0x84c1, 0x84d1, 0x84e1, 0x84f1, 0x0000, 0x84c2, 0x84d2, 0x84e2, 0x84f2, 0x0000, 0x84c3, 0x84d3, 0x84e3, 0x84f3, 0x84b4, 0x84c4, 0x84d4, 0x84e4, 0x84f4, 0x0000,  }
};
const Image image_d526 = {
    26, 0x0000, 0xb, 0, 0,
    { 0x0000, 0x0000, 0x0000, 0x8409, 0x8419, 0x0000, 0x84c1, 0x84d1, 0x840a, 0x841a, 0x0000, 0x84c2, 0x8406, 0x840b, 0x841b, 0x0000, 0x84c3, 0x8407, 0x840c, 0x841c, 0x84b4, 0x84c4, 0x8408, 0x840d, 0x841d, 0x0000,  }
};
const Image image_d580 = {
    26, 0x0000, 0xb, 0, 0,
    { 0x0000, 0x0000, 0x0000, 0x8429, 0x8439, 0x0000, 0x84c1, 0x84d1, 0x842a, 0x843a, 0x0000, 0x84c2, 0x8416, 0x842b, 0x843b, 0x0000, 0x84c3, 0x8417, 0x842c, 0x843c, 0x84b4, 0x84c4, 0x8418, 0x842d, 0x843d, 0x0000,  }
};
const Image image_d5da = {
    26, 0x0000, 0xb, 0, 0,
    { 0x0000, 0x0000, 0x0000, 0x84e0, 0x84f0, 0x0000, 0x84c1, 0x84d1, 0x84e1, 0x84f1, 0x0000, 0x84c2, 0x8426, 0x8436, 0x84f2, 0x0000, 0x84c3, 0x8427, 0x8437, 0x8447, 0x84b4, 0x84c4, 0x8428, 0x84e4, 0x8448, 0x0000,  }
};
const Image image_d634 = {
    26, 0x0000, 0xb, 0, 0,
    { 0x0000, 0x0000, 0x0000, 0x8409, 0x8419, 0x0000, 0x84c1, 0x84d1, 0x840a, 0x841a, 0x0000, 0x84c2, 0x8456, 0x8466, 0x8476, 0x0000, 0x84c3, 0x8457, 0x8467, 0x8477, 0x84b4, 0x84c4, 0x8458, 0x8468, 0x841d, 0x0000,  }
};
const Image image_d68e = {
    26, 0x0000, 0xb, 0, 0,
    { 0x0000, 0x0000, 0x0000, 0x8429, 0x8439, 0x0000, 0x84c1, 0x84d1, 0x842a, 0x843a, 0x0000, 0x84c2, 0x8485, 0x8495, 0x84a5, 0x0000, 0x84c3, 0x8486, 0x8496, 0x84a6, 0x84b4, 0x84c4, 0x8487, 0x842d, 0x843d, 0x0000,  }
};
const CAFrame actlist_d438[7] = {
	{ 8, 0x00, 0x0, &image_d4cc, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0}, /* LOOP BEGIN */ 
	{ 8, 0x00, 0x0, &image_d526, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x00, 0x0, &image_d580, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x00, 0x0, &image_d5da, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x00, 0x0, &image_d634, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 8, 0x80, 0x0, &image_d68e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_d438[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};



static void _create_bycycle(Object *obj, Object *nobj1, Object *nobj2) {		// d2ba
	struct UserData_Act2 *ud   = (struct UserData_Act2 *)&obj->UserData;
	struct UserData_Act2 *udn1 = (struct UserData_Act2 *)&nobj1->UserData;
	
	short temp;
	const static short data_d326[] = { 29,30,31,29,30,31,29,30 };	// palettes
	
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
	
	temp = data_d326[(sf2rand() & 0xe) >>1];
	nobj1->Draw2.part.integer = temp;
	nobj2->Draw2.part.integer = temp;
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
	
	//const short data_d3ae[]={-0x200, 0,  0x200, 0};
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
//				obj->XPI   += obj->Step ? 464 : -80;
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
                    printf("bicycle %d X1:%04d ", g_d7, obj->X.part.integer);
					update_motion(obj);
                    printf("X2:%04d\n", obj->X.part.integer);
					_update_bicycle_shadow(obj);
					actiontick(obj);
				}
				check_rect_queue_draw(obj);
				break;
			case 4:
			case 6:		//object went offscreen
                printf("bicycle %d die\n", g_d7);
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
					obj->LocalTimer = (u16 []){60,300,300,420,420,420,420,300}[(RAND32 & 0xe) >> 1];
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