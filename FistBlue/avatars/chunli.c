/*
 *  chunli.c
 *  GLUTBasics
 *
 *  Created by Ben on 10/02/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
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

#include "chunli.h"

typedef struct UserData_ChunLi UD;
typedef struct UserDataComp_ChunLi UDCOMP;
extern Game g;


void pl_cb_setstatus2_chunli(Player *ply, short status, int argd0) {
    RHSetActionList((Object *)ply, RHOffsetLookup16(RHCODE(0x56dbc), status / 2), argd0);
}
void pl_cb_setstatus3_chunli(Player *ply, short status) {
	pl_cb_setstatus2_chunli(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_chunli(Player *ply, short status) {
	pl_cb_setstatus2_chunli(ply, status, 0);
}


void PSCBVictoryChunLi(Player *ply) {
	u16 d2;
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->Flip ^= 1;
			d2 = 0xaaaa;
			if (g.OnBonusStage) {
				d2 = 0x1044;
				if (ply->RoundsWon != 1) {
					d2 = 0xefbb;
				}
			}
			if (d2 & (1 << (RAND16))) {
				CASetAnim2(ply, CHUNLI_STATUS_VICTORY, 1);
			} else {
				CASetAnim2(ply, CHUNLI_STATUS_VICTORY, 0);
			}
			break;
		case 2:
			PLAYERTICK;
			if (AF2) {
				NEXT(ply->mode3);
				g.FightOver = TRUE;
				ply->PSFinishedParticipating = TRUE;
			}
			break;
		case 4:
			if (AF2 & 0x80) {
				PLAYERTICK;
			} else {
				soundsting(AF2);
			}
			break;
		FATALDEFAULT;
	}
}
#pragma mark Computer ChunLi
