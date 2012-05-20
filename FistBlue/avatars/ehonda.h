/*
 *  ehonda.h
 *  GLUTBasics
 *
 *  Created by Ben on 18/12/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


void pl_cb_setstatus1_ehonda(Player *player, short status);
void pl_cb_setstatus2_ehonda(Player *player, short status, int argd0);
void pl_cb_setstatus3_ehonda(Player *player, short status);

void PLCBPowerEHonda(Player *ply);
void PSCBAttackEHonda(Player *ply);		/* attacking callback */
void PLCBCompAttackEHonda(Player *ply);
int PLCBCompJumpEHonda(Player *ply);
short PLCBStandEHonda(Player *ply);
short PLCBCrouchEHonda(Player *ply);
short PLCBJumpEHonda(Player *ply);

struct ehondathrow {
	short d2;
	short success;
	short d0;
};

struct ehondares {
	u8 d0;
	u8 d1;
};

struct ehondamove {
	u8	a;
	u8	b;
};
typedef struct ehondamove EHondaMove;



struct UserData_EHonda {
	u16	x0082;
	u16	x0084;
	u8	x0086;
	u16	x0088;
	u8	x008a;
	u8	x0090;		// current move?
	u8	x0091;		// Oink SM
	u8	x0092;		// timer
	u8	x0093;
	u8	x0094;		// Oink distance timer
	u8	x0095;
	u32 x0096;	// XXX hmmm
	EHondaMove pm1;	/// 97
	u8	x0098;		// XXX
	EHondaMove pm2;	// 99
	u8	x009a;
	EHondaMove pm3;	//9b
	u8	x009c;
	
} __attribute__((packed));


