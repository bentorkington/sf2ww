/*
 *  act_3f.c
 *  MT2
 *
 *  Created by Ben on 13/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */

#include "sf2.h"

#include "particle.h"
#include "sound.h"
#include "structs.h"
#include "lib.h"


#include "act_3f.h"

extern Game g;
typedef struct UserData_3f * UD;

// Display the bonus completion score of 30000 in the center of display,
// then move it toward the winning player's score, and award them those points

void action_3f(Object *obj) {
	UD ud=(UD)&obj->UserData;
	extern const VECT16 data_cfe74[];
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Path = data_cfe74;
			obj->XPI = (SCREEN_WIDTH / 2);
			obj->YPI = 144;
            RHSetAction(obj, RHCODE(0x2104a));
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
							ud->XDest = 312;
						} else {
							ud->XDest =  74;
						}
						ud->YDest = 224;
						obj->Step = (calc_flightpath(obj, ud->XDest, ud->YDest)+2)/4;
					}
					break;
				case 4:
					if (obj->YPI >= 208) {
						NEXT(obj->mode0);
						g.Pause_9e1 = 1;
						g.x0ae7     = TRUE;
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