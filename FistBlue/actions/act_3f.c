/*
 *  act_3f.c
 *  MT2
 *
 *  Created by Ben on 13/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */

#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"

#include "particle.h"
#include "sound.h"
#include "structs.h"
#include "lib.h"


#include "act_3f.h"

extern Game g;
typedef struct UserData_3f * UD;

const Image image_21066 = {
    5, 0x000d, 0x2d, 0, 0,
    { 0x8103, 0x8100, 0x8100, 0x8100, 0x8100,  }
};
const CAFrame actlist_2104a[] = {
	{ 4, 0x80, 0x0, &image_21066, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_2104a[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};



void action_3f(Object *obj) {
	UD ud=(UD)&obj->UserData;

	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			// todo obj->Path = data_cfe74;
			obj->XPI = 192;
			obj->YPI = 144;
			setaction_direct(obj, actlist_2104a);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					obj->LocalTimer = 40;
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode1);
						obj->LocalTimer = 6;
						if (obj->SubSel) {
							ud->XDest = 0x138;
						} else {
							ud->XDest = 0x4a;
						}
						ud->YDest = 0xe0;
						obj->Step = (calc_flightpath(obj, ud->XDest, ud->YDest)+2)/4;
					}
					break;
				case 4:
					if (obj->YPI >= 0xd0) {
						NEXT(obj->mode0);
						g.Pause_9e1 = 1;
						g.x0ae7 = 1;
						LBAddPoints(0x16, obj->SubSel);
					} else {
						obj->Step = (calc_flightpath(obj, ud->XDest, ud->YDest)+2)/4;
						update_motion(obj);
					}

					break;
				FATALDEFAULT;
			}
			enqueue_and_layer(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}