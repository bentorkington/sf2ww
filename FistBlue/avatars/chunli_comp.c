/*
 *  chunli_comp.c
 *  MT2
 *
 *  Created by Ben on 25/05/12.
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
#include "sf2io.h"

#include "lib.h"
#include "gfxlib.h"

#include "chunli.h"

typedef struct UserDataComp_ChunLi UDCOMP;
extern Game g;



static void sub_34d9a(Player *ply) {		// 34d9a
	CASetAnim2(ply, 
			   (short []){
				   STATUS_PUNCH, STATUS_KICK, 
				   STATUS_CROUCH_PUNCH, STATUS_CROUCH_KICK
			   }[ply->StandSquat + ply->PunchKick/2],
			   ply->Move
			   );
}
static void sub_34dba(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	if (ply->OppXDist < ud->comp_distance) {	
		ply->Move = ud->comp_closemove;
	} else {
		ply->Move = ud->comp_farmove;
	}
	sub_34d9a(ply);
}
static void sub_34d5e(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	if (AF1) {
		if (ply->ActionScript->Crouch) {
			//34dea
			ply->AISigAttack = FALSE;
			ply->AIVolley = FALSE;
			exit_to_compdisp1(ply);
		} else {
			ply->AISigAttack = FALSE;
			ply->AIVolley = FALSE;
			exit_comp_normal(ply);
		}
	} else if (AF2 && ud->comp_volley && comp_diceroll(ply)) {
		if (comp_setnextaction(ply)) {
			comp_do_next_action(ply);
		} else {
			quirkysound(ply->ButtonStrength / 2);
			sub_34dba(ply);
		}
	} else {
		PLAYERTICK;
	}
}
static void sub_34ddc(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley    = FALSE;
	exit_comp_normal(ply);
}
static void sub_34dea(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley    = FALSE;
	exit_to_compdisp1(ply);
}
static void sub_34dd2(Player *ply) {
	if (ply->ActionScript->Crouch) {
		sub_34dea(ply);
	} else {
		sub_34ddc(ply);
	}
}
static void sub_34874(Player *ply) {
	PLAYERTICK;
	if (AF2 < 0) {
		NEXT(ply->mode2);
		ply->EnemyDirection = ply->Flip;
		set_throw_trajectory(ply, 12, ply->Flip, 14);
	}
}	
static void sub_348a4(Player *ply) {
	if (--ply->LocalTimer == 0) {
		NEXT(ply->mode2);
	}
}
static void sub_348b0(Player *ply) {
	PLAYERTICK;
	if (AF1) {
		sub_34ddc(ply);
	}
}
static void sub_347ec(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	if (ply->CompDoThrow && ply->OppXDist <= 35) {
		PLY_THROW_SET(0xffe0, 0x35, 0x20, 0x10);
		if (throwvalid(ply)) {
			ply->mode2 = 4;
			ply->Flip  = ply->IsWithinBounds;
			ply->Move  = 7;
			CASetAnim2(ply, STATUS_PUNCH, ply->Move);
			queuesound(SOUND_CHUNLI_YUP);
			return;
		}
	}
	NEXT(ply->mode2);
	CLM(4, 5, 0, 0x29);
	quirkysound(2);
	sub_34dba(ply);
}	
static void sub_34ec0(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	ud->h008a.part.p0 = ud->h0086.part.p0;
	CASetAnim2(ply, (short []){0x54, 0x5a, 0x5c}[ud->comp_select], ud->h008a.part.p1);
	if (AF1) {
		queuesound(SOUND_PUNCH1);
	}
	CATrajectory((Object *)ply);
}	
static void sub_34f00(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	if (--ud->h008a.part.p0 == 0) {
		++ud->h008a.part.p1;
		ud->h008a.part.p1 &= 7;
		ud->h0086.full -= ud->comp_bkaccel;
		if (ud->h0086.full < 0x100) {
			NEXT(ply->mode2);
			ud->h0086.full = 0x100;
		}
		sub_34ec0(ply);
	} else {
		CATrajectory((Object *)ply);
	}
	
}


// Standing Punches
static void sub_3470e(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CLM(0, 1, 1, 0x15);
			quirkysound(0);
			sub_34dba(ply);
			break;
		case 2:
			sub_34d5e(ply);
			break;
			FATALDEFAULT;
	}
}
static void sub_34744(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			if (ply->CompDoThrow && ply->OppXDist <= 35) {
				PLY_THROW_SET(0xffe0, 0x35, 0x20, 0x10);
				if (throwvalid(ply)) {
					ply->mode2 = 4;
					ply->Flip = ply->IsWithinBounds;
					ply->Move = 7;
					CASetAnim2(ply, STATUS_PUNCH, ply->Move);
					queuesound(SOUND_CHUNLI_YUP);
					return;
				}
			}
			NEXT(ply->mode2);
			CLM(4, 5, 0, 0x29);
			quirkysound(2);
			sub_34dba(ply);
			break;
		case 2:
			sub_34d5e(ply);
			break;
		case 4:
			NEXT(ply->mode2);
			CASetAnim2(ply, STATUS_PUNCH, ply->Move);
			break;
		case 6:
			sub_34874(ply);
			break;
		case 8:
			sub_348a4(ply);
			break;
		case 10:
			sub_348b0(ply);
			break;
			FATALDEFAULT;
	}
}
static void sub_347d4(Player *ply) {
	switch (ply->mode2) {
		case 0:				//347ec
			sub_347ec(ply);
			break;
		case 2:
			sub_34d5e(ply);
			break;
		case 4:
			NEXT(ply->mode2);
			CASetAnim2(ply, STATUS_PUNCH, ply->Move);
			break;
		case 6:
			sub_34874(ply);
			break;
		case 8:
			sub_348a4(ply);
			break;
		case 10:
			sub_348b0(ply);
			break;
			FATALDEFAULT;
	}
	
}
// Standing Kicks
static void sub_348d0(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CLM(0, 0, 0, 0x24);
			
			quirkysound(0);
			sub_34dba(ply);
			break;
		case 2:
			sub_34d5e(ply);
			break;
			FATALDEFAULT;
	}
}
static void sub_34906(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CLM(2, 4, 0, 0x29);
			quirkysound(1);
			sub_34dba(ply);
			break;
		case 2:
			sub_34d5e(ply);
			break;
		case 4:
			NEXT(ply->mode2);
			CASetAnim2(ply, STATUS_KICK, ply->Move);
			break;
		case 6:
			PLAYERTICK;
			if (AF2 == 0) {
				NEXT(ply->mode2);
				PLY_TRAJ0(ply->Flip ? 0x180 : -0x180, 0x780, 0, 0x68);
				ply->Airborne = AIR_JUMPING;
				ply->YPI += 0x10;
				quirkysound(1);
			}
			break;
		case 8:
			CATrajectory((Object *)ply);
			if (PLAYERGROUND) {
				ply->Airborne = AIR_ONGROUND;
				queuesound(SOUND_IMPACT8);
				sub_34ddc(ply);
			} else {
				PLAYERTICK;
			}
			break;
			FATALDEFAULT;
	}
}
static void sub_349c0(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CLM(5, 7, 0, 0x29);
			quirkysound(2);
			sub_34dba(ply);
			break;
		case 2:
			sub_34d5e(ply);
			break;
		case 4:			//34a06
			NEXT(ply->mode2);
			PLY_TRAJ0(ply->Flip ? 0x180 : -0x180, 0x780, 0, 0x50);
			quirkysound(2);
			ply->Airborne = AIR_JUMPING;
			ply->YPI += 16;
			CASetAnim2(ply, STATUS_KICK, ply->Move);
			break;
		case 6:
			CATrajectory((Object *)ply);
			if (PLAYERGROUND) {
				ply->Airborne = AIR_ONGROUND;
				ply->Flip ^= 1;
				quirkysound(0x2f);
				sub_34ddc(ply);
			} else {
				PLAYERTICK;
			}
			break;
		default:
			break;
	}
}
//Crouching Punches & Kicks gone, synthesised
static void sub_34e0e(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			queuesound(SOUND_BIRD_KICK);
			ud->comp_select = ply->ButtonStrength / 2;
			CASetAnim2(ply, 0x52, ud->comp_select);
			queuesound(SOUND_BIRD_KICK);
			/* FALLTHRU */
		case 2:
			if (AF1) {
				NEXT(ply->mode2);
				ud->h008a.part.p1 = 0;
				ud->h0086.full = (short []){0x400, 0x480, 0x500}[ud->comp_select];
				ud->comp_bkaccel = (short []){0x46, 0x50, 0x5c}[ud->comp_select];
				ud->h0088s = ud->h0086.full;
				ud->comp_bktimer = (short []){0x8, 0x10, 0x28}[ud->comp_select];
				BumpDiff_PowerMove();
				PLY_TRAJ0((ply->Flip ? 0x380 : -0x380), 0x400, 0, 0x60);
			}
			PLAYERTICK;
			break;
		case 4:
			NEXT(ply->mode2);
			sub_34ec0(ply);
			break;
		case 6:
			if (ply->VelY.full < 0) {
				ply->VelY.full = 0;
				ply->AclY.full = 0;
			}
			sub_34f00(ply);
			break;
		case 8:
			if (--ud->comp_bktimer == 0) {
				NEXT(ply->mode2);
				ply->LocalTimer = 12;
				CATrajectory((Object *)ply);
			} else {
				sub_34f00(ply);
				ply->mode2 -= 2;
			}
			break;
		case 10:
			if (--ply->LocalTimer < 0) {
				if (PLAYERGROUND) {
					NEXT(ply->mode2);
					CASetAnim1(ply, 0x56);
					PLAYERTICK;
					return;
				}
				ply->VelY.full = 0xfc00;
				ply->AclY.full = 0xffa0;
			}
			if (--ud->h008a.part.p0 == 0) {
				++ud->h008a.part.p1;
				ud->h008a.part.p1 &= 7;
				ud->h0086.full += ud->comp_bkaccel;
				if (ud->h0086.full > ud->h0088s) {
					ud->h0086.full = ud->h0088s;
				}
				sub_34ec0(ply);
			} else {
				CATrajectory((Object *)ply);
			}
			break;
		case 12:
			if (AF1) {
				queuesound(SOUND_IMPACT8);
				ply->Airborne = AIR_ONGROUND;
				sub_34ddc(ply);
			} else {
				PLAYERTICK;
			}
			break;
			FATALDEFAULT;
	}
}


