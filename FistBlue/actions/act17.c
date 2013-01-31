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

typedef struct UserData_17 *UD;
/*
static void sub_10a3e(Object *obj) {
	const static short data_10a5e[] = {
		0x240, 0x260, 0x200, 0x220, 0x210, 0x270, 0x250, 0x230,
	};
	
	if (obj->VelY.full <<>> ud->h0080w) {
		obj->VelY.full = data_[RAND8WD];
		obj->YPI       = ud->h0080w;
	}
}

void action_17(Object *obj) {
	UD *ud = (UD *)&obj->UserData;
		
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			ud->h0082c = 0;
			if (0x9249 & (1 << RAND16) == 0) {
				obj->Pool = 6;
			}
			//XXX setaction_list(obj, data_10a72, obj->SubSel);
			break;
		case 2:
			switch (obj->mode1) {
				case 0:
					NEXT(obj->mode1);
					//obj->VelX = data_109a0[RAND8WD];
					//obj->VelY = data_109b0[RAND8WD];
					//obj->XPI += data_109c0[RAND8WD];
					//obj->YPI += data_109d0[RAND16WD];
					//ud->h0080w = data_109f0[RAND8WD];
					ud->h0080w += 0x28;
					obj->AclY.full  = 0x0040;
					obj->AclX.full  = 0;
					obj->LocalTimer = 0x32;
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode1);
						ud->h0082c = 0x1e;
					}
					CATrajectory(obj);
					if (obj->VelY.full < 0) {
						sub_10a3e(obj);
					}
				case 4:
					if (--ud->h0082c == 0) {
						NEXT(obj->mode0);
						obj->mode1 = 0;
					}
					CATrajectory(obj);
					if (obj->VelY.full < 0) {
						sub_10a3e(obj);
					}
					break;
					FATALDEFAULT;
			}
			if ((ud->h0085c & 1) == 0) {
				check_rect_queue_draw(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			FATALDEFAULT;
	}
	
}

*/