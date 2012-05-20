/*
 *  guile.h
 *  GLUTBasics
 *
 *  Created by Ben on 18/12/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

void pl_cb_setstatus1_guile(Player *player, short status);
void pl_cb_setstatus2_guile(Player *player, short status, int argd0);
void pl_cb_setstatus3_guile(Player *player, short status);


short PLCBStandGuile(Player *ply);	/* standmove callback */
short PLCBCrouchGuile(Player *ply);
short PLCBJumpGuile(Player *ply);
void PLCBPowerGuile(Player *ply);
void sub_2ff6e(Player *ply);	/* victory */

void PSCBAttackGuile(Player *ply);		/* PLSTAT_ATTACKING callback */
void PSCBPowerGuile(Player *ply);		/* PLSTAT_POWERMOVE callback */

void PLCBCompAttackGuile(Player *ply);		//33b4c
int  PLCBCompJumpGuile(Player *ply);


struct UserData_Guile {
	u8	SonicStep;
	u8	SonicTimer;
	u8	SonicTimer2;
	u8	x0083;
	u8	x0084;
	u8	x0085;
	u8	BladeStep;	// x0086
	u8	BladeTimer;	// x0087
	u8	BladeTimer2;
	u8	x0089;
	u8	BladeMaxStr;
	u8	x008b;
	u8	x008c;	/* Blade strength 0,1,2 */
	u8	x008d;
	u8	AirThrow;		// 8e
	u8	AirThrow2;
	u8	SonicStrength;	//0090  0,1,2
} __attribute__((packed));