static void _ChunLiStandComp(Player *ply) {		//346ec
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->ButtonStrength) {
				case 0:	sub_3470e(ply);	break;
				case 2:	sub_34744(ply);	break;
				case 4:	sub_347d4(ply);	break;
					FATALDEFAULT;
			}
			break;
		case PLY_KICKING:		//348be
			switch (ply->ButtonStrength) {
				case 0:	sub_348d0(ply);	break;
				case 2:	sub_34906(ply);	break;
				case 4:	sub_349c0(ply);	break;
					FATALDEFAULT;
			}
			break;
			FATALDEFAULT;
	}
}
static void _ChunLiCrouchComp(Player *ply) {		//34a6c gone, synthesised
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
	static const char moves[3][4]=
	{         
		{ 0, 0, 1, 0 }, { 1, 1, 0, 0 }, { 2, 2, 0, 0 }
	};
	
	
	const char *cl = moves[ply->ButtonStrength / 2];
	switch (ply->mode2) {		/* Synthetic */
		case 0:
			NEXT(ply->mode2);
			CLM(cl[0], cl[1], cl[2], cl[3]);
			quirkysound(ply->ButtonStrength / 2);
			sub_34dba(ply);
			break;
		case 2:
			sub_34d5e(ply);
			break;
			FATALDEFAULT;
	}
}

