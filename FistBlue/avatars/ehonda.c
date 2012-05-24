/*
 *  ehonda.c
 *  GLUTBasics
 *
 *  Created by Ben on 18/12/10.
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
#include "actions.h"
#include "playerstate.h"
#include "computer.h"
#include "sound.h"
#include "sf2io.h"


#include "lib.h"
#include "gfxlib.h"


#include "ehonda.h"
#include "ehondadata.h"

extern Game g;


typedef struct UserData_EHonda UD;


void pl_cb_setstatus2_ehonda(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_3d2fc[status / 2], argd0);
}
void pl_cb_setstatus3_ehonda(Player *ply, short status) {
	pl_cb_setstatus2_ehonda(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_ehonda(Player *ply, short status) {
	pl_cb_setstatus2_ehonda(ply, status, 0);
}

void sub_2e3ac(Player *ply, u16 d0) {		
	// XXX setaction_direct((Object *)ply, actlist_3ffbc[d0]);		// XXX
}		


static void _CheckCharge(Player *ply, EHondaMove *bm, short buttons_d0, short d1) { //2e040
	UD *ud=(UD *)&ply->UserData;
	if (ud->x0082 & buttons_d0) {
		if(--bm->a == 0) {
			bm->b = 0;
			bm->a = d1;
		} 
	} else {
		bm->b++;
		bm->a = d1;
	}
}
static void _EHondaSMCheckOink(Player *ply) {		// 2df74
	static const u8 data_2dfa6[]={0x3c, 0x23, 0x0c, 0x1};
	static const u8 data_2dfd4[]={ 8, 9, 8,10, 8,11, 8, 12, 8, 9, 8,10, 8,11, 8,13,
		8, 9, 8,10, 8,11, 8,14, 8, 9, 8,10, 8,12, 8,15,
	};
	UD *ud=(UD *)&ply->UserData;

	
	switch (ud->x0091) {
		case 0:
			ud->x0093 = 0;
			if(ply->JoyCorrect & 1) {
				if(--ud->x0092 == 0){
					NEXT(ud->x0091);
				}
			} else {
				ud->x0092 = data_2dfa6[g.x0320];
			}
			break;
		case 2:
			if((ply->JoyCorrect & 1)==0) {
				NEXT(ud->x0091);
				ud->x0092 = data_2dfd4[RAND32];
			}
			break;
		case 4:
			if (ply->JoyCorrect & 2) {
				NEXT(ud->x0091);
				ud->x0092 = 8;
				ud->x0093 = 6;
			} else {
				if (--ud->x0092 == 0) {
					ud->x0091 = 0;
					ud->x0092 = 60;
				}
			}
			break;
		case 6:
			if (--ud->x0092 == 0) {
				ud->x0092 = 4;
				if (--ud->x0093 == 0) {
					ud->x0091 = 0;
					ud->x0092 = 60;
				}
			}
			break;
			FATALDEFAULT;
	}
	
}



#pragma mark Callbacks


						   



void PLCBPowerEHonda(Player *ply) {	    //2de90
/*	if (ud->x0080 == 0) {
		Machine suicides if interrupt handler changed
	}
 */
	UD *ud=(UD *)&ply->UserData;
	
	ud->x0082 = (~ply->JoyCorrectDash & ply->JoyCorrect);
	
	_EHondaSMCheckOink(ply);
	_CheckCharge(ply, &ud->pm1, BUTTON_A, 0xf);
	_CheckCharge(ply, &ud->pm2, BUTTON_B, 0xa);
	_CheckCharge(ply, &ud->pm3, BUTTON_C, 0x5);
	if (ud->x0082 & 0x70) {
		ud->x0088 = (sf2rand() & 1) << 16 + sf2rand();
	} else {
		return;
	}
}
	
#pragma mark Computer CBs

void PSCBVictoryEHonda(Player *ply) {		// 2e1fe
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			// still todo, this will do for now
			sub_2e3ac(ply, ((sf2rand() & 1) ? 0x6c : 0x88));
			break;
		case 2:
			ply->PSFinishedParticipating = TRUE;
			if (AF2) {
				soundsting(AF2);
			}
			PLAYERTICK;
			break;
		FATALDEFAULT;
	}
}

