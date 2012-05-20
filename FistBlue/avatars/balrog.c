/*
 *  balrog.c
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

#include "lib.h"
#include "gfxlib.h"

struct UserData_Balrog {
	short	x0080;
	short	x0082;
};
typedef struct UserData_Balrog UD;


#include "balrog.h"

static void sub_36f10(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley = FALSE;
	exit_comp_normal(ply);
}
static void sub_36f1e(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley = FALSE;
	exit_to_compdisp1(ply);
}
static void sub_36f2c(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley = FALSE;
	comp_setjumping_main(ply);
}

static void sub_36f64(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = 0;
					quirkysound(0);
					break;
				case 2:
					ply->Move = (ply->OppXDist < 0x1f ? 1 : 2);
					quirkysound(1);
					break;
				case 4:
					ply->Move = (ply->OppXDist < 0x29 ? 3 : 4);
					quirkysound(2);
					break;
					FATALDEFAULT;
			}
			CASetAnim2(ply, (ply->PunchKick ? 0x42 : 0x40), ply->Move);
			PLAYERTICK;
			break;
		case 2:
			switch (ply->Move) {
				case 0:
					if (AF1) {
						sub_36f10(ply);
					} else {
						if (AF2) {
							if (comp_diceroll(ply)) {
								if (comp_setnextaction(ply)) {
									comp_do_next_action(ply);
									return;
								}
							}
						}
						ply->mode2 = 0;
						PLAYERTICK;
					}
					break;
				case 1:
				case 2:
				case 3:
				case 4:
					if (AF1) {
						sub_36f10(ply);
					} else {
						PLAYERTICK;
					}
					break;
				default:
					break;
			}
		default:
			break;
	}
}
static int sub_37242(Player *ply) {
	if (ply->AISigSpecial) {
		ply->StandSquat = 6;
		return TRUE;
	}
	return FALSE;
}
static void sub_37114(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = 0;
					quirkysound(0);
					break;
				case 2:
				case 4:
					ply->Move = 1;
					quirkysound(1);
					break;
				FATALDEFAULT;
			}
			CASetAnim2(ply, 0x44, ply->Move);
			PLAYERTICK;
			break;
		case 2:
			switch (ply->Move) {
					if (AF1) {
						sub_36f1e(ply);
					} else {
						if (AF2) {
							if (comp_diceroll(ply)) {
								if (comp_setnextaction(ply)) {
									comp_do_next_action(ply);
									return;
								}
							}
						}
						ply->mode2 = 0;
						PLAYERTICK;
					}
					break;
				case 1:
					if (AF1) {
						sub_36f1e(ply);
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_37250(Player *ply) {
	UD *ud = (UD*)&ply->UserData;
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->LocalTimer = (char[]){
						60, 20, 40, 60, 20, 40, 60, 20, 
						40, 60, 20, 40, 20, 40, 60, 40, 
						60, 20, 40, 60, 20, 40, 60, 20,
						40, 60, 20, 40, 20, 40, 60, 40,
					}[RAND32];
					CASetAnim1(ply, 0x48);
					PLAYERTICK;
					break;
				case 2:
					if (AF2) {
						if (ply->LocalTimer) {
							--ply->LocalTimer;
						} else {
							NEXT(ply->mode2);
							quirkysound(2);
							PLAYERTICK;
						}
					} else {
						PLAYERTICK;
					}
					break;
				case 4:
					if (AF1) {
						sub_36f10(ply);
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case PLY_KICKING:
		case 4:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->VelX.full = 0x0800;
					if (ply->Flip == 0) {
						ply->VelX.full = -ply->VelX.full;
					}
					ply->AclX.full = 0;
					ply->VelY.full = 0;
					ply->AclY.full = 0;
					ud->x0080 = (short []){0x40,0x80,0xc0}[ply->ButtonStrength/2];
					ud->x0082 = ply->XPI;
					CASetAnim2(ply, (ply->PunchKick == 4 ? 0x4c: 0x4a), ply->ButtonStrength/2);
					quirkysound(2);
					CATrajectory((Object *)ply);
					break;
				case 2:
					if (ply->BoundCheck) {
						CATrajectory((Object *)ply);
					} else {
						if (ud->x0080 > ((ply->Flip ? 1 : -1) * ply->XPI - ud->x0082) 
							&& ply->OppXDist > 0x50) {
							CATrajectory((Object *)ply);
						} else {
							NEXT(ply->mode2);
							ply->AclX.full = 0x00a0;
							if (ply->Flip == 0) {
								ply->AclX.full = -ply->AclX.full;
							}
							CATrajectory((Object *)ply);
							PLAYERTICK;
						}
					}
					break;
				case 4:
					if ((ply->Flip && ply->VelX.full < 0) 
						||(ply->Flip == 0 && ply->VelX.full >= 0)) {
						if (AF1) {
							sub_36f10(ply);
						} else {
							PLAYERTICK;
						}
					} else {
						CATrajectory((Object *)ply);
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}
int PLCBCompJumpBalrog (Player *ply) {			//371c2
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = (ply->VelX.full ? 0 : 2);
					quirkysound(0);
					break;
				case 2:
				case 4:
					ply->Move = (ply->VelX.full ? 1 : 3);
					quirkysound(1);
					break;
					FATALDEFAULT;
			}
			CASetAnim2(ply, 0x46, ply->Move);
			break;
		case PLY_KICKING:
			/* nothing */
			break;
			FATALDEFAULT;
	}
	return 0;	
}
void PLCBCompAttackBalrog (Player *ply) {		// 36f3a
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		if (ply->mode2 == 0) {
			sub_37242(ply);
		}
		switch (ply->StandSquat) {
			case PLY_STAND:	sub_36f64(ply);	break;
			case 2:			sub_37114(ply);	break;
			case 4:			PLCBCompJumpBalrog(ply); break;
			case 6:			sub_37250(ply); break;
			FATALDEFAULT;
		}
	}		
}
