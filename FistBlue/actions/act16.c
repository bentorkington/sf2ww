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



const SimpleImage image_12924 = {
    11, 2, 1,
    { 0x10e7, 0x0020, 0x10c8, 0x0000, 0x10e6, 0x0020, 0x10c9, 0x0000, 0x10e5, 0x0020, 0x10ca, 0x0000, 0x10e7, 0x0020, 0x10cb, 0x0000, 0x10e6, 0x0020, 0x10cc, 0x0000, 0x10e5, 0x0020, 0x10cd, 0x0000, 0x10e7, 0x0020, 0x10ce, 0x0000, 0x10e6, 0x0020, 0x10cf, 0x0000, 0x10e5, 0x0020, 0x10e8, 0x0000, 0x10e1, 0x0000, 0x10e9, 0x0000, 0x10e2, 0x0000, 0x10ea, 0x0000,  }
};
const SimpleImage image_12982 = {
    11, 2, 1,
    { 0x1106, 0x0020, 0x10c8, 0x0000, 0x1105, 0x0020, 0x10c9, 0x0000, 0x1104, 0x0020, 0x10ca, 0x0000, 0x1106, 0x0020, 0x10cb, 0x0000, 0x1105, 0x0020, 0x10cc, 0x0000, 0x1104, 0x0020, 0x10cd, 0x0000, 0x1106, 0x0020, 0x10ce, 0x0000, 0x1105, 0x0020, 0x10cf, 0x0000, 0x1104, 0x0020, 0x10e8, 0x0000, 0x1100, 0x0000, 0x1108, 0x0000, 0x1101, 0x0000, 0x1109, 0x0000,  }
};
const SimpleImage image_129e0 = {
    11, 2, 1,
    { 0x1116, 0x0020, 0x10c8, 0x0000, 0x1115, 0x0020, 0x10c9, 0x0000, 0x1114, 0x0020, 0x10ca, 0x0000, 0x1116, 0x0020, 0x10cb, 0x0000, 0x1115, 0x0020, 0x10cc, 0x0000, 0x1114, 0x0020, 0x10cd, 0x0000, 0x1116, 0x0020, 0x10ce, 0x0000, 0x1115, 0x0020, 0x10cf, 0x0000, 0x1114, 0x0020, 0x10e8, 0x0000, 0x1110, 0x0000, 0x1118, 0x0000, 0x1111, 0x0000, 0x1119, 0x0000,  }
};
const CASimpleFrame actlist_128d8[] = {
	{ 8, 0x00, 0x0, &image_12924},
	{ 8, 0x00, 0x0, &image_12982},
	{ 8, 0x80, 0x0, &image_129e0},
	{0,0,0,(struct image2 *)&actlist_128d8[0] }
};
const SimpleImage image_12a3e = {
    11, 2, 0,
    { 0x10e3, 0x10eb, 0x10e4, 0x10ec, 0x10e5, 0x80ed, 0x10e6, 0x80ee, 0x10e7, 0x80ef, 0x10e5, 0x8120, 0x10e6, 0x8121, 0x10e7, 0x8122, 0x10e5, 0x8128, 0x10e6, 0x8128, 0x10e7, 0x8129,  }
};
const SimpleImage image_12a70 = {
    11, 2, 0,
    { 0x1102, 0x110a, 0x1103, 0x110b, 0x1104, 0x80ed, 0x1105, 0x80ee, 0x1106, 0x80ef, 0x1104, 0x8120, 0x1105, 0x8121, 0x1106, 0x8122, 0x1104, 0x8128, 0x1105, 0x8128, 0x1106, 0x8129,  }
};
const SimpleImage image_12aa2 = {
    11, 2, 0,
    { 0x1112, 0x111a, 0x1113, 0x111b, 0x1114, 0x80ed, 0x1115, 0x80ee, 0x1116, 0x80ef, 0x1114, 0x8120, 0x1115, 0x8121, 0x1116, 0x8122, 0x1114, 0x8128, 0x1115, 0x8128, 0x1116, 0x8129,  }
};
const CASimpleFrame actlist_128f4[] = {
	{ 8, 0x00, 0x0, &image_12a3e},
	{ 8, 0x00, 0x0, &image_12a70},
	{ 8, 0x80, 0x0, &image_12aa2},
	{0,0,0,(struct image2 *)&actlist_128f4[0] }
};
const SimpleImage image_12ad4 = {
    9, 2, 1,
    { 0x10f8, 0x0003, 0x0000, 0x0003, 0x10f9, 0x0003, 0x0000, 0x0003, 0x10fa, 0x0003, 0x0000, 0x0003, 0x10fb, 0x0003, 0x0000, 0x0003, 0x10fb, 0x0023, 0x0000, 0x0003, 0x10fa, 0x0023, 0x0000, 0x0003, 0x10f9, 0x0023, 0x0000, 0x0003, 0x113a, 0x0003, 0x0000, 0x0003, 0x113b, 0x0003, 0x0000, 0x0003,  }
};
const SimpleImage image_12b22 = {
    9, 2, 1,
    { 0x1123, 0x0003, 0x0000, 0x0003, 0x1124, 0x0003, 0x112c, 0x0003, 0x1125, 0x0003, 0x112d, 0x0003, 0x1126, 0x0003, 0x112e, 0x0003, 0x1126, 0x0023, 0x112e, 0x0023, 0x1125, 0x0023, 0x112d, 0x0023, 0x1124, 0x0023, 0x112c, 0x0023, 0x1138, 0x0003, 0x0000, 0x0003, 0x1139, 0x0003, 0x0000, 0x0003,  }
};
const CASimpleFrame actlist_12910[] = {
	{60, 0x00, 0x0, &image_12ad4},
	{ 8, 0x80, 0x0, &image_12b22},
	{0,0,0,(struct image2 *)&actlist_12910[0] }
};
const CASimpleFrame *actlist_128d2[]={
	actlist_128d8, 	actlist_128f4, 	actlist_12910, 
};



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
		case 1:
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
		case 2:
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
		case 3:
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
		case 4:
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


