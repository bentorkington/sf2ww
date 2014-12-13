/*
 *  act1e_selcursor.c
 *  MT2
 *
 *  Created by Ben on 5/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */
#include "sf2.h"

#include "particle.h"
#include "sound.h"
#include "structs.h"
#include "lib.h"

#include "act1e_worldflags.h"

extern Game g;
typedef struct UserData_Act1e UD1E;

const Image image_19212 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 0x836c, 0x001f, 0x837c, 0x001f, 
		0x835d, 0x001f, 0x836d, 0x001f, 0x837c, 0x003f, 0x7ec2, 0x001d, 
		0x7ed2, 0x001d, 0x7ec3, 0x001d, 0x7ed3, 0x001d,  }
};
const Image image_19244 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 
		0x836c, 0x001f, 0x837c, 0x001f, 0x835d, 0x001f, 0x836d, 0x001f, 
		0x837c, 0x003f, 0x7ec2, 0x001e, 0x7ed2, 0x001e, 0x7ec3, 0x001e, 
		0x7ed3, 0x001e,  }
};
const CAFrame actlist_191de[] = {
	{10, 0x00, 0x0, &image_19212, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19244, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_191de[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
/* image 19212 */
/* image 19244 */
const Image image_193d4 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 0x836c, 0x001f, 0x837c, 0x001f, 
		0x835d, 0x001f, 0x836d, 0x001f, 0x837c, 0x003f, 0x7ec4, 0x001d, 
		0x7ed4, 0x001d, 0x7ec5, 0x001d, 0x7ed5, 0x001d,  }
};
const Image image_19406 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 
		0x836c, 0x001f, 0x837c, 0x001f, 0x835d, 0x001f, 0x836d, 0x001f, 
		0x837c, 0x003f, 0x7ec4, 0x001e, 0x7ed4, 0x001e, 0x7ec5, 0x001e, 
		0x7ed5, 0x001e,  }
};
const CAFrame actlist_193a0[] = {
	{10, 0x00, 0x0, &image_193d4, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19406, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_193a0[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19596 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 0x836c, 0x001f, 0x837c, 0x001f, 
		0x835d, 0x001f, 0x836d, 0x001f, 0x837c, 0x003f, 0x7ee2, 0x001d, 
		0x7ef2, 0x001d, 0x7ee3, 0x001d, 0x7ef3, 0x001d,  }
};
const Image image_195c8 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 
		0x836c, 0x001f, 0x837c, 0x001f, 0x835d, 0x001f, 0x836d, 0x001f, 
		0x837c, 0x003f, 0x7ee2, 0x001e, 0x7ef2, 0x001e, 0x7ee3, 0x001e, 
		0x7ef3, 0x001e,  }
};
const CAFrame actlist_19562[] = {
	{10, 0x00, 0x0, &image_19596, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_195c8, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19562[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
/* image 19596 */
/* image 195c8 */
const Image image_19758 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 0x836c, 0x001f, 0x837c, 0x001f, 
		0x835d, 0x001f, 0x836d, 0x001f, 0x837c, 0x003f, 0x7ee6, 0x001d, 
		0x7ef6, 0x001d, 0x7ee7, 0x001d, 0x7ef7, 0x001d,  }
};
const Image image_1978a = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 
		0x836c, 0x001f, 0x837c, 0x001f, 0x835d, 0x001f, 0x836d, 0x001f, 
		0x837c, 0x003f, 0x7ee6, 0x001e, 0x7ef6, 0x001e, 0x7ee7, 0x001e, 
		0x7ef7, 0x001e,  }
};
const CAFrame actlist_19724[] = {
	{10, 0x00, 0x0, &image_19758, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_1978a, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19724[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1991a = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 0x836c, 0x001f, 0x837c, 0x001f, 
		0x835d, 0x001f, 0x836d, 0x001f, 0x837c, 0x003f, 0x7ee4, 0x001d, 
		0x7ef4, 0x001d, 0x7ee5, 0x001d, 0x7ef5, 0x001d,  }
};
const Image image_1994c = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 
		0x836c, 0x001f, 0x837c, 0x001f, 0x835d, 0x001f, 0x836d, 0x001f, 
		0x837c, 0x003f, 0x7ee4, 0x001e, 0x7ef4, 0x001e, 0x7ee5, 0x001e, 
		0x7ef5, 0x001e,  }
};
const CAFrame actlist_198e6[] = {
	{10, 0x00, 0x0, &image_1991a, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_1994c, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_198e6[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19adc = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 0x836c, 0x001f, 0x837c, 0x001f, 
		0x835d, 0x001f, 0x836d, 0x001f, 0x837c, 0x003f, 0x7ec6, 0x001d, 
		0x7ed6, 0x001d, 0x7ec7, 0x001d, 0x7ed7, 0x001d,  }
};
const Image image_19b0e = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835c, 0x001f, 
		0x836c, 0x001f, 0x837c, 0x001f, 0x835d, 0x001f, 0x836d, 0x001f, 
		0x837c, 0x003f, 0x7ec6, 0x001e, 0x7ed6, 0x001e, 0x7ec7, 0x001e, 
		0x7ed7, 0x001e,  }
};
const CAFrame actlist_19aa8[] = {
	{10, 0x00, 0x0, &image_19adc, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19b0e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19aa8[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_192aa = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 0x836e, 0x001f, 0x835b, 0x001f, 
		0x835f, 0x001f, 0x836f, 0x001f, 0x835b, 0x003f, 0x7ec2, 0x001d, 
		0x7ed2, 0x001d, 0x7ec3, 0x001d, 0x7ed3, 0x001d,  }
};
const Image image_192dc = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 
		0x836e, 0x001f, 0x835b, 0x001f, 0x835f, 0x001f, 0x836f, 0x001f, 
		0x835b, 0x003f, 0x7ec2, 0x001e, 0x7ed2, 0x001e, 0x7ec3, 0x001e, 
		0x7ed3, 0x001e,  }
};
const CAFrame actlist_19276[] = {
	{10, 0x00, 0x0, &image_192aa, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_192dc, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19276[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
/* image 192aa */
/* image 192dc */
const Image image_1946c = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 0x836e, 0x001f, 0x835b, 0x001f, 
		0x835f, 0x001f, 0x836f, 0x001f, 0x835b, 0x003f, 0x7ec4, 0x001d, 
		0x7ed4, 0x001d, 0x7ec5, 0x001d, 0x7ed5, 0x001d,  }
};
const Image image_1949e = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 
		0x836e, 0x001f, 0x835b, 0x001f, 0x835f, 0x001f, 0x836f, 0x001f, 
		0x835b, 0x003f, 0x7ec4, 0x001e, 0x7ed4, 0x001e, 0x7ec5, 0x001e, 
		0x7ed5, 0x001e,  }
};
const CAFrame actlist_19438[] = {
	{10, 0x00, 0x0, &image_1946c, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_1949e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19438[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1962e = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 0x836e, 0x001f, 0x835b, 0x001f, 
		0x835f, 0x001f, 0x836f, 0x001f, 0x835b, 0x003f, 0x7ee2, 0x001d, 
		0x7ef2, 0x001d, 0x7ee3, 0x001d, 0x7ef3, 0x001d,  }
};
const Image image_19660 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 
		0x836e, 0x001f, 0x835b, 0x001f, 0x835f, 0x001f, 0x836f, 0x001f, 
		0x835b, 0x003f, 0x7ee2, 0x001e, 0x7ef2, 0x001e, 0x7ee3, 0x001e, 
		0x7ef3, 0x001e,  }
};
const CAFrame actlist_195fa[] = {
	{10, 0x00, 0x0, &image_1962e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19660, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_195fa[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
/* image 1962e */
/* image 19660 */
const Image image_197f0 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 0x836e, 0x001f, 0x835b, 0x001f, 
		0x835f, 0x001f, 0x836f, 0x001f, 0x835b, 0x003f, 0x7ee6, 0x001d, 
		0x7ef6, 0x001d, 0x7ee7, 0x001d, 0x7ef7, 0x001d,  }
};
const Image image_19822 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 
		0x836e, 0x001f, 0x835b, 0x001f, 0x835f, 0x001f, 0x836f, 0x001f, 
		0x835b, 0x003f, 0x7ee6, 0x001e, 0x7ef6, 0x001e, 0x7ee7, 0x001e, 
		0x7ef7, 0x001e,  }
};
const CAFrame actlist_197bc[] = {
	{10, 0x00, 0x0, &image_197f0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19822, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_197bc[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_199b2 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 0x836e, 0x001f, 0x835b, 0x001f, 
		0x835f, 0x001f, 0x836f, 0x001f, 0x835b, 0x003f, 0x7ee4, 0x001d, 
		0x7ef4, 0x001d, 0x7ee5, 0x001d, 0x7ef5, 0x001d,  }
};
const Image image_199e4 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 
		0x836e, 0x001f, 0x835b, 0x001f, 0x835f, 0x001f, 0x836f, 0x001f, 
		0x835b, 0x003f, 0x7ee4, 0x001e, 0x7ef4, 0x001e, 0x7ee5, 0x001e, 
		0x7ef5, 0x001e,  }
};
const CAFrame actlist_1997e[] = {
	{10, 0x00, 0x0, &image_199b2, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_199e4, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1997e[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19b74 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 0x836e, 0x001f, 0x835b, 0x001f, 
		0x835f, 0x001f, 0x836f, 0x001f, 0x835b, 0x003f, 0x7ec6, 0x001d, 
		0x7ed6, 0x001d, 0x7ec7, 0x001d, 0x7ed7, 0x001d,  }
};
const Image image_19ba6 = {
    10+IMAGE_ATTR, 0x001d, 0x55, 0, 0,
    { 0x835e, 0x001f, 
		0x836e, 0x001f, 0x835b, 0x001f, 0x835f, 0x001f, 0x836f, 0x001f, 
		0x835b, 0x003f, 0x7ec6, 0x001e, 0x7ed6, 0x001e, 0x7ec7, 0x001e, 
		0x7ed7, 0x001e,  }
};
const CAFrame actlist_19b40[] = {
	{10, 0x00, 0x0, &image_19b74, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19ba6, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19b40[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};




const Image image_191d2 = {
    4, 0x111d, 0x31, 0, 0,
    { 0x7ec2,  }
};
const CAFrame actlist_191aa[] = {
	{10, 0x80, 0x0, &image_191d2, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_191aa[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19394 = {
    4, 0x111d, 0x31, 0, 0,
    { 0x7ec4,  }
};
const CAFrame actlist_1936c[] = {
	{10, 0x80, 0x0, &image_19394, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1936c[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19556 = {
    4, 0x111d, 0x31, 0, 0,
    { 0x7ee2,  }
};
const CAFrame actlist_1952e[] = {
	{10, 0x80, 0x0, &image_19556, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1952e[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

const Image image_19718 = {
    4, 0x111d, 0x31, 0, 0,
    { 0x7ee6,  }
};
const CAFrame actlist_196f0[] = {
	{10, 0x80, 0x0, &image_19718, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_196f0[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_198da = {
    4, 0x111d, 0x31, 0, 0,
    { 0x7ee4,  }
};
const CAFrame actlist_198b2[] = {
	{10, 0x80, 0x0, &image_198da, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_198b2[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19a9c = {
    4, 0x111d, 0x31, 0, 0,
    { 0x7ec6,  }
};
const CAFrame actlist_19a74[] = {
	{10, 0x80, 0x0, &image_19a9c, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19a74[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19e20 = {
    4, 0x111d, 0x31, 0, 0,
    { 0x7ee0,  }
};
const CAFrame actlist_19df8[] = {
	{10, 0x80, 0x0, &image_19e20, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19df8[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

const Image image_19c5e = {
    4, 0x111d, 0x31, 0, 0,
    { 0x7ec0,  }
};
const CAFrame actlist_19c36[] = {
	{10, 0x80, 0x0, &image_19c5e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19c36[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};



const Image image_191c6 = {
    4, 0x111e, 0x31, 0, 0,
    { 0x7ec2,  }
};

const CAFrame actlist_19192[] = {
	{10, 0x00, 0x0, &image_191c6, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_191d2, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19192[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19388 = {
    4, 0x111e, 0x31, 0, 0,
    { 0x7ec4,  }
};

const CAFrame actlist_19354[] = {
	{10, 0x00, 0x0, &image_19388, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19394, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19354[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1954a = {
    4, 0x111e, 0x31, 0, 0,
    { 0x7ee2,  }
};
const CAFrame actlist_19516[] = {
	{10, 0x00, 0x0, &image_1954a, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19556, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19516[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1970c = {
    4, 0x111e, 0x31, 0, 0,
    { 0x7ee6,  }
};
const CAFrame actlist_196d8[] = {
	{10, 0x00, 0x0, &image_1970c, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19718, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_196d8[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_198ce = {
    4, 0x111e, 0x31, 0, 0,
    { 0x7ee4,  }
};
const CAFrame actlist_1989a[] = {
	{10, 0x00, 0x0, &image_198ce, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_198da, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1989a[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19a90 = {
    4, 0x111e, 0x31, 0, 0,
    { 0x7ec6,  }
};
const CAFrame actlist_19a5c[] = {
	{10, 0x00, 0x0, &image_19a90, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19a9c, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19a5c[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19e14 = {
    4, 0x111e, 0x31, 0, 0,
    { 0x7ee0,  }
};
const CAFrame actlist_19de0[] = {
	{10, 0x00, 0x0, &image_19e14, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19e20, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19de0[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

const Image image_19c52 = {
    4, 0x111e, 0x31, 0, 0,
    { 0x7ec0,  }
};
const CAFrame actlist_19c1e[] = {
	{10, 0x00, 0x0, &image_19c52, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{10, 0x80, 0x0, &image_19c5e, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19c1e[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};

const Image image_1a234 = {
    4, 0x1110, 0x31, 0, 0,
    { 0x8200,  }
};
const CAFrame actlist_1a218[] = {
	{65535, 0x80, 0x0, &image_1a234, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a218[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a25c = {
    4, 0x1113, 0x31, 0, 0,
    { 0x8202,  }
};
const CAFrame actlist_1a240[] = {
	{65535, 0x80, 0x0, &image_1a25c, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a240[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a284 = {
    4, 0x1114, 0x31, 0, 0,
    { 0x8204,  }
};
const CAFrame actlist_1a268[] = {
	{65535, 0x80, 0x0, &image_1a284, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a268[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a2ac = {
    4, 0x1115, 0x31, 0, 0,
    { 0x8206,  }
};
const CAFrame actlist_1a290[] = {
	{65535, 0x80, 0x0, &image_1a2ac, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a290[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a2d4 = {
    4, 0x1110, 0x31, 0, 0,
    { 0x8220,  }
};
const CAFrame actlist_1a2b8[] = {
	{65535, 0x80, 0x0, &image_1a2d4, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a2b8[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a2fc = {
    4, 0x1116, 0x31, 0, 0,
    { 0x8222,  }
};
const CAFrame actlist_1a2e0[] = {
	{65535, 0x80, 0x0, &image_1a2fc, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a2e0[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a324 = {
    4, 0x1111, 0x31, 0, 0,
    { 0x8224,  }
};
const CAFrame actlist_1a308[] = {
	{65535, 0x80, 0x0, &image_1a324, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a308[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a34c = {
    4, 0x1111, 0x31, 0, 0,
    { 0x8226,  }
};
const CAFrame actlist_1a330[] = {
	{65535, 0x80, 0x0, &image_1a34c, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a330[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a374 = {
    4, 0x1119, 0x31, 0, 0,
    { 0x822a,  }
};
const CAFrame actlist_1a358[] = {
	{65535, 0x80, 0x0, &image_1a374, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a358[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a39c = {
    4, 0x111a, 0x31, 0, 0,
    { 0x8208,  }
};
const CAFrame actlist_1a380[] = {
	{65535, 0x80, 0x0, &image_1a39c, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a380[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a3c4 = {
    4, 0x111b, 0x31, 0, 0,
    { 0x8228,  }
};
const CAFrame actlist_1a3a8[] = {
	{65535, 0x80, 0x0, &image_1a3c4, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a3a8[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_1a3ec = {
    4, 0x111c, 0x31, 0, 0,
    { 0x820a,  }
};
const CAFrame actlist_1a3d0[] = {
	{65535, 0x80, 0x0, &image_1a3ec, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1a3d0[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};



const Image image_1932a = {
    8+IMAGE_ATTR, 0x001d, 0x56, 0, 0,
    { 0x7ee8, 0x001d, 0x7ef8, 0x001d, 0x7ee9, 0x001d, 
		0x7ef9, 0x001d, 0x7ec2, 0x001e, 0x7ed2, 0x001e, 0x7ec3, 0x001e, 
		0x7ed3, 0x001e,  }
};
const CAFrame actlist_1930e[] = {
	{65535, 0x80, 0x0, &image_1932a, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_1930e[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
/* image 1932a */
const Image image_194ec = {
    8+IMAGE_ATTR, 0x001d, 0x56, 0, 0,
    { 0x7ee8, 0x001d, 0x7ef8, 0x001d, 0x7ee9, 0x001d, 
		0x7ef9, 0x001d, 0x7ec4, 0x001e, 0x7ed4, 0x001e, 0x7ec5, 0x001e, 
		0x7ed5, 0x001e,  }
};
const CAFrame actlist_194d0[] = {
	{65535, 0x80, 0x0, &image_194ec, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_194d0[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_196ae = {
    8+IMAGE_ATTR, 0x001d, 0x56, 0, 0,
    { 0x7ee8, 0x001d, 0x7ef8, 0x001d, 0x7ee9, 0x001d, 
		0x7ef9, 0x001d, 0x7ee2, 0x001e, 0x7ef2, 0x001e, 0x7ee3, 0x001e, 
		0x7ef3, 0x001e,  }
};
const CAFrame actlist_19692[] = {
	{65535, 0x80, 0x0, &image_196ae, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19692[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
/* image 196ae */
const Image image_19870 = {
    8+IMAGE_ATTR, 0x001d, 0x56, 0, 0,
    { 0x7ee8, 0x001d, 0x7ef8, 0x001d, 0x7ee9, 0x001d, 
		0x7ef9, 0x001d, 0x7ee6, 0x001e, 0x7ef6, 0x001e, 0x7ee7, 0x001e, 
		0x7ef7, 0x001e,  }
};
const CAFrame actlist_19854[] = {
	{65535, 0x80, 0x0, &image_19870, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19854[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19a32 = {
    8+IMAGE_ATTR, 0x001d, 0x56, 0, 0,
    { 0x7ee8, 0x001d, 0x7ef8, 0x001d, 0x7ee9, 0x001d, 
		0x7ef9, 0x001d, 0x7ee4, 0x001e, 0x7ef4, 0x001e, 0x7ee5, 0x001e, 
		0x7ef5, 0x001e,  }
};
const CAFrame actlist_19a16[] = {
	{65535, 0x80, 0x0, &image_19a32, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19a16[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19bf4 = {
    8+IMAGE_ATTR, 0x001d, 0x56, 0, 0,
    { 0x7ee8, 0x001d, 0x7ef8, 0x001d, 0x7ee9, 0x001d, 
		0x7ef9, 0x001d, 0x7ec6, 0x001e, 0x7ed6, 0x001e, 0x7ec7, 0x001e, 
		0x7ed7, 0x001e,  }
};
const CAFrame actlist_19bd8[] = {
	{65535, 0x80, 0x0, &image_19bf4, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19bd8[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
const Image image_19f78 = {
    8+IMAGE_ATTR, 0x001d, 0x56, 0, 0,
    { 0x7ee8, 0x001d, 0x7ef8, 0x001d, 0x7ee9, 0x001d, 
		0x7ef9, 0x001d, 0x7ee0, 0x001e, 0x7ef0, 0x001e, 0x7ee1, 0x001e, 
		0x7ef1, 0x001e,  }
};
const CAFrame actlist_19f5c[] = {
	{65535, 0x80, 0x0, &image_19f78, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19f5c[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};
/* image 19f78 */
/* image 196ae */
const Image image_19db6 = {
    8+IMAGE_ATTR, 0x001d, 0x56, 0, 0,
    { 0x7ee8, 0x001d, 0x7ef8, 0x001d, 0x7ee9, 0x001d, 
		0x7ef9, 0x001d, 0x7ec0, 0x001e, 0x7ed0, 0x001e, 0x7ec1, 0x001e, 
		0x7ed1, 0x001e,  }
};
const CAFrame actlist_19d9a[] = {
	{65535, 0x80, 0x0, &image_19db6, 0, 0, 0, 0, 0, 0, 0, 0x00, 0, 0, 0, 0, 0, 0, 0, 0},
	{0,0,0,(struct image *)&actlist_19d9a[0], 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};



const CAFrame *actlist_19142[]={		// 19142 flags also
	actlist_191aa, 	actlist_191aa, 	actlist_1936c, 	actlist_1952e, 	
	actlist_1952e, 	actlist_196f0, 	actlist_198b2, 	actlist_19a74, 	
	actlist_19df8, 	actlist_19df8, 	actlist_1952e, 	actlist_19c36, 
};
const CAFrame *actlist_1916a[]={		// 1916a flag with P2 cursor over
	actlist_19276, 	actlist_19276, 	actlist_19438, 	actlist_195fa, 	
	actlist_195fa, 	actlist_197bc, 	actlist_1997e, 	actlist_19b40, 
};
const CAFrame *actlist_1915a[]={		// 1915a flag with P1 cursor over
	actlist_191de, 	actlist_191de, 	actlist_193a0, 	actlist_19562, 	
	actlist_19562, 	actlist_19724, 	actlist_198e6, 	actlist_19aa8, 
};
const CAFrame *actlist_1912a[]={		// 1912a flags
	actlist_19192, 	actlist_19192, 	actlist_19354, 	actlist_19516, 	
	actlist_19516, 	actlist_196d8, 	actlist_1989a, 	actlist_19a5c, 	
	actlist_19de0, 	actlist_19de0, 	actlist_19516, 	actlist_19c1e, 
};
const CAFrame *actlist_1a200[]={		// 1a200 small player pictures
	actlist_1a218, 	actlist_1a240, 	actlist_1a268, 	actlist_1a290, 	
	actlist_1a2b8, 	actlist_1a2e0, 	actlist_1a308, 	actlist_1a330, 	
	actlist_1a358, 	actlist_1a380, 	actlist_1a3a8, 	actlist_1a3d0, 
};
const CAFrame *actlist_grayflags[]={			// 1917a
	actlist_1930e, 	actlist_1930e, 	actlist_194d0, 	actlist_19692, 	
	actlist_19692, 	actlist_19854, 	actlist_19a16, 	actlist_19bd8, 	
	actlist_19f5c, 	actlist_19f5c, 	actlist_19692, 	actlist_19d9a, 
};


static void sub_18d9a(Object *obj, Player *ply) {
	UD1E *ud = (UD1E *)&obj->UserData;
	
	if (ply->Human && ply->FighterID == obj->UserByte) {
		if (ply->SelectComplete) {
			ud->x0080 = ply->Side ? actlist_1916a : actlist_1915a;
		} else {
			ud->x0080 = actlist_19142;
		}
	}
}
static void sub_18e52(Object *obj) {
	setaction_list(obj, actlist_19142, obj->UserByte);
}

void action_1e(Object *obj) {		//18c1c
	UD1E *ud = (UD1E *)&obj->UserData;
	static const char data_18e2a[][2]={{0x78, 0x78},{0x28,0xc8},{0x50,0xa0}};
	switch (obj->SubSel) {
		case 3:
			if (obj->mode0 == 0) {		
				setaction_list(obj, actlist_1912a, obj->UserByte);		// flags
			}
			enqueue_and_layer(obj);
			break;
		case 0 ... 2:
			switch (obj->mode0) {
				case 0:
					NEXT(obj->mode0);
					obj->LocalTimer = 1 * TICKS_PER_SECOND;
					ud->x0084 = 0;
					g.PlaneLandedInCity[obj->UserByte] = FALSE;
					ud->x0080 = actlist_1912a;
					setaction_list(obj, actlist_1912a, obj->UserByte);
					if (obj->UserByte >= STAGE_THAILAND_BISON) {
						if (g.UpToBosses == FALSE) {
							obj->mode0 = 6;
							return;
						}
						if (obj->UserByte != STAGE_THAILAND_BISON) {
							if (obj->UserByte == STAGE_THAILAND_SAGAT && g.OnLevel8 && g.x0a03 == 0) {
								obj->mode0 = 6;
								return;
							}
						} else {
							if (g.OnLevel8 == 0) {
								obj->mode0 = 6;
								return;
							}
						}
						
					} 
					//18cc0
					if (obj->SubSel == 2) {
						obj->mode1 = 4;
					} else if (g.CurrentStage != STAGE_USA_BALROG) {
						return;
					} else if (obj->SubSel == 1) {
						if (g.x0a02) {
							obj->mode1 = 16;
							sub_18e52(obj);
						} else {
							obj->mode1 = 10;
						}
					}
					break;
				case 2:
					if (g.CurrentStage != STAGE_USA_BALROG && obj->UserByte == 0 && obj->Step == 0 && obj->mode2 == 0) {
						if (--obj->LocalTimer == 0) {
							g.Pause_9e1 = 1;
							obj->mode2 += 2;
						}
					}
					switch (obj->mode1) {
						case 0:
						FLAGAGAIN:
							if (g.Defeated[obj->UserByte]) {
								ud->x0080 = actlist_grayflags;
							} else {
								ud->x0080 = actlist_1912a;	
							}
							sub_18d9a(obj, PLAYER1);
							sub_18d9a(obj, PLAYER2);
							setaction_list(obj, ud->x0080, obj->UserByte);
							if (g.PlaneLandedInCity[obj->UserByte]) {
								NEXT(obj->mode1);
							}
							break;
						case 2:				
							actiontick(obj);
							break;
						case 4:
							NEXT(obj->mode1);
							obj->LocalTimer = 3 * TICKS_PER_SECOND;
							setaction_list(obj, actlist_1a200, obj->UserByte);
							break;
						case 6:
							if (--obj->LocalTimer == 0) {
								NEXT(obj->mode1);
								g.x0a03 = 1;
								g.Pause_9e1 = -1;
							}
							break;
						case 8:		/* nothing */  break;
						case 10:
							NEXT(obj->mode1);
							ud->x0084 = 1;
							obj->LocalTimer = data_18e2a[obj->UserByte - 9][0];
							obj->x001f      = data_18e2a[obj->UserByte - 9][1];
							setaction_list(obj, actlist_1a200, obj->UserByte);
							break;
						case 12:
							if (--obj->LocalTimer == 0) {
								NEXT(obj->mode1);
								ud->x0084 = 0;
								queuesound(0x23);		/*dingdong!*/
							}
							break;
						case 14:
							if (--obj->x001f == 0) {
								NEXT(obj->mode1);
								sub_18e52(obj);
							}
							break;
						case 16:
							obj->mode1 = 0;
							if (obj->UserByte == STAGE_THAILAND_SAGAT) {
								g.Pause_9e1 = 1;
								g.x0a02     = TRUE;
							}
							goto FLAGAGAIN;			// suck it
							break;
							FATALDEFAULT;
					}
					if (ud->x0084 == 0) {
						enqueue_and_layer(obj);
					}
					break;
				case 4:
				case 6:
					FreeActor(obj);
				default:
					break;
			}
			break;
		default:
			break;
	}
}
void action_2c (Object *obj) {
	switch (obj->SubSel) {
		case 0:
		case 2:
			break;
			FATALDEFAULT;
	}
	switch (obj->mode0) {
		case 0:
			NEXT(obj->mode0);
			obj->Pool = 6;
			setaction_list(obj, actlist_1a200, obj->UserByte);
			break;
		case 2:
			if(g.Defeated[obj->UserByte]) {
				if (g.Player1.Human == FALSE || g.Player1.FighterID != obj->UserByte) {
					if (g.Player2.Human == FALSE || g.Player2.FighterID != obj->UserByte) {
						enqueue_and_layer(obj);
					}
				}
			}
			break;
		case 4:
		case 6:
			FreeActor(obj);
			break;
			FATALDEFAULT;
	}
}
