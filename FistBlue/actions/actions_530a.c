/*
 *  actions_530a.c
 *  GLUTBasics
 *
 *  Created by Ben on 26/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


#include "sf2.h"

#include "structs.h"

#include "particle.h"

#include "lib.h"
#include "rules.h"


#include "gfxlib.h"

extern Game g;

#include "actions_530a.h"


u16 **data_82c16[4] = {
	NULL, NULL, NULL, NULL, 
	/* XXX
	 &data_8a8ac,
	 data_c0000,
	 data_c5000,
	 data_ca000,
	 */	
};

static void sub_82bec(Object2 *act) {
	act->x0048=data_82c16[act->UserByte][g.CurrentStage][act->Timer2];		
	/* array array of void * */
}


static void sub_82c4c(Object2 *act) {
	u16 *a0 = act->x0042;
	u16 *a1 = act->x0048;
	short i;
	
	for(i=0; i<16; i++) {
		*a0 = (*a0 & 0xf000) | *a1;
		a0++;
		a1++;
	}
}

static void sub_82bb2(Object2 *act) {
	act->LocalTimer = act->x001f;
	if (act->x004e[act->NextReactMode2] < 0) {
		act->NextReactMode2 = 0;		
	}		/* coded like a while() in 68k */
	act->Timer2 = act->x004e[act->NextReactMode2];
	act->NextReactMode2++;
	sub_82bec(act);
	sub_82c4c(act->x0042);	
}

static void sub_82c26(Object2 *act) {
//XXX	act->x0042=data_90000[act->x0040][act->x0041];	/* x0042 is *, x0040 uchar <<10, x0041 uchar << 5 */
}

static void action_530a_00 (Object2 *act, short d7) {	
	/* sub_82a82 Actions have params in place of AclX and AclY */

	const static signed char data_82b2e[112]={
		 6,   7,  -1,  
		16,  21,  30,  21,  -1, 
		26,  26,  25,  26,  -1, 
		27,  28,  29,  -1,  
		28,  29,  27,  -1,  
		29,  27,  28,  -1,  
		24,  25,  26,  25,  -1,  
		14,  15,  13,  -1,
		17,  18,  19,  20,  -1,
		 2,   3,   4,   5,   2,   3,   4,   5,   2,   3,   4,   5,   2,   3,   4,   5,   0,
		 1,   0,   1,  -1,   9,  10,  11,  10, 
		-1,  23,  24,  25,  26,  25,  24,  -1,
		21,  22,  -1,   7,   8,   9,   8,   9, 
		10,   9,   8,   9,   8,  -1,  11,  12, 
		-1,  27,  28,  -1,   8,   9,  10,  -1, 
		 0,   1,   2,   3,   4,   3,   4,   3,
		 4,   4,   4,  -1,   4,   5,   6,  -1, 
	};
	/* next address 00082b9e */
	
	const static signed char *data_82b08[]={
		&data_82b2e[0],				//NextReactMode2
		&data_82b2e[3],
		&data_82b2e[8],
		&data_82b2e[0xd],
		&data_82b2e[0x11],
		&data_82b2e[0x15],
		&data_82b2e[0x19],
		&data_82b2e[0x1e],
		&data_82b2e[0x22],
		&data_82b2e[0x28],
		&data_82b2e[0x3d],
		&data_82b2e[0x41],
		&data_82b2e[0x48],
		&data_82b2e[0x4b],
		&data_82b2e[0x56],
		&data_82b2e[0x59],
		&data_82b2e[0x5c],
		&data_82b2e[0x60],
		&data_82b2e[0x6c],
	};
	
	const static unsigned char data_82acc[20]={0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x01,0x01,0x03,0x03,0x02,0x02,0x03,0x05};               /* for x0040 and UserByte */
	const static unsigned char data_82ae0[20]={0x14, 0x0e, 0x0a, 0x04, 0x04, 0x04, 0x10, 0x04, 0x0a, 0x04, 0x0e, 0x07, 0x0a, 0x0c, 0x05, 0x05, 0x0a, 0x10, 0x09, 0x1f };     /* for x001f */
	const static unsigned char data_82af4[20]={0x06, 0x10, 0x1a, 0x1b, 0x1c, 0x1d, 0x18, 0x0e, 0x11, 0x02, 0x09, 0x17, 0x16, 0x08, 0x0f, 0x1f, 0x1a, 0x1c, 0x04, 0x44 };		/* for x0041 XXX check that last value */
	
	switch (act->mode0) {
		case 0:
			/* 82a96 */
			act->mode0 +=2;
			
			act->NextReactMode2 = 0;
			act->UserByte = data_82acc[act->SubSel];	/* all chars */
			act->x0040 = data_82acc[act->SubSel];	
			act->x0041 = data_82af4[act->SubSel];
			act->x001f = data_82ae0[act->SubSel];
			act->x004e = data_82b08[act->SubSel];	
			sub_82c26(act);	/* lookup palette in data_90000, x0040, x0041 */
			break;
		case 2:	/* 0x82b9e */
			switch (act->mode1) {
				case 0:
					act->mode1 +=2;
					sub_82bb2(act);
					/* XXX */
					break;
				case 2:
					if(act->LocalTimer-- == 0) {
						sub_82bb2(act);
					}
					return;
			}
		case 4:
		case 6:
			clearpush_5d0c(act);
			break;
		FATALDEFAULT;
	}
	
}

void actions_530a(void) {			/* 82a52 */
	short d7, i;
	
	debughook(3);
	d7=0x13;
	for(i=0; i<0x13; i++) {
		if(g.x530a[i].exists == 0) {break;}
		switch (g.x530a[i].Sel) {
			case 0:
				action_530a_00(&g.x530a[i], d7);
				break;
			default:
				//printf("action 530a id %d not implemented\n", g.Objects2[i].Sel);
				break;
		}
		d7--;
	}
}


