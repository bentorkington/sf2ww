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

const Image image_21014 = {
    8, 0x0700, 0x4c, 0, 0,
    { 0x8140,  }
};
const CAFrame actlist_20fdc[] = {
	{ 4, 0x80, 0x0, &image_21014, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_20fdc[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_21020 = {
    16, 0x0000, 0x7b, 0, 0,
    { 0x8140, 0x8141, 0x8142, 0x8143, 0x8144, 0x8145, 0x8146, 
		0x8147, 0x8150, 0x8151, 0x8152, 0x8153, 0x8154, 0x8155, 0x8156, 
		0x8157,  }
};
const CAFrame actlist_20ff8[] = {
	{ 4, 0x80, 0x0, &image_21020, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_20ff8[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const CAFrame *actlist_20fd8[]={
	actlist_20fdc, 	actlist_20ff8, 
};



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