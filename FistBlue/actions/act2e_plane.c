/*
 *  act2e_plane.c
 *  MT2
 *
 *  Created by Ben on 5/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */
#include <stdio.h>

#include "sf2.h"

#include "particle.h"
#include "sound.h"
#include "structs.h"
#include "lib.h"
#include "act2e_plane.h"

typedef struct UserData_Act2e UD2E;
extern Game g;

void synth_plane_setup(Object *obj, int city_from, int city_to) {
	UD2E *ud = (UD2E *)obj->UserData;
	ud->city_from = city_from;
	ud->city_to   = city_to;
}


const Image image_1a094 = {
    2, 0x000d, 0x2a, 8, 8,
    { 0x0c78, 0x0c79,  }
};
const CAFrame actlist_19fb4[] = {
	{65535, 0x80, 0x0, &image_1a094, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19fb4[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a0a2 = {
    1, 0x000d, 0x2a, 8, 8,
    { 0x0000, 0x0c05,  }
};
const CAFrame actlist_19fd0[] = {
	{65535, 0x80, 0x0, &image_1a0a2, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19fd0[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a0b0 = {
    1, 0x000d, 0x2a, 8, 8,
    { 0x0000, 0x0c3a,  }
};
const CAFrame actlist_19fec[] = {
	{65535, 0x80, 0x0, &image_1a0b0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19fec[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a0be = {
    2, 0x000d, 0x2a, 8, 8,
    { 0x0d00, 0x0d01, 
	}
};
const CAFrame actlist_1a008[] = {
	{65535, 0x80, 0x0, &image_1a0be, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a008[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a0cc = {
    2+IMAGE_ATTR, 0x000d, 0x2a, 8, 8,
    { 0x0c78, 0x004d,  }
};
const CAFrame actlist_1a024[] = {
	{65535, 0x80, 0x0, &image_1a0cc, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a024[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a0de = {
    1, 0x000d, 0x2a, 8, 8,
    { 0x0000, 0x0c8c,  }
};
const CAFrame actlist_1a040[] = {
	{65535, 0x80, 0x0, &image_1a0de, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a040[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a0ec = {
    1+IMAGE_ATTR, 0x000d, 0x2a, 8, 8,
    { 0x0000, 0x0000, 0x0c3a,  }
};
const CAFrame actlist_1a05c[] = {
	{65535, 0x80, 0x0, &image_1a0ec, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a05c[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a0fe = {
    1, 0x002d, 0x2a, 8, 8,
    { 0x0000, 
		0x0c05,  }
};
const CAFrame actlist_1a078[] = {
	{65535, 0x80, 0x0, &image_1a0fe, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a078[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

const CAFrame *actlist_19fa2[]={
	actlist_19fb4, 	actlist_19fd0, 	actlist_19fec, 	actlist_1a008, 	actlist_1a024, 	actlist_1a040, 	actlist_1a05c, 	actlist_1a078, 
};

// sin and cos for 64 angles, with an aspect ratio of 1.143:1
const VECT16 data_cfe74[64] = {
	// 0 degrees
	{ 0x0000, 0x02a0, }, { 0x004b, 0x029c, }, { 0x0095, 0x0292, }, { 0x00de, 0x0282, },
	{ 0x0125, 0x026c, }, { 0x0169, 0x024f, }, { 0x01aa, 0x022e, }, { 0x01e6, 0x0207, },	
	{ 0x021e, 0x01da, }, { 0x0251, 0x01a9, }, { 0x027e, 0x0174, }, { 0x02a4, 0x013c, },
	{ 0x02c4, 0x0100, }, { 0x02de, 0x00c2, }, { 0x02f0, 0x0082, }, { 0x02fc, 0x0041, },
	// 90 degrees
	{ 0x0300, 0x0000, }, { 0x02fc, 0xffbf, }, { 0x02f0, 0xff7e, }, { 0x02de, 0xff3e, },
	{ 0x02c4, 0xff00, },
	{ 0x02a4, 0xfec4, },
	{ 0x027e, 0xfe8c, },
	{ 0x0251, 0xfe57, },
	{ 0x021e, 0xfe26, },//18
	{ 0x01e6, 0xfdf9, },
	{ 0x01aa, 0xfdd2, },
	{ 0x0169, 0xfdb1, },
	{ 0x0125, 0xfd94, },
	{ 0x00de, 0xfd7e, },
	{ 0x0095, 0xfd6e, },
	{ 0x004b, 0xfd64, },
	{ 0x0000, 0xfd60, },//20
	{ 0xffb5, 0xfd64, },
	{ 0xff6b, 0xfd6e, },
	{ 0xff22, 0xfd7e, },
	{ 0xfedb, 0xfd94, },
	{ 0xfe97, 0xfdb1, },
	{ 0xfe56, 0xfdd2, },
	{ 0xfe1a, 0xfdf9, },
	{ 0xfde2, 0xfe26, },//28
	{ 0xfdaf, 0xfe57, },
	{ 0xfd82, 0xfe8c, },
	{ 0xfd5c, 0xfec4, },
	{ 0xfd3c, 0xff00, },
	{ 0xfd22, 0xff3e, },
	{ 0xfd10, 0xff7e, },
	{ 0xfd04, 0xffbf, },
	{ 0xfd00, 0x0000, },//30
	{ 0xfd04, 0x0041, },
	{ 0xfd10, 0x0082, },
	{ 0xfd22, 0x00c2, },
	{ 0xfd3c, 0x0100, },
	{ 0xfd5c, 0x013c, },
	{ 0xfd82, 0x0174, },
	{ 0xfdaf, 0x01a9, },
	{ 0xfde2, 0x01da, },//38
	{ 0xfe1a, 0x0207, },
	{ 0xfe56, 0x022e, },
	{ 0xfe97, 0x024f, },
	{ 0xfedb, 0x026c, },
	{ 0xff22, 0x0282, },
	{ 0xff6b, 0x0292, },
	{ 0xffb5, 0x029c, },
};



/*!
 action_2e
 sf2ua: 0x18f92
 description: plane on world map
 */

void action_2e(Object *obj) {
	UD2E *ud = (UD2E *)obj->UserData;
	const static POINT16 city_coords[12] = {		// 19040
		{ 0x00be, 0x00ab, }, { 0x00c4, 0x00b3, }, { 0x012c, 0x008c, }, { 0x0117, 0x00ac, },
		{ 0x0112, 0x00bb, }, { 0x00ae, 0x00b6, }, { 0x0088, 0x00bf, }, { 0x0092, 0x00a3, },
		{ 0x00a0, 0x00a0, }, { 0x00a4, 0x00a4, }, { 0x0103, 0x00b0, }, { 0x004e, 0x00b6, },
	};
	
	int d6;
	switch (obj->mode0) {
		case 0:
			switch (obj->mode1) {
				case 0:					
					NEXT(obj->mode1);
					obj->LocalTimer = 0x32;
					obj->Pool = 2;
					obj->Path = data_cfe74;
										
					if (ud->city_from == ud->city_to) {				// 1901e
						g.PlaneLandedInCity[ud->city_to] = TRUE;
						obj->mode0 = 6;						// die
						obj->mode1 = 0;
						g.Pause_9e1 = -1;
					} else {
						//18fd8
						obj->XPI = city_coords[ud->city_from].x;
						obj->YPI = city_coords[ud->city_from].y;
						ud->destination.x = city_coords[ud->city_to].x;
						ud->destination.y = city_coords[ud->city_to].y;
						d6 = calc_flightpath(obj, ud->destination.x, ud->destination.y);
						
						obj->Step = (d6 + 2) / 4;
                        RHSetActionList(obj, RHCODE(0x19fa2), (obj->Step+1) >> 3);
					}
					break;
				case 2:
					if (--obj->LocalTimer == 0) {
						NEXT(obj->mode0);
						obj->mode1 = 0;
						queuesound(SOUND_PLANE);
					}
					break;
					FATALDEFAULT;
			}
			break;
		case 2:
			//190a0
			if (obj->mode1 == 0) {
				d6 = calc_flightpath(obj, ud->destination.x, ud->destination.y);
				obj->Step = (d6 + 2) / 4;
				
				if ((ABS(obj->XPI - ud->destination.x) > 3)   ||	
					(ABS(obj->YPI - ud->destination.y) > 3)) {
					update_motion(obj);
					enqueue_and_layer(obj);
				} else {
					NEXT(obj->mode1);	// flight over
					g.Pause_9e1 = -1;
					g.PlaneLandedInCity[ud->city_to] = TRUE;
					ud->sound = (short []) {
						SOUND_JAPAN,	SOUND_JAPAN,	SOUND_BRAZIL,	SOUND_USA,
						SOUND_USA,		SOUND_CHINA,	SOUND_USSR,		SOUND_INDIA,
						SOUND_THAILAND,	SOUND_THAILAND, SOUND_USA,		SOUND_SPAIN,
					}[ud->city_to];
					obj->LocalTimer = 0x32;
					enqueue_and_layer(obj);
				}
			} else {
				if (--obj->LocalTimer == 0) {
					queuesound(ud->sound);
				}
				enqueue_and_layer(obj);
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
			FATALDEFAULT;
	}
}

