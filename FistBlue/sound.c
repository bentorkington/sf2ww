/*
 *  sound.c
 *  GLUTBasics
 *
 *  Created by Ben on 20/01/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include "sf2types.h"
#include "sf2macros.h"

#include "sf2const.h"

#include "structs.h"
#include "sound.h"

extern Game g;




#pragma mark ---- SOUND ---

void sound_cq_addto(short data) {	/* 62ac */
	/* sound unimplemented */
}

void sound_cq_1(short data) {	/* 629a */
	if(g.DemoSound || g.FastEndingFight || g.InDemo == FALSE) {
		sound_cq_addto(data);
	}
}

void soundsting(short data) {		/* 62c4 */
	sound_cq_addto(data);
	sound_cq_addto(data);
}
void ssound1(short data) {			/* 62cc */
	sound_cq_addto(data);
	sound_cq_addto(data);
}
void sound_cq_f0f7(void) {			/* 62d4 */
	ssound1(0xf0);
	ssound1(0xf7);
}
void coinsound(void) {				/* 635a */
	ssound1(0x20);
}
void quirkysound(short data) {
	queuesound(data + 0x25);
	/* was full of tamper protection - removed */
}
void queuesound(int data) {
	/* unimplemented */
}
void setstagemusic(void) {
	sound_cq_1( (u16 []){1,2,3,5,4,6,7,8,12,11,9,10,13,13,13}[g.CurrentStage] );
}


void sound_cq_f7_ff(void) {
	/* unimplemented */
}
