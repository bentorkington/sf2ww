/*
 *  projdata.h
 *  GLUTBasics
 *
 *  Created by Ben on 9/11/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


const short data_22bf6[5] = { 0x50, 0x58, 0x00, 0x40, 0x68 };
const short data_22c00[5] = { 0x38, 0x22, 0x00, 0x32, 0x4e };

const HitBox hitb_2329c[] = {{0,0,0,0}, {0,0,0x1c,0x13},{0,0,0x29,0x11}};
const HitBoxAct hitb_232a8[] = {{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,15,12,11,1,0x2a,3,2,2,3,0x14}};

const HitBox hitb_23334[] = {{0,0,0,0}, {0,0,0x29, 0x11}};
const HitBoxAct hitb_2334c = {0,0,0,0, 0,0,0,0};

const HitBox hitb_233bc[] = {{ 0,0,0,0}, {0,0, 0x1c, 0x13},{ 0, 0, 0, 0}};
const HitBoxAct hitb_233c4 = { 0,0,0,0, 0,0, 0,    0,    0, 0, 0, 0};

const HitBox hitb_2493e[] = {{0,0,0,0}, {0x2, 0x11, 0x18, 0x10}};
const HitBoxAct hitb_24946 = { 0,0,0,0,0,0,0,0,0,0,0,0};

const HitBox hitb_249a2[] = {{ 0,0,0,0}, {0xde, 0x20, 0x28, 0x18},{ 0,0,0,0}};
const HitBoxAct hitb_249aa = { 0,0,0,0, 0,    0,    0,    0,    0,0,0,0};

const struct hitboxes data_23290 = {		/* Hadouken Hitboxes */
	hitb_2329c,
	hitb_2329c,
	hitb_2329c,
	hitb_2329c,
	hitb_232a8,
	hitb_2329c,
};
const struct hitboxes data_23328 = {
	hitb_23334,
	hitb_23334,
	hitb_23334,
	hitb_23334,
	&hitb_2334c,
	hitb_23334,
};
const struct hitboxes data_233b0 = {
	hitb_233bc,
	hitb_233bc,
	hitb_233bc,
	hitb_233bc,
	&hitb_233c4,
	hitb_233bc,
};
const struct hitboxes data_24932 = {
	hitb_2493e,
	hitb_2493e,
	hitb_2493e,
	hitb_2493e,
	&hitb_24946,
	hitb_2493e,
};
const signed char data_24976[32]={
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
};
/* next address 00024996 */

const struct hitboxes data_24996= {
	hitb_249a2,
	hitb_249a2,
	hitb_249a2,
	hitb_249a2,
	&hitb_249aa,
	hitb_249a2,
};
const signed char data_249da[32]={
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
};
/* next address 000249fa */

const short data_23572[][2]={
	{0,0}, { -0x26, 0x3b }, {-0x3c, 0x2a}, {-0x49, 0x17}
};
	

	
/* What is this waste of time for? */
const signed char data_23308[] = {0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,
};
const signed char data_23390[] = {0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,
};
const signed char data_233f4[] = {0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,
};

struct {
	const struct hitboxes *hb;
	const signed char *mb;
} data_22c0a[5] = {
	{&data_23290, data_23308,},
	{NULL, NULL},	// Can't happen
	{NULL, NULL},	// Can't happen
	{&data_23328, data_23390},
	{&data_233b0, data_233f4},
};


//velocities, XXX not all done
const VECT16 data_cf038[] = {{0x300, 0}, {-0x300, 0}, {0x380, 0}, {-0x380, 0}};
const VECT16 data_cf048[] = {{0x400, 0}, {-0x400, 0}, {    0, 0}, { 0x480, 0}, {-0x480, 0}};		// very strange
const VECT16 data_cf05c[] = {{0x500, 0}, {-0x500, 0}, {0x580, 0}, {-0x580, 0}};
const VECT16 data_cf07c[] = {{0x700, 0}, {-0x700, 0}, {0x780, 0}, {-0x780, 0}};


#pragma mark ---- Hadouken Data ----

const VECT16 *data_22c32[]={data_cf038, data_cf048 , data_cf05c};

#pragma mark ---- Sonic Boom ----

#pragma mark ---- TIGER ----

const VECT16 *data_22c3e[]={data_cf038, data_cf05c, data_cf07c};

#pragma mark ---- Yoga Fire Data ----
/*
 * YOGA FIRE 
 */

const VECT16 *data_2349a[] = 	// velocity paths
	{data_cf038, data_cf05c, data_cf07c };

#define IMAGE_BLOCK	0x8000		// actually image_with_attrs

#pragma mark ---- Yoga Flame ----