void PLCBCompAttackChunLi(Player *ply) {		//346be
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
	if (ply->Timer2) {
		if (--ply->Timer2) {
			ud->x0092 = ply->Timer2;
		}
	} else {
		if (ply->AISigSpecial) {
			//34e06
			if (ply->PunchKick) {					//34fc4
				switch (ply->mode2) {				// Lightning kick
					case 0:
						NEXT(ply->mode2);
						ud->comp_multicount = ply->AIMultiCount;
						CASetAnim2(ply, 0x58, ply->ButtonStrength/2);
						break;
					case 2:
						if (AF1) {
							queuesound(SOUND_CHUNLI_YUP);
						}
						if (g.RoundResult) {
							sub_34ddc(ply);
						} else if (--ud->comp_multicount == 0) {
							sub_34ddc(ply);
						} else {
							PLAYERTICK;
						}
						break;
						FATALDEFAULT;
				}
			} else {
				sub_34e0e(ply);
				
			}
		} else {
			switch (ply->StandSquat) {
				case 0:	_ChunLiStandComp(ply);			break;
				case 2:	_ChunLiCrouchComp(ply);			break;
				case 4:
					//XXX sub_34be4(ply); todo
					break;
					FATALDEFAULT;
			}
		}
	}
}
static int sub_34572(Player *ply) {
	short y = ply->YPI - 40;
	
	if (y < 0 || y >= 48) {
		return TRUE;
	}
	return FALSE;
}

