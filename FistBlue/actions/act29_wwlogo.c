/*
 *  act29_wwlogo.c
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

#include "act29_wwlogo.h"


typedef struct UserData_29 * UD;

void action_29(Object *obj) {
	UD ud = (UD)&obj->UserData;
	Object *nobj;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Scroll = SCROLL_NONE;
			obj->Pool   = 4;
			obj->XPI    = (SCREEN_WIDTH / 2);
			obj->YPI    = 80;
			obj->LocalTimer = 0;
			ud->funky   = 0x00b00000;
			ud->XSave = obj->XPI;
			if (obj->SubSel) {
				NEXT(obj->mode1);
			}
            RHSetAction(obj, RHCODE(0x20f76));
			/* FALLTHRU */
		case 2:
			switch (obj->mode1) {
				case 0:
					obj->XPI = ud->XSave;
					ud->funky -= 0x25000;
					if (ud->funky < 0) {
						NEXT(obj->mode1);
						if ((nobj=AllocActor())) {
							nobj->exists = TRUE;
							nobj->Sel    = 0x3e;
						}
					} else {
						++obj->LocalTimer;
						if (obj->LocalTimer & 1) {
							obj->X.full += ud->funky;
						} else {
							obj->X.full -= ud->funky;
						}
					}
					break;
				case 2:
					/* does nothing */
					break;
				FATALDEFAULT;
			}
			enqueue_and_layer(obj);			//??
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}