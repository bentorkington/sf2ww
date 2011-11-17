/*
 *  dhalsim.c
 *  GLUTBasics
 *
 *  Created by Ben on 7/11/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


#include "sf2types.h"
#include "sf2macros.h"
#include "sf2const.h"
#include "structs.h"
#include "player.h"
#include "particle.h"
#include "rules.h"

#include "computer.h"

#include "lib.h"
#include "gfxlib.h"


#include "dhalsim.h"
#include "dhalsimdata.h"

struct UserData_Dhalsim {
	signed char x0080;
};

typedef struct UserData_Dhalsim UD;

static short sub_35f7e(Player *ply, int d0) {
	UD *ud = (UD *)ply->UserData;
	
	ply->StandSquat = 4;	// SS_AIRMOVE
	ply->Move = d0;
//XXX	ply->Path = data_cfaf4;
	if (ply->PunchKick) {
		// 35fb6 
		CASetAnim2(ply, STATUS_JUMP_KICK, d0);
		if(ply->Flip != FACING_LEFT){
			ud->x0080 = 0xb;
		} else {
			ud->x0080 = 0x15;
		}
	}
	else {
		// 35f96
		CASetAnim2(ply, STATUS_JUMP_PUNCH, d0);
		if(ply->Flip != FACING_LEFT){
			ud->x0080 = 0xa;
		} else {
			ud->x0080 = 0x16;
		}
	}
	// 35fd4
	ply->mode1 = 0xa;
	ply->mode2 = 0;
	ply->mode3 = 0;
	return -1;
}

short sub_35fe6(Player *ply) {
	if(ply->VelY.full + 0x100 < 0x200) {
		return sub_35f7e(ply, 5);
	}
	return 0;
}