static int sub_3452e(Player *ply) {
	if (ply->PunchKick == PLY_PUNCHING 
		&& ply->CompDoThrow 
		&& ply->ButtonStrength != 0
		&& sub_34572(ply) != 0) {
		
		PLY_THROW_SET(0xffe2, 0x0038, 0x0025, 0x0024);
		if (airthrowvalid(ply)) {
			ply->mode1 = 0xa;
			return -1;
		}
	}
	return 0;
}

int PLCBCompJumpChunLi(Player *ply) {		// 34596
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	int retval = sub_3452e(ply);
	if (retval < 0) {
		return retval;
	}
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->ButtonStrength) {
				case 0:
					CASetAnim2(ply, STATUS_JUMP_PUNCH, (ply->VelX.full ? 3 : 0));
					quirkysound(0);
					break;
				case 2:
					CASetAnim2(ply, STATUS_JUMP_PUNCH, (ply->VelX.full ? 4 : 1));
					quirkysound(1);
					break;
				case 4:
					CASetAnim2(ply, STATUS_JUMP_PUNCH, (ply->VelX.full ? 5 : 2));
					quirkysound(2);
					break;
					FATALDEFAULT;
			}
			break;
		case PLY_KICKING:
			ud->comp_didairthrow = 0;
			switch (ply->ButtonStrength) {
				case 0:
					CASetAnim2(ply, STATUS_JUMP_KICK, (ply->VelX.full ? 5 : 1));
					quirkysound(0);
					break;
				case 2:
					if (ply->CompDoAirThrow) {
						ud->comp_didairthrow = 1;
						ud->x0092 = 0;	
						CASetAnim2(ply, STATUS_JUMP_KICK, (ply->VelX.full ? 8 : 1));
						quirkysound(1);
					} else {
						CASetAnim2(ply, STATUS_JUMP_KICK, (ply->VelX.full ? 6 : 3));
						quirkysound(1);						
					}
					break;
				case 4:
					CASetAnim2(ply, STATUS_JUMP_KICK, (ply->VelX.full ? 7 : 4));
					quirkysound(2);
					break;
					FATALDEFAULT;
			}
			if (ud->comp_didairthrow) {
				ply->mode1 = 0xa;
				return -1;
			}
			return 0;
			break;
			FATALDEFAULT;
	}
	return 0;
}

