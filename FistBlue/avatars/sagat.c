/*
 *  sagat.c
 *  MT2
 *
 *  Created by Ben on 20/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
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
#include "projectiles.h"
#include "lib.h"
#include "gfxlib.h"


#include "sagat.h"


int PLCBCompJumpSagat (Player *ply) {		// 36cd6
	quirkysound(ply->ButtonStrength / 2);
	if (ply->VelX.full == 0) {
		ply->Move = ply->ButtonStrength;
	} else {
		ply->Move = ply->ButtonStrength + 1;
	}
	CASetAnim2(ply, STATUS_JUMP_PUNCH, ply->Move);
	return TRUE;
}

static int sub_36d00(Player *ply) {
	if (ply->AISigSpecial) {
		if (ply->PunchKick == 4 || ply->Projectile == NULL) {
			ply->StandSquat = 8;
			return TRUE;
		}
	}
	return FALSE;
}
static void sub_36c9c(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley    = FALSE;
	exit_comp_normal(ply);
}
static void sub_36cc8(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley    = FALSE;
	exit_to_compdisp1(ply);
}
static void sub_36c50(Player *ply) {
	if (ply->mode2 == 0) {
		ply->Move = ply->ButtonStrength / 2;
		setstatus4(ply, (ply->PunchKick ? 0x44 : 0x42));
	} else {
		if (AF1) {
			sub_36c9c(ply);
		} else {
			PLAYERTICK;
		}
	}
}
static void sub_36caa(Player *ply) {
	if (ply->mode2 == 0) {
		ply->Move = 0;
		setstatus4(ply, 0x46);
	} else {
		if (AF1) {
			sub_36cc8(ply);
		} else {
			PLAYERTICK;
		}
	}
}
inline static void sub_36cfc(Player *ply) {
	/* does nothing */
}
static void sub_36d20(Player *ply) {
	Object *obj;
	switch (ply->mode2) {
		case 0:
			//suicide code removed
			NEXT(ply->mode2);
			CASetAnim2(ply, STATUS_JUMP_KICK, ply->PunchKick / 2);			
			break;
		case 2:
			if (AF1) {
				NEXT(ply->mode2);
				if ((obj = AllocProjectile())) {
					obj->exists     = TRUE;
					obj->Sel        = SF2_PROJ_TIGER;
					obj->XPI        = ply->XPI;
					obj->YPI        = ply->YPI;
					obj->Flip       = ply->Flip;
					obj->SubSel     = ply->ButtonStrength;
					obj->UserByte   = ply->PunchKick;
					obj->Owner      = ply;
					ply->Projectile = obj;
					soundsting(SOUND_TIGER);
				}
				ply->LocalTimer = 0x19;
			} 
			PLAYERTICK;
			break;
		case 4:
			if (--ply->LocalTimer == 0) {
				sub_36c9c(ply);
			} else {
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}

void PLCBCompAttackSagat (Player *ply) {	// 36c24
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		if (ply->mode2 == 0) {
			sub_36d00(ply);
		}
		switch (ply->StandSquat) {
			case 0:		sub_36c50(ply);	break;
			case 2:		sub_36caa(ply); break;
			case 4:		PLCBCompJumpSagat(ply); break;
			case 6:		sub_36cfc(ply); break;
			case 8:		sub_36d20(ply); break;
				FATALDEFAULT;
		}
	}
}

