/*
 *  act17.c
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
#include "gstate.h"

#include "act17.h"


extern Game g;
extern int g_d7;

typedef struct UserData_17 * UD;

void action_17(Object *obj) {
	UD ud = (UD)&obj->UserData;
		
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			//ud->h0082c = 0;
			if (0x9249 & (1 << RAND16) == 0) {
				obj->Pool = 6;
			}
			//setaction_list(obj, data_10a72, obj->SubSel);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					//obj->VelX = data_[RAND8W];
					//obj->VelY = data_[RAND8W];
					//obj->XPI += data_[RAND16W];
					//obj->YPI += data_[RAND8W];
					//ud->h0080w += 0x28;
					obj->AclY.full = 0x0040;
					obj->AclX.full = 0;
					obj->LocalTimer = 0x32;
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode1);
						//ud->h0082c = 0x1e;
					}
					CATrajectory(obj);
					if (obj->VelY.full < 0) {
						/* XXX oh god! */
					}
				case 4:
					//todo
					
					
					break;
					FATALDEFAULT;
			}
			//if ((ud->h0085c & 1) == 0) {
			//	check_rect_queue_draw(obj);
			//}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			FATALDEFAULT;
	}
	
}

