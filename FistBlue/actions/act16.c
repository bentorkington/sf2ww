/*
 *  act16.c
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
#include "gfxlib.h"
#include "act16.h"


extern Game g;
extern int g_d7;


static int sub_12890(Object *obj) {
	if (obj->UserByte & 1) {
		return (g.x8a60[obj->UserByte / 2] & 0xf);
	} else {
		return ((g.x8a60[obj->UserByte / 2] & 0xf0) >> 4);
	}
}


void action_16(Object *obj) {
	switch (obj->SubSel) {
		case 0:
        case 1:                     // conveyor belts
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->Pool = 2;
                    RHSetScrollActionList(obj, RHCODE(0x128d2), obj->SubSel);
					break;
				case 2:
					actiontickdraw(obj);
					break;
					FATALDEFAULT;
			}
			break;
		case 2:                 // trampoline for barrels to bounce off
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->Pool = 4;
					g.x8a64[0] = 0;
                    RHSetAction(obj, RHCODE(0x12b70));
					check_rect_queue_draw(obj);
					break;
				case 2:
					if (g.x8a64[0]) {
						g.x8a64[0] = 0;
                        RHSetAction(obj, RHCODE(0x12b70));
					}
					actiontick(obj);
					check_rect_queue_draw(obj);
					break;
					FATALDEFAULT;
			}
			break;
		case 3:             // barrel score counter each P1 & P2
			if (obj->mode0 == 0) {
				NEXT(obj->mode0);
				obj->Pool = 6;
				g.x8a60[0] = g.x8a60[1] = 0;
				g.x8a62[0] = 0;
				obj->Draw1 = TRUE;
				obj->Draw2.part.integer = 0x1d;
				obj->XPI = (short []){0x30, 0x50, 0x138, 0x158}[obj->UserByte];
				obj->YPI = 0xa4;
                RHSetActionList(obj, RHCODE(0x12c60), sub_12890(obj));
			} else {
				obj->Draw2.part.integer = 0x1d;
				if (g.x8a62[obj->UserByte / 2]) {
					--g.x8a62[obj->UserByte / 2];
					if ((g.libsplatter & 2) == 0) {
						obj->Draw2.part.integer = 6;
					}
				}
                RHSetActionList(obj, RHCODE(0x12c60), sub_12890(obj));
				check_rect_queue_draw(obj);
			}
			break;
		case 4:         // P2 score background (P1 is burned into scroll tiles)
			if (obj->mode0 == 0) {
				NEXT(obj->mode0);
				obj->Pool = 6;
                RHSetAction(obj, RHCODE(0x12e90));
			}
			check_rect_queue_draw(obj);
			break;
			
			FATALDEFAULT;
	}
}


