/*
 *  act29_wwlogo.c
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

#include "act29_wwlogo.h"


typedef struct UserData_29 * UD;

const Image image_20f92 = {
    20, 0x001e, 0x89, 0, 0,
    { 0x7796, 0x7796, 
		0x7796, 0x7796, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
		0x7bc8, 0x7bc9, 0x7bca, 0x7bcb, 0x7bcc, 0x7bcd, 0x7bce, 0x7bcf, 
		0x7bd8, 0x7bd9, 0x7bda, 0x7bdb, 0x7bdc, 0x7bcd, 0x7bce, 0x7bdf, 
	}
};
const CAFrame actlist_20f76[] = {
	{48, 0x80, 0x0, &image_20f92, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_20f76[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};


void action_29(Object *obj) {
	UD ud = (UD)&obj->UserData;
	
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Scroll = SCROLL_NONE;
			obj->Pool   = 4;
			obj->XPI    = 192;
			obj->YPI    = 80;
			obj->LocalTimer = 0;
			ud->funky   = 0x00b00000;
			ud->XSave = obj->XPI;
			if (obj->SubSel) {
				NEXT(obj->mode1);
			}
			setaction_direct(obj, actlist_20f76);
			/* FALLTHRU */
		case 2:
			switch (obj->mode1) {
				case 0:
					obj->XPI = ud->XSave;
					ud->funky -= 0x25000;
					if (obj->VelX.full < 0) {
						NEXT(obj->mode1);
						if (obj=AllocActor()) {
							obj->exists = TRUE;
							obj->Sel    = 0x3e;
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
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}
}