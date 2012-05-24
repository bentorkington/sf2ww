/*
 *  guile.c
 *  GLUTBasics
 *
 *  Created by Ben on 18/12/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

#include "sf2types.h"
#include "sf2macros.h"
#include "sf2const.h"
#include "gstate.h"
#include "structs.h"
#include "player.h"
#include "particle.h"
#include "rules.h"
#include "actions.h"
#include "playerstate.h"
#include "computer.h"
#include "sound.h"
#include "lib.h"
#include "gfxlib.h"
#include "sf2io.h"

#include "projectiles.h"
#include "guile.h"
#include "guiledata.h"

extern Game g;



static void guile_attack_bison(Player *ply);


static void sub_2f1b0(Player *ply);
static short GuileButtons(Player *ply);

typedef struct UserData_Guile UD;

#pragma mark Animation Callbacks

void pl_cb_setstatus2_guile(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_4abac[status / 2], argd0);
}
void pl_cb_setstatus3_guile(Player *ply, short status) {
	pl_cb_setstatus2_guile(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_guile(Player *ply, short status) {
	pl_cb_setstatus2_guile(ply, status, 0);
}

void _GuileExitStand(Player *ply) {		/* 2f08c */
	UD *ud=(UD *)&ply->UserData;	
	ud->x008d = 0;
	ply_exit_stand(ply);
}
void _GuileExitCrouch(Player *ply) {		/* 2f096 */
	UD *ud=(UD *)&ply->UserData;	
	ud->x008d = 0;
	ply_exit_crouch(ply);
}
void _GuileExitJump(Player *ply) {		/* 2f0a0 */
	UD *ud=(UD *)&ply->UserData;	
	ud->x008d = 0;
	ply_exit_air(ply);
}

#pragma mark Attack Callback

// Powermove Callback
// Victory Callback
void sub_2ff6e(Player *ply) {			// 2ff6e
	u16 d1;
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->PSFinishedParticipating = TRUE;
			g.FightOver = TRUE;
			if (ply->RoundsWon == 2 && g.OnFinalStage) {
				NEXT(ply->mode3);	// Beaten Bison
			} else {
				if (g.OnBonusStage) {
					d1 = 0x5555;
				} else if (ply->RoundsWon == 2) {
					d1 = 0x1110;
				} else {
					d1 = 0xeeef;
				}
				if (d1 & (1 << (RAND16))) {
					CASetAnim2(ply, 0x50, 0);
				} else {
					CASetAnim2(ply, 0x50, 1);
				}
			}
			break;
		case 2:
			if (AF2) {
				soundsting(AF2);
			}
			PLAYERTICK;
			break;
		case 4:
			guile_attack_bison(ply);
			break;
		default:
			break;
	}
	
}

static void guile_attack_bison(Player *ply) {
	Object *obj;
	const static VECT16 data_3003c[] = {{0xfca0, 0x0}, {0x360, 0x0}};
	
	if (g.Pause_9e1 == 2) {
		switch (ply->UserData[5]) {
			case 0:
				NEXT(ply->UserData[5]);
				if (ply->Opponent->XPI > ply->XPI) 
				{ply->Flip = 0;} else {ply->Flip = 1;}
				ply->Step = ply->Flip;
				ply->Path = data_3003c;
				CASetAnim2(ply, STATUS_WALKING, 0);	// walk up to M.Bison
				PLAYERTICK;
				break;
			case 2:
				update_motion((Object *)ply);
				if (ABS(ply->XPI - ply->Opponent->XPI) < 0x20) {
					NEXT(ply->UserData[5]);
					if(obj = AllocActor()) {
						INITOBJC(obj, 0x25, 0xd, PLAYERX, PLAYERY);
						obj->Pool = 6;
						obj->Owner = ply->Opponent;
						obj->Flip = ply->Flip;
						if (ply->mode0 != 4) {
							obj->UserByte = 1;
						}
					}
					if (obj=AllocActor()) {
						INITOBJC(obj, 0x25, 0xb, 0, 0);
						obj->Pool = 6;
						obj->Owner = ply;
					}
					if (obj = AllocActor()) {
						INITOBJC(obj, 0x25, 0xc, 0, 0);
						obj->Pool = 6;
					}
					PLAYERY -= 6;
					//setaction_direct((Object *)ply, actlist_4d22e);  todo
				} else {
					PLAYERTICK;
				}
				break;
			case 4:
				/* does nothing */
				break;
			default:
				break;
		}
	}
}
