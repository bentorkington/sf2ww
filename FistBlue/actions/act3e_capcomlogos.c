/*
 *  act3e_capcomlogos.c
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
#include "gstate.h"

#include "act3e_capcomlogos.h"


extern Game g;
extern int g_d7;

void action_3e(Object *obj) {
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Scroll = SCROLL_NONE;
			obj->Pool   = 2;
			obj->XPI	= (SCREEN_WIDTH / 2);
			obj->YPI	= 32;
			if (g.Version == VERSION_USA) {
                RHSetActionList(obj, RHCODE(0x20fd8), 0);
			} else {
                RHSetActionList(obj, RHCODE(0x20fd8), 1);
			}
			/* FALLTHRU */
		case 2:
			enqueue_and_layer(obj);
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
		FATALDEFAULT;
	}

}