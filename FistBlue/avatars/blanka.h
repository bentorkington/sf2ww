/*
 *  blanka.h
 *  GLUTBasics
 *
 *  Created by Ben on 7/02/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */


void pl_cb_setstatus1_blanka(Player *player, short status);
void pl_cb_setstatus2_blanka(Player *player, short status, int argd0);
void pl_cb_setstatus3_blanka(Player *player, short status);


short PLCBStandBlanka(Player *ply);
short PLCBCrouchBlanka(Player *ply);
void PLCBPowerBlanka(Player *ply);
short PLCBJumpBlanka(Player *ply);

void PSCBPowerBlanka(Player *ply);
void PSCBAttackBlanka(Player *ply);

void PSCBVictoryBlanka(Player *ply);

void PLCBCompJumpBlanka(Player *ply);
void PLCBCompAttackBlanka(Player *ply);

struct blankamove {
	u8	a;
	u8	b;
};
typedef struct blankamove BlankaMove;


struct blankares {
	u8 d0;
	u8 d1;
};


struct blankathrow {
	short d2;
	short success;
	short d0;
};


struct UserData_Blanka {
	u8	x0080;
	u16	x0082;		//newbuttons
	u16	x0084;
	u8	x0086;
	u16	x0088;
	u8	x008a;
	u16	x008c;
	u8	x0090;		// current move?
	u8	x0091;
	u8	x0092;		// timer
	u8	x0093;
	u8	x0094;
	u8	x0095;
	const char *x0096;	// XXX hmmm
	u8	x0098;
	
	u8	x009a;
	
	struct blankamove pm1;	//9b
	struct blankamove pm2;	//9d
	struct blankamove pm3;	//9f
	u8	x00a1;
	Object *x00a2;
};
