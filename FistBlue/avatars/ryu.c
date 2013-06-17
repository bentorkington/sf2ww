/*
 *  ryu.c
 *  GLUTBasics
 *
 *  Created by Ben Torkington on 20/05/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

#include "sf2types.h"
#include "sf2macros.h"
#include "sf2const.h"
#include "sf2io.h"

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

#include "projectiles.h"
#include "ryu.h"
#include "ryudata.h"
#include "kendata.h"

extern Game g;

typedef struct KenRyuButtons KRBtns;
typedef struct UserData_RyuKen UD;


void pl_cb_setstatus2_ryu(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_37f1e[status / 2], argd0);
}
void pl_cb_setstatus3_ryu(Player *ply, short status) {
	pl_cb_setstatus2_ryu(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_ryu(Player *ply, short status) {
	pl_cb_setstatus2_ryu(ply, status, 0);
}

void pl_cb_setstatus2_ken(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_51730[status / 2], argd0);
}
void pl_cb_setstatus3_ken(Player *ply, short status) {
	pl_cb_setstatus2_ken(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_ken(Player *ply, short status) {
	pl_cb_setstatus2_ken(ply, status, 0);
}

void PSCBVictoryRyu(Player *ply) {		//2d3bc
	UD *ud=(UD *)&ply->UserData;

	int temp;
	Object *obj;
	switch (ply->mode3) {
		case 0:
			if (ply->x02e6 == 0) {
				if (ply->UserData[0x43]) {
					//2d3e4
					NEXT(ply->mode3);
					ply->UserData[0x43] = 0;
					ply->PSFinishedParticipating = TRUE;
					ply->Flip = ply->EnemyDirection;
					g.FightOver = TRUE;
					temp = sf2rand();
					if (g.OnBonusStage) {
						CASetAnim2(ply, 0x52, temp & 1);
					} else {
						CASetAnim2(ply, 0x52, 
								   (u16 []){0x1082, 0xef7d}[ply->RoundsWon-1] & (1 << (temp & 0xf)) ? 1 : 0
								   /* sorry! */
								   );
					}
				} else {
					ply->UserData[0x43]+=2;
					return;
				}
			} else {
				return;
			}
			/* FALLTHRU */	
		case 2:			
			if (ply->FighterID && ply->RoundsWon == 2 && g.OnFinalStage && g.Pause_9e1 == 2) {
				/* 2d45e */
				NEXT(ply->mode3);
				if ((obj=AllocActor())) {
					obj->exists = TRUE;
					obj->Sel = SF2ACT_0X25;
					obj->SubSel = 9;
					ud->x00d0 = obj;
				}
				if (ply->XPI > get_scr2x() + 0xc0) {
					ply->Flip			= FACING_LEFT;
					ply->EnemyDirection = FACING_LEFT;
				} else {
					ply->Flip			= FACING_RIGHT;
					ply->EnemyDirection = FACING_RIGHT;
				}
				CASetAnim2(ply, 0x56, 0);
				return;
			}
			PLAYERTICK;
			break;
		case 4:		//2d4ac
			if (AF1) {
				NEXT(ply->mode3);
				CASetAnim2(ply, 0, 0);
			}
			PLAYERTICK;
			break;
		case 6:		//2d4c2
			ply->XPI += ply->Flip ? 2 : -2 ;
			obj = ud->x00d0;
			if(obj->XPI - ply->XPI > 0x30 || obj->XPI - ply->XPI <=0x30) {
				PLAYERTICK;
			} else {
				NEXT(ply->mode3);
			}
			break;
		case 8:
			/* does nothing */
			break;
		FATALDEFAULT;
	}
}

#pragma mark Hadouken

void RyuKenStartHadouken(Player *ply) {			//2d766
	NEXT(ply->mode2);
	CASetAnim2(ply, STATUS_HADOUKEN, ply->ButtonStrength/2);
}
// animate until seqence ends, then init projectile
void RyuSMHadouken(Player *ply) {		//2d778
	Object *obj;
	
	if (AF1) {
		NEXT(ply->mode2);
		if ((obj=AllocProjectile())) {
			obj->exists = TRUE;
			obj->Sel	= SF2_PROJ_HADOUKEN;
			obj->XPI	= ply->XPI;
			obj->YPI	= ply->YPI;
			obj->Flip	= ply->Flip;
			obj->SubSel = ply->ButtonStrength;
			obj->Owner	= ply;
			ply->Projectile = obj;
			queuesound(SOUND_HADOUKEN);
		}
		ply->LocalTimer = 40;
	} 
	PLAYERTICK;
}
// hold timer at end
short sub_2d7d2(Player *ply) {
	if (--ply->LocalTimer) {
		PLAYERTICK;
		return TRUE;
	}
	return FALSE;
}
// End Hadouken shared


#pragma mark Shoryuken

