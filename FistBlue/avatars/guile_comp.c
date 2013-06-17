/*
 *  guile_comp.c
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
#include "lib.h"

#include "projectiles.h"
#include "guile.h"


extern Game g;
typedef struct UserDataComp_Guile UDCOMP;

static void sub_33b3e(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley = FALSE;
	comp_setjumping_main(ply);		// 2c534
}

/*
 static void sub_33b30(Player *ply) {
 ply->AISigAttack = FALSE;
 ply->AIVolley = FALSE;
 exit_to_compdisp1(ply);
 }
 */

static void sub_33b22(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley = FALSE;
	exit_comp_normal(ply);
}



static void sub_3438a(Player *ply) {
	if (ply->AISigSpecial) {
		if (ply->PunchKick || ply->Projectile == NULL) {
			ply->StandSquat = 6;
		} 
	}
}
static void guile_comp_punch(Player *ply) {	// 33b86
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			switch (ply->ButtonStrength) {
				case 0:		// 33bf2
					ply->Move = ply->OppXDist >= 20 ? 1 : 0;
					quirkysound(0);
					break;
				case 2:
					if (ply->CompDoThrow) {
						PLY_THROW_SET(-24, 0x0035, 24, 16);
						if (throwvalid(ply)) {
							ply->Move = 6;
							return;
						}
					}
					ply->Move = ply->OppXDist >= 40 ? 3 : 2;
					quirkysound(1);
					break;
				case 4:
					if (ply->CompDoThrow) {
						PLY_THROW_SET(-24, 0x0035, 24, 16);
						if (throwvalid(ply)) {
							ply->Move = 7;
							return;
						}
					}
					ply->Move = ply->OppXDist >= 0x3d ? 5 : 4;
					quirkysound(2);
					break;
					FATALDEFAULT;
			}
			if (ply->Move == 7) {
				ply->Flip = ply->IsWithinBounds ? 1 : 0;
			}
			else if (ply->Move >= 6) {
				ply->Flip = ply->IsWithinBounds ? 0 : 1;
			}
			/* 33bdc */
			CASetAnim2(ply, STATUS_PUNCH, ply->Move);
			PLAYERTICK;
			break;
		case 2:
			/* 33c9e */
			switch (ply->Move) {
				case 0:
				case 1:
					if (AF1) {
						ply_exit_stand(ply);
						return;
					} else if (AF2) {
						if (comp_diceroll(ply)) {
							if(comp_setnextaction(ply)) {
								comp_do_next_action(ply);
								return;
							}
							ply->mode2 = 0;
						}
					}
					PLAYERTICK;
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					if (AF1) {
						ply_exit_stand(ply);
					} else {
						PLAYERTICK;
					}
					break;
				case 6:					/* 33cf8 */
					switch (ply->mode3) {
						case 0:
							if (AF2) {
								NEXT(ply->mode3);
								set_throw_trajectory(ply, 0, ply->Flip ^ 1, 13);
							} else {
								PLAYERTICK;
							}
							break;
						case 2:
							if (AF1) {
								ply->mode3 = 0;
								ply->Flip ^= 1;
								ply->EnemyDirection = ply->Flip;
								ply_exit_stand(ply);
							} else {
								PLAYERTICK;
							}
							break;
							FATALDEFAULT;
					}
					break;
				case 7:
					/* 33d46 */
					switch (ply->mode3) {
						case 0:
							if (AF2) {
								ply->Timer2 = 12;
								ply->Opponent->Timer2 = 12;
								NEXT(ply->mode3);
								sub_36d6(ply, ply->Flip ? 32 : -32, 0, 2, ply->Flip, 13, 0x2e, 2);
								ActStartScreenWobble();
							} else {
								PLAYERTICK;
							}
							break;
						case 2:
							if (AF1) {
								ply->mode3 = 0;
								ply->EnemyDirection = ply->Flip;
								ply_exit_stand(ply);
							} else {
								PLAYERTICK;
							}
							break;
							FATALDEFAULT;
					}
					
					break;
					
				default:
					break;
			}
			break;
			FATALDEFAULT;
	}
	
}
static void guile_comp_kick(Player *ply) {		//33db8
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = 0;
					quirkysound(0);
					break;
				case 2:
					if (ply->CompDoThrow && ply->OppXDist <= 0x33) {
						ply->Move = 5;
					} else {
						ply->Move = ply->OppXDist >= 0x26 ? 2 : 1;
					}
					quirkysound(1);
					break;
				case 4:
					if (ply->CompDoThrow && ply->OppXDist <= 0x47) {	// XXX check logic
						ply->Move = 6;
					} else {
						ply->Move = ply->OppXDist >= 40 ? 4 : 3;
					}
					quirkysound(2);
					break;
				default:
					break;
			}
			CASetAnim2(ply, STATUS_KICK, ply->Move);
			PLAYERTICK;
			break;
		case 2:		//33e6c
			switch (ply->Move) {
				case 0:
					if (AF1) {
						ply_exit_stand(ply);
						return;
					}
					else if (AF2) {
						if (comp_diceroll(ply)) {
							if(comp_setnextaction(ply)) {
								comp_do_next_action(ply);
								return;
							}
							ply->mode2 = 0;
						}
					}
					PLAYERTICK;
					break;
				case 1 ... 6:
					if (AF1) {
						ply_exit_stand(ply);
					} else {
						PLAYERTICK;
					}
					break;
					FATALDEFAULT;
			}
			FATALDEFAULT;
	}
}
static void guile_comp_crouch_punch(Player *ply) {		// 33ed4 
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			/* was a switch statement with only three cases */
			ply->Move = ply->ButtonStrength / 2;	
			quirkysound(ply->ButtonStrength / 2);
			CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
			break;
		case 2:
			switch (ply->Move) {
				case 0:
					if (AF1) {
						ply_exit_crouch(ply);
						return;
					} else if (AF2) {
						if (comp_diceroll(ply)) {
							if (comp_setnextaction(ply)) {
								comp_setnextaction(ply);
								return;
							}
							ply->mode2 = 0;
						}
					} 
					PLAYERTICK;
					break;
				case 1:
				case 2:
					if (AF1) {
						ply_exit_crouch(ply);
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
static void guile_comp_crouch_kick(Player *ply) {	// 33f84
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			switch (ply->ButtonStrength) {
				case 0: ply->Move=0; quirkysound(0); break;
				case 2: ply->Move=1; quirkysound(1); break;
				case 4: ply->Move=2; break;
					FATALDEFAULT;
			}
			CASetAnim2(ply, STATUS_CROUCH_KICK, ply->Move);
			break;
		case 2:
			switch (ply->Move) {
				case 0:
					if (AF1) {
						ply_exit_crouch(ply);
						return;
					} else if (AF2) {
						if (comp_diceroll(ply)) {
							if (comp_setnextaction(ply)) {
								comp_do_next_action(ply);
								return;
							}
							ply->mode2 = 0;
						}
					} 
					PLAYERTICK;
					break;
				case 1:
				case 2:
					if (AF2 > 0) {
						if (AF2 == 1) {
							quirkysound(2);
						} else {
							quirkysound(1);
						}
					}
					if (AF1) {
						ply_exit_crouch(ply);
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
static void guile_comp_jump_punch(Player *ply) {		//341ea, bad name
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->Timer2 = 12;
			ply->Flip = ply->IsWithinBounds ? 1 : 0;
			ply->EnemyDirection = ply->Flip;
			CASetAnim2(ply, 0x52, ply->Move);
			CATrajectory((Object *)ply);
			if (ply->VelY.full < 0) {
				if (PLAYERGROUND) {
					ply->mode2 = 6;
					CASetAnim2(ply, 0x54, ply->Move);
				}
			}
			break;
		case 2:		// 34250
			PLAYERTICK;
			if (AF1) {
				NEXT(ply->mode2);
				set_throw_trajectory(ply, 8, ply->Flip, 15);
			}
			CATrajectory((Object *)ply);
			if (ply->VelY.full < 0) {
				if (PLAYERGROUND) {
					ply->mode2 = 6;
					CASetAnim2(ply, 0x54, ply->Move);
				}
				
			}
			break;
		case 4:		// 3428c
			if (ply->VelY.full > 0  ) {
				CATrajectory((Object *)ply);
			} else if (PLAYERGROUND == 0) {
				CATrajectory((Object *)ply);
			} 
			ply->CompDoJump = FALSE;
			sub_33b3e(ply);
			break;
		case 6:
			PLAYERTICK;
			if (AF1) {
				set_throw_trajectory(ply, 0, ply->Flip, 0x20);
				sub_33b3e(ply);
			}
			break;
		default:
			break;
	}
}
static void sub_342c6(Player *ply) {
	switch (ply->mode2) {
		case 0:
			// 342da
			NEXT(ply->mode2);
			ply->Timer2 = 12;
			ply->Flip = ply->IsWithinBounds ? 0 : 1;
			ply->EnemyDirection = ply->Flip;
			if (ply->VelY.full > 0) {
				ply->VelY.full = 0;
			}
			ply->VelX.full = 0;
			CASetAnim2(ply, 0x52, ply->Move);
			CATrajectory((Object *)ply);
			break;
		case 2:
			PLAYERTICK;
			CATrajectory((Object *)ply);
			if(PLAYERGROUND) {
				NEXT(ply->mode2);
				CASetAnim1(ply, 0x56);
			}
			break;
		case 4:
			NEXT(ply->mode2);
			ply->Timer2 = 12;
			ply->Opponent->Timer2 = 0xc;
			
			sub_36d6(ply, ply->Flip ? 16 : -16, 50, 10, ply->Flip, 15, 0x2e, 2);
			ActStartScreenWobble();
			break;
		case 6:
			if (AF1) {
				ply->CompDoJump = FALSE;
				sub_33b22(ply);
			}
			break;
		default:
			break;
	}
	
}
static void sub_343b4(Player *ply) {
	Object *obj;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CASetAnim2(ply, 0x4c, ply->ButtonStrength / 2);
			queuesound(0x70);
			break;
		case 2:
			if ((ply->AnimFlags & 0xff00) == 0x100) {
				if ((obj = AllocProjectile())) {
					INITOBJC(obj, SF2_PROJ_SONICBOOM, ply->ButtonStrength, PLAYERX, PLAYERY);
					obj->Flip = ply->Flip;
					obj->Owner = ply;
					ply->Projectile = obj;
				}
				NEXT(ply->mode2);
			}
			PLAYERTICK;
			break;
		case 4:
			if (AF1) {
				NEXT(ply->mode2);
				ply->LocalTimer = 15;
			}
			PLAYERTICK;
			break;
		case 6:
			if (--ply->LocalTimer == 0) {
				sub_33b22(ply);
			} else {
				PLAYERTICK;
			}
			break;
			FATALDEFAULT;
	}
}
static void sub_3444e(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CASetAnim2(ply, 0x4e, ply->ButtonStrength / 2);
			quirkysound(2);
			break;
		case 2:
			NEXT(ply->mode2);
			ply->VelX.full = 0x180;		/* redundant array lookup */
			ply->AclX.full = 0;
			if (ply->Flip == 0) {
				ply->VelX.full = -ply->VelX.full;
			}
			ply->VelY.full  = (short []){0x800,0x900,0xa00}[ply->ButtonStrength / 2];
			ply->AclY.full = 0x60;	/* redundant array */
			ply->Airborne = AIR_JUMPING;
			PLAYERTICK;
			break;
		case 4:
			if ((ply->AnimFlags & 0xff00) == 0x100) { /* oh shiii... */
				PLAYERTICK;
			} else {
				CATrajectory((Object *)ply);
				if (ply->VelY.full < 0) {
					if (PLAYERGROUND) {
						ply->Airborne = AIR_ONGROUND;
						sub_33b3e(ply);
					}
				} else {
					PLAYERTICK;
				}
			}
			break;
		default:
			break;
	}
}

void PLCBCompAttackGuile(Player *ply) {		/* 33b4c guile comp attack cb */
	if (ply->Timer2) {
		--ply->Timer2;
		return;
	}
	if (ply->mode2 == 0) {
		/* 3438a */
		sub_3438a(ply);
	}
	switch (ply->StandSquat) {
		case PLY_STAND:
			/* 33b76 */
			switch (ply->PunchKick) {
				case PLY_PUNCHING:
					guile_comp_punch(ply);
					break;
				case PLY_KICKING:
					guile_comp_kick(ply);
					break;
					FATALDEFAULT;
			}
			break;
		case PLY_CROUCH:
			switch (ply->PunchKick) {
				case PLY_PUNCHING:
					guile_comp_crouch_punch(ply);
					break;
				case PLY_KICKING:
					guile_comp_crouch_kick(ply);
					break;
					FATALDEFAULT;
			}
			break;
		case 4:
			//341ea
			switch (ply->PunchKick) {
				case 0:
					guile_comp_jump_punch(ply);
					break;
				case PLY_KICKING:
					sub_342c6(ply);
					break;
					FATALDEFAULT;
			}
			break;
		case 6:
			//343a4
			switch (ply->PunchKick) {
				case PLY_PUNCHING:
					sub_343b4(ply);
					break;
				case PLY_KICKING:
					sub_3444e(ply);
					break;
					FATALDEFAULT;
			}
			break;
			FATALDEFAULT;
	}
	
}

static short sub_3414c(Player *ply) {
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			if (ply->ButtonStrength) {
				PLY_THROW_SET(0xffe2, 0x0038, 0x0025, 0x0024);
				if (airthrowvalid(ply)) {
					ply->Move = 0;
					ply->mode1 = 0xa;
					return -1;
				}
			}
			return 0;
			break;
		case PLY_KICKING:
			if (ply->ButtonStrength) {
				PLY_THROW_SET(0xffe2, 0x0038, 0x0025, 0x0024);
				if (airthrowvalid(ply)) {
					ply->Move = 1;
					ply->mode1 = 0xa;
					return -1;
				}
				return 0;
				
			}
			break;
			FATALDEFAULT;
	}
	return 0;
}

static short sub_341c6(Player *ply) {
	short y = ply->YPI - 0x28;
	if (ply->VelY.full < 0) {
		if (y<30) { return FALSE; }
	} else {
		if (y<20) {	return FALSE; }
	}
	return TRUE;
}

static short sub_3413e(Player *ply) {
	if (ply->CompDoThrow) {
		if (sub_341c6(ply)) {
			return sub_3414c(ply);
		}
	}
	return 0;
}

int PLCBCompJumpGuile(Player *ply) {		// 3404a
	short temp;
	temp = sub_3413e(ply);
	if(temp < 0) {
		return temp;
	}
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			// 34066
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = ply->VelX.full == 0 ? 3 : 0;
					quirkysound(0);
					break;
				case 2:
					ply->Move = ply->VelX.full == 0 ? 4 : 1;
					quirkysound(1);
					break;
				case 4:
					ply->Move = ply->VelX.full == 0 ? 5 : 2;
					quirkysound(2);
					break;
					FATALDEFAULT;
			}
			CASetAnim2(ply, STATUS_JUMP_PUNCH, ply->Move);
			break;
		case PLY_KICKING:
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = ply->VelX.full == 0 ? 0 : 2;
					quirkysound(0);
					break;
				case 2:
					ply->Move = ply->VelX.full == 0 ? 1 : 3;
					quirkysound(1);
					break;
				case 4:
					ply->Move = ply->VelX.full == 0 ? 4 : 5;
					quirkysound(2);
					break;
					FATALDEFAULT;
			}
			CASetAnim2(ply, STATUS_JUMP_KICK, ply->Move);
			break;
			FATALDEFAULT;
	}
	return 0;
}
