/*
 *  ehonda.c
 *  GLUTBasics
 *
 *  Created by Ben on 18/12/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

#include "sf2.h"
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

extern Game g;


typedef struct UserData_EHonda UD;

#define EHONDA_STATUS_VICTORY1	0x6c
#define EHONDA_STATUS_VICTORY2	0x88


void pl_cb_setstatus2_ehonda(Player *ply, short status, int argd0) {
    RHSetActionList((Object *)ply, RHOffsetLookup16(RHCODE(0x3d2fc), status / 2), argd0);
}
void pl_cb_setstatus3_ehonda(Player *ply, short status) {
	pl_cb_setstatus2_ehonda(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_ehonda(Player *ply, short status) {
	pl_cb_setstatus2_ehonda(ply, status, 0);
}

void sub_2e3ac(Player *ply, u16 d0) {
    RHSetActionList((Object *)ply, RHCODE(0x3ffbc), d0);
}		
	
void PSCBVictoryEHonda(Player *ply) {		// 2e1fe
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			// still todo, this will do for now
			sub_2e3ac(ply, ((sf2rand() & 1) ? EHONDA_STATUS_VICTORY1 : EHONDA_STATUS_VICTORY2));
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