/* 2d3a2 apex finder */
int RyuAtApex(Player *ply) {		//2d3a2
	UD *ud=(UD *)&ply->UserData;
	
	ply->X.full += ud->ShoryukenX.full;
	ply->Y.full += ud->ShoryukenY.full;
	ud->ShoryukenY.full += ud->ShoryukenYDash.full;
	
	return ud->ShoryukenYDash.full;
}
/* 2d7f0 Shoryuken Init */
void RyuStartShoryuken(Player *ply) {		//2d7f0
	UD *ud=(UD *)&ply->UserData;
	
	static const short data_2d82c[]={4, 6, 8};
	static const int data_2d83a[]={0x50000, 0x70000, 0x90000};
	
	NEXT(ply->mode2);
	queuesound(0x6a);			   /* Shoryuken! */
	ud->ShoryukenX.part.integer	 = data_2d82c[ply->ButtonStrength/2];
	ud->ShoryukenX.part.fraction = 0;
	ud->ShoryukenXDash.full		 = -0x8000;
	ud->ShoryukenY.full		     = data_2d83a[ply->ButtonStrength/2];
	ud->ShoryukenYDash.full		 = -0x6000;
	
	CASetAnim2(ply, STATUS_SHORYUKEN, ply->ButtonStrength/2);
}
/* 2d84a Shoryuken State Machine */
void RyuSMShoryuken(Player *ply) {		//2d84a
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	
	switch (ply->mode3) {
		case 0:
			if (AF2 == 0) {				// XXX check logic
				NEXT(ply->mode3);
				ply->Airborne = AIR_JUMPING;
			}
			PLAYERTICK;
			break;
		case 2:
			if (ud->ShoryukenX.part.integer < 0) {
				NEXT(ply->mode3);
			} else {
				ply->Y.full += ud->ShoryukenY.full;
				ud->ShoryukenY.full += ud->ShoryukenYDash.full;
				if (ply->Flip != FACING_LEFT) {
					ply->X.full += ud->ShoryukenX.full;
				} else {
					ply->X.full -= ud->ShoryukenX.full;
				}
				ud->ShoryukenX.full += ud->ShoryukenXDash.full;
			}
			PLAYERTICK;
			break;
		case 4:
			ply->Y.full += ud->ShoryukenY.full;
			ud->ShoryukenY.full += ud->ShoryukenYDash.full;
			if (ud->ShoryukenY.full < 0) {
				if (PLAYERGROUND) {
					NEXT(ply->mode3);
					ply->Airborne = AIR_ONGROUND;
					soundsting(SOUND_IMPACT8);
					PLAYERTICK;
					return;
				}
			}
			if ((AF2) == 0) {
				PLAYERTICK;
			}
			break;
		case 6:
			if (AF1) {
				NEXT(ply->mode2);
				ply->mode3 = 0;
			} else {
				PLAYERTICK;
			}
			break;
			FATALDEFAULT;
	}
}


#pragma mark Hurricane
// start moving
void RyuStartHurricane(Player *ply) {		//2d914
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;

	NEXT(ply->mode2);
	ply->mode3=0;
	soundsting(SOUND_HURRICANE);	/* "Don't scratch that veruca!" */
	ud->x00c1 = (ply->ButtonStrength/2)+2;
	ud->ShoryukenX.full = data_2d962[ply->ButtonStrength/2];
	if (ply->Flip==FACING_LEFT) {
		ud->ShoryukenX.full = -ud->ShoryukenX.full;
	}
	ud->ShoryukenY.full = 0x00040000;
	ud->ShoryukenYDash.full = -0x6000;
	ply->Airborne = AIR_JUMPING;
	CASetAnim2(ply, STATUS_HURRICANE , 0);
}
// manage state while in air, then finish
void RyuSMHurricane(Player *ply) {		//2d96e
	UD *ud=(UD *)&ply->UserData;
	
	switch (ply->mode3) {
		case 0:
			if(RyuAtApex(ply)<0){
				NEXT(ply->mode3);
				CASetAnim2(ply, STATUS_HURRICANE, (ply->ButtonStrength/2)+2);
			}
			PLAYERTICK;
			break;
		case 2:
			if (AF1) {
				soundsting(SOUND_SWOOSH3);
				if(--ud->x00c1 < 0){		// XXX check
					NEXT(ply->mode3);
					CASetAnim2(ply, STATUS_HURRICANE, 1);
				}
			}			
			ply->X.full += ud->ShoryukenX.full;
			PLAYERTICK;
			break;
		case 4:
			RyuAtApex(ply);
			if (PLAYERGROUND) {
				NEXT(ply->mode2);
				ply->mode3        = 0;
				ply->Airborne     = AIR_ONGROUND;
				ply->ThrowDisable = data_2d9fe[RAND16];
				if (ply->Human) {
					ply_exit_air(ply);
				}
			} else {
				PLAYERTICK;
			}
			break;

		FATALDEFAULT;
	}
}



