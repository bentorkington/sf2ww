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

void sub_2e3ac(Player *ply, u16 d0);


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
	u16			newbuttons;			// 0082
	u16			throwbuttons;
	char		volley;				//0086 
	u16			x0088;
	char		x008a;
	char		oink_mode;		// 0091 Oink SM
	char		oink_timer;		// timer
	char		oink_potential;
	char		oink_timer2;		// 0094 Oink distance timer
	char		x0095;
	char		x0096;
	EHondaMove	pm1;	// 97
	EHondaMove	pm2;	// 99
	EHondaMove	pm3;	// 9b
	
};

