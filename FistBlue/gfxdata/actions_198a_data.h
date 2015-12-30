/*
 *  actions_198a_data.h
 *  MT2
 *
 *  Created by Ben on 28/06/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

/* sythentic: */
const HitBox hitb_null[] = {{0, 0, 0, 0}};
const HitBoxAct hitbact_null[] = {{0,0,0,0,0,0,0,0,0,0,0}};

								  

const static HitBox hitb_24ef2[] = {EMPTY_HITBOX, {0, 48, 17, 50}};
	const static struct hitboxes hitboxes_24eda={
		hitb_null,
		hitb_null,
		hitb_null,
		hitb_null,
		hitbact_null,
		hitb_24ef2,
	};

char data_24c3c[] = { 0,4,2,3,0,1,3,2,1,4,3,2,0,1,4,2};

const HitBox hitb_24ec6[]={{0,0,0,0}, {0,0,0,0}};
const HitBoxAct hitb_24ed2 = { 0,0,0,0,-5,0x32, 0x49, 0x2e};

const struct hitboxes data_24eba = {
	hitb_24ec6,
	hitb_24ec6,
	hitb_24ec6,
	hitb_24ec6,
	&hitb_24ed2,
	hitb_24ec6,
};


#pragma mark ActB06 The Car
// in car.c
#pragma mark Act07

const HitBox hitb_274dc[] = {{ 0, 0, 0, 0}, {-3, 36, 44, 35}};

const struct hitboxes hitboxes_274c4 = {
	hitb_null,
	hitb_null,
	hitb_null,
	hitb_null,
	hitbact_null,
	hitb_274dc,
};


#pragma mark Act09 Barrels

const HitBox hitb_27e82[] = {EMPTY_HITBOX, {-7, 28, 34, 29}};
const HitBoxAct hitb_27e8a[] = {EMPTY_HITBOXACT, {-7, 28, 18, 15, 0, 1, 0, 1, 2, 0, 0, 0}};

const struct hitboxes hitboxes_27e6a = {
	hitb_27e82,
	hitb_null,
	hitb_null,
	hitb_null,
	hitb_27e8a,
	hitb_27e82,
};

