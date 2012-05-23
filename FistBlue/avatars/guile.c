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

#define QUICKMOVE(MOVE_ID)      \
ply->Move = MOVE_ID;			\
quirkysound(MOVE_ID);			\




#define STDANIM(ARG_A,EXITROUTINE)					\
switch (ply->mode2) {				\
	case 0:							\
		NEXT(ply->mode2);			\
		CASetAnim2(ply, ARG_A, ply->Move);	\
		break;						\
	case 2:							\
		if (AF1) {					\
			EXITROUTINE(ply);	\
		} else {					\
			PLAYERTICK;				\
		}							\
		break;						\
	default:						\
		break;						\
}

#define STDPUNCHKICK(A,B)			\
switch (ply->PunchKick) {			\
	case PLY_PUNCHING:				\
		A(ply);						\
		break;						\
	case PLY_KICKING:				\
		B(ply);						\
		break;						\
}

extern Game g;


static void _GuileSMSonic(Player *ply);
static void _GuileSMBlade(Player *ply);		// 2f216

static short _GuileStartSonic(Player *ply);		/* 2f1ea */
static short _GuileStartBlade(Player *ply);
static short GuileStandMove(Player *ply);
static short GuileCrouchMove(Player *ply);
static short GuileJumpMove(Player *ply);		// 2f5e2

static void guile_attack_bison(Player *ply);


static void sub_2f1b0(Player *ply);
static short GuileButtons(Player *ply);

static void _GuileExitStand(Player *ply);		/* 2f08c */
static void _GuileExitCrouch(Player *ply);		/* 2f096 */
static void _GuileExitJump(Player *ply);		/* 2f0a0 */





typedef struct UserData_Guile UD;


#pragma mark Animation Callbacks

void pl_cb_setstatus2_guile(Player *ply, short status, int argd0) {
	//printf("setstatus stat 0x%x arg %d\n",status,argd0);
	setaction_list((Object *)ply, data_4abac[status / 2], argd0);
}
void pl_cb_setstatus3_guile(Player *ply, short status) {
	pl_cb_setstatus2_guile(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_guile(Player *ply, short status) {
	pl_cb_setstatus2_guile(ply, status, 0);
}



#pragma mark Human move checkers 

short PLCBStandGuile(Player *ply) {	/* 2efdc standmove callback */
	if (GuileButtons(ply)) {
		return GuileStandMove(ply);
	}
	return 0;
}
short PLCBCrouchGuile(Player *ply) {	/* 2efe6 crouch callback */
	if (GuileButtons(ply)) {
		return GuileCrouchMove(ply);
	}
	return 0;
}
short PLCBJumpGuile(Player *ply) {	/* 2eff0 jump callback */
	if (GuileButtons(ply)) {
		return GuileJumpMove(ply);
	}
	return 0;
}
void PLCBPowerGuile(Player *ply) {	//2effa
	_GuileSMSonic(ply);
	_GuileSMBlade(ply);
	
}

#pragma mark Attack Callback

// Guile Standing Punches
static void sub_2f850(Player *ply) {		/* 2f850 Guile stand punch */
	switch (ply->Move) {
		case 0:
		case 1:
			switch (ply->mode2) {		// 2f86e		Little punches
				case 0:
					NEXT(ply->mode2);
					CASetAnim2(ply, STATUS_PUNCH, ply->Move);
					break;
				case 2:
					if (AF1) {
						_GuileExitStand(ply);
					} else if (AF2 == 0) {
						PLAYERTICK;
					} else if (PLY_NEWBUTTONS & BUTTON_A) {
						if(PSSetNextAction(ply)) {
							plstat_do_nextaction(ply);
						} else {
							g.HumanMoveCnt++;
							ply->Opponent->SufferHB5 = 0;
							if (ply->OppXDist >= 20) {
								ply->Move = 1;
							} else {
								ply->Move = 0;
							}
							CASetAnim2(ply, STATUS_PUNCH, ply->Move);
							quirkysound(0);
						}
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			switch (ply->mode2) {		// 2f8f4 All other punches
				case 0:
					NEXT(ply->mode2);
					CASetAnim2(ply, STATUS_PUNCH, ply->Move);
					break;
				case 2:
					if (AF1) {
						_GuileExitStand(ply);
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 6:			// 2f922 Twist-throw
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->Flip = ply->JoyDecode.full & 1 ? 0 : 1;
					CASetAnim2(ply, STATUS_PUNCH, ply->Move);
					break;
				case 2:
					if (AF2) {
						NEXT(ply->mode2);	// release the victim from the grip
						set_throw_trajectory(ply, 0, ply->Flip, 0xd);
					} else {
						PLAYERTICK;
					}
					break;
				case 4:
					if (AF1) {
						ply->Flip ^= 1;
						ply->EnemyDirection = ply->Flip;
						_GuileExitStand(ply);
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 7:		// 2f996 Backbreaker
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->Flip = ply->JoyDecode.full & 1 ? 0 : 1;
					CASetAnim2(ply, STATUS_PUNCH, ply->Move);
					break;
				case 2:
					if (AF2) {
						ply->Timer2 = 12;
						ply->Opponent->Timer2 = 12;
						NEXT(ply->mode2);
						sub_36d6(ply, 
								 ply->Flip ? 128 : -128,
								 0, 2, ply->Flip, 13, 46, 2);			
						ActStartScreenWobble();		// Screen wobble
					} else {
						PLAYERTICK;
					}
					break;
				case 4:
					if (AF1) {
						ply->EnemyDirection = ply->Flip;
						_GuileExitStand(ply);
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
// Guile Standing Kicks
static void sub_2fa2c(Player *ply) {
	switch (ply->Move) {
		case 0:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					CASetAnim2(ply, STATUS_KICK, ply->Move);
					break;
				case 2:
					if (AF1) {
						_GuileExitStand(ply);
					} else if (AF2 
					           && (PLY_NEWBUTTONS & BUTTON_D) ) {
						if (PSSetNextAction(ply)) {
							plstat_do_nextaction(ply);
						} else {
							// 2fa92
							g.HumanMoveCnt++;
							ply->Opponent->SufferHB5 = 0;
							CASetAnim2(ply, STATUS_KICK, 0);
							quirkysound(0);
						}
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			// All other standing kicks  2faba
			STDANIM(0x42, _GuileExitStand);
			break;
		FATALDEFAULT;
	}
	
}
//Guile Crouching Punches 
static void sub_2faf8(Player *ply) {
	switch (ply->Move) {
		case 0:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
					break;
				case 2:
					if (AF1) {
						_GuileExitCrouch(ply);
					} else if (AF2 && (PLY_NEWBUTTONS & BUTTON_A)) {
						if(PSSetNextAction(ply)) {
							plstat_do_nextaction(ply);
						} else {
							g.HumanMoveCnt++;
							ply->Opponent->SufferHB5 = 0;
							CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
							quirkysound(0);
						}
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 1:
		case 2:
			// 2fb7e
			STDANIM(0x44, _GuileExitCrouch);
			break;
		FATALDEFAULT;
	}
}
//Guile Crouching Kicks 
static void sub_2fbac(Player *ply) {
	switch (ply->Move) {
		case 0:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					CASetAnim2(ply, STATUS_CROUCH_KICK, ply->Move);
					break;
				case 2:
					if (AF1) {
						_GuileExitCrouch(ply);
					} else if (AF2 && (PLY_NEWBUTTONS & BUTTON_D)) {
						if(PSSetNextAction(ply)) {
							plstat_do_nextaction(ply);
						} else {
							g.HumanMoveCnt++;
							ply->Opponent->SufferHB5 = 0;
							CASetAnim2(ply, STATUS_CROUCH_KICK, ply->Move);
							quirkysound(0);
						}
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 1:
		case 2:
			// 2fb7e
			switch (ply->mode2) {		
				case 0:
					NEXT(ply->mode2);
					CASetAnim2(ply, STATUS_CROUCH_KICK, ply->Move);
					break;
				case 2:
					if (AF1) {
						_GuileExitCrouch(ply);
					} else if (AF2 < 1) {
						PLAYERTICK;
					} else if (AF2 == 1) {
						quirkysound(1);
						if (AF1) {
							_GuileExitCrouch(ply);
						} else {
							PLAYERTICK;
						}
					} else {
						quirkysound(2);
						if (AF1) {
							_GuileExitCrouch(ply);
						} else {
							PLAYERTICK;
						}
					}
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}

static void sub_2fc80(Player *ply) {
	switch (ply->Move) {
		case 0:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					if ((ply->JoyCorrect & 0xb) || (ply->JoyCorrect & 2) == 0) {
						ply->Flip ^= 1;
					}
					ply->Timer2 = 12;
					ply->EnemyDirection = ply->Flip;
					CASetAnim2(ply, 0x52, ply->Move);
					CATrajectory((Object *)ply);
					if (ply->VelY.full < 0 && PLAYERGROUND) {
						ply->mode2 = 6;
						CASetAnim1(ply, 0x54);
					}
					break;
				case 2:
					PLAYERTICK;
					if (AF1) {
						NEXT(ply->mode2);
						set_throw_trajectory(ply, 8, ply->Flip, 15);
						CATrajectory((Object *)ply);
						if (ply->VelY.full < 0 && PLAYERGROUND) {
							ply->mode2 = 6;
							CASetAnim1(ply, 0x54);
						}	
					}
					break;
				case 4:
					if (ply->VelY.full < 0 && PLAYERGROUND) {
						_GuileExitJump(ply);
					} else {
						CATrajectory((Object *)ply);
					}
					break;
				case 6:
					PLAYERTICK;
					if (AF1) {
						set_throw_trajectory(ply, 0, ply->Flip, 0x20);	//XXX high damage!
						_GuileExitJump(ply);
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 1:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					if ((ply->JoyCorrect & 0xb) || (ply->JoyCorrect & 2) == 0) {
						ply->Flip ^= 1;
					}
					ply->Timer2 = 12;
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
					if (PLAYERGROUND) {
						NEXT(ply->mode2);
						CASetAnim1(ply, 0x56);
					}
					break;
				case 4:
					NEXT(ply->mode2);
					ply->Timer2 = 12;
					ply->Opponent->Timer2 = 12;
					
					sub_36d6(ply, (ply->Flip ? 16 : -16), 0x3c, 10, ply->Flip, 16, 0x2e, 2);
					ActStartScreenWobble();
					break;
				case 6:
					PLAYERTICK;
					if (AF1) {
						_GuileExitStand(ply);
					}
					break;
					FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}

// Publics
// Attack callback
void PSCBAttackGuile(Player *ply) {		/* PLSTAT_ATTACKING callback */
	UD *ud=(UD *)&ply->UserData;	

	if (ply->Timer2) {
		ply->Timer2--;
	} else {
		ud->x008d++;
		
		switch (ply->StandSquat) {
			case PLY_STAND:	
				STDPUNCHKICK(sub_2f850, sub_2fa2c);
				break;
			case PLY_CROUCH:
				STDPUNCHKICK(sub_2faf8, sub_2fbac)
				break;
			case 4:
				sub_2fc80(ply);
				break;
			FATALDEFAULT;
		}
	}
}
// Powermove Callback
void PSCBPowerGuile(Player *ply) {		/* 2fe26 PLSTAT_POWERMOVE callback */
	static short data_2ff3e[] = {0x800, 0x900, 0xa00};

	UD *ud=(UD *)&ply->UserData;	
	Object *obj;

	if (ply->Timer2) {
		ply->Timer2--;
	} else {
		switch (ud->x008b) {
			case 0:
				switch (ply->mode2) {
					case 0:
						if (ply->mode3 == 0) {
							NEXT(ply->mode3);
							CASetAnim2(ply, 0x4c, ud->SonicStrength);
							soundsting(SOUND_SONIC_BOOM);
						}
						if (((ply->AnimFlags & 0xff00) >> 8) == 1) {
							if(obj=AllocProjectile()){
								obj->exists = TRUE;
								obj->Sel = SF2_PROJ_SONICBOOM;
								obj->XPI = ply->XPI;
								obj->YPI = ply->YPI;
								obj->Flip = ply->Flip;
								obj->SubSel = ply->ButtonStrength;
								obj->Owner = ply;
								ply->Projectile = obj;
							}
							NEXT(ply->mode2);
						}
						PLAYERTICK;
						break;
					case 2:
						if (AF1) {
							NEXT(ply->mode2);
							ply->LocalTimer = 15;
						}
						PLAYERTICK;
						break;
					case 4:
						if (--ply->LocalTimer >= 0) {
							PLAYERTICK;
						} else {
							_GuileExitStand(ply);
						}
						break;
					FATALDEFAULT;
				}
				break;
			case 2:
				/* blade  2fede */
				
				switch (ply->mode2) {
					case 0:
						NEXT(ply->mode2);
						ply->VelX.full = 0x180;		// redundant table
						ply->AclX.full = 0;
						if (ply->Flip) {
							ply->VelX.full = -ply->VelX.full;
						}
						ply->VelY.full = data_2ff3e[ply->UserData[0xc]];
						ply->AclY.full = 0x60;	// redundant table
						ply->Airborne = AIR_JUMPING;
						CASetAnim2(ply, 0x4e, ply->UserData[0xc]);
						quirkysound(2);
						PLAYERTICK;
						break;
					case 2:
						if ((ply->AnimFlags & 0xff00) == 0x100) {
							PLAYERTICK;
						} else {
							CATrajectory((Object *)ply);
							if (ply->VelY.full >= 0 || PLAYERGROUND == 0) {
								PLAYERTICK;
							} else {
								ply->Airborne = AIR_ONGROUND;
								_GuileExitJump(ply);
							}
						}
						break;
					FATALDEFAULT;
				}
				break;
			FATALDEFAULT;
		 }
	}
}
// Victory Callback
void sub_2ff6e(Player *ply) {
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


#pragma mark Private Functions
static short GuileNewButtonsDown(Player *ply, short mask ) {		/* 2f396 */
	return (PLY_NEWBUTTONS & mask);
}
static short GuileNewButtonsUp(Player *ply, short mask ) {		/* 2f396 */
	return (~ply->JoyDecode.full & ply->JoyDecodeDash.full & mask);
}
static void _GuileExitStand(Player *ply) {		/* 2f08c */
	UD *ud=(UD *)&ply->UserData;	
	ud->x008d = 0;
	ply_exit_stand(ply);
}
static void _GuileExitCrouch(Player *ply) {		/* 2f096 */
	UD *ud=(UD *)&ply->UserData;	
	ud->x008d = 0;
	ply_exit_crouch(ply);
}
static void _GuileExitJump(Player *ply) {		/* 2f0a0 */
	UD *ud=(UD *)&ply->UserData;	
	ud->x008d = 0;
	ply_exit_air(ply);
}


#pragma mark Computer Guile

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
					ply->Move = ply->OppXDist >= 0x28 ? 3 : 2;
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
						ply->Move = ply->OppXDist >= 0x28 ? 4 : 3;
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
			if (ply->AnimFlags & 0xff00 == 0x100) {	
				if (obj = AllocProjectile()) {
					INITOBJC(obj, 0x3, ply->ButtonStrength, PLAYERX, PLAYERY);
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
			if (ply->AnimFlags & 0xff00 == 0x100) { /* oh shiii... */
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
}
	

static void sub_2f1d0(Player *ply) {
	UD *ud=(UD *)&ply->UserData;	
	if (--ud->x0083 == 0) {
		ud->x0084 -= 2;
		if (ud->x0084 == 0) {
			ud->SonicStep = 0;
		} else {
			ud->x0083 = 2;
		}
	}
}
static void sub_2f19a(Player *ply, short d0) {
	UD *ud=(UD *)&ply->UserData;	

	decode_buttons(ply, d0);
	if (ply->PunchKick) {
		ud->SonicStep = 0;
		return;
	}
	if ((ply->ButtonStrength + 2) > ud->x0084 ) {
		sub_2f1d0(ply);
	} else {
		sub_2f1b0(ply);
	}	
}
static void _GuileSMSonic(Player *ply) {		//2f0aa
	UD *ud=(UD *)&ply->UserData;	
	short d0;
	
	u8 data_2f0c0[]={0x3c, 0x1e, 0x0c, 0x1};
	u8 data_2f12c[]={ 8, 9, 8,10, 8,11, 8, 9, 8,10, 8,11, 8,12, 8,13,
					  9,14, 8,15, 8, 9, 8,10, 8,11, 8,12, 8, 9, 8,10,
	};

	switch (ud->SonicStep) {
		case 0:
			NEXT(ud->SonicStep);
			ud->SonicTimer = data_2f0c0[g.x0320];
			ud->SonicTimer2 = 0;
			ud->x0084 = 0;
			break;
		case 2:
			if (ply->JoyCorrect2 & 1) {
				if(--ud->SonicTimer == 0) {
					NEXT(ud->SonicStep);
				}
			} else {
				ud->SonicTimer = data_2f0c0[g.x0320];
			}
			break;
		case 4:
			if ((ply->JoyCorrect2 & 1) == 0) {
				NEXT(ud->SonicStep);
				ud->SonicTimer2 = data_2f12c[RAND32];
			}
			break;
		case 6:
			if (--ud->SonicTimer2 == 0) {
				ud->SonicStep = 0;
			} else if (ply->JoyCorrect2 == 2) {
				if(GuileNewButtonsDown(ply, 0x70)) {
					sub_2f1b0(ply);
				} else if (GuileNewButtonsUp(ply, 0x70)) {
					sub_2f1b0(ply);
				} else {
					ud->x0083 = 8;
					ud->x0084 = 6;
					NEXT(ud->SonicStep);
				}
			}
			break;
		case 8:
			d0 = GuileNewButtonsDown(ply, 0x70); 
			if (d0) {
				sub_2f19a(ply, d0);
			} else {
				d0 = GuileNewButtonsUp(ply, 0x70);
				if (d0) {
					sub_2f19a(ply, d0);
				} else {
					sub_2f1d0(ply);
				}
			}
			break;
		FATALDEFAULT;
	}
}

static void sub_2f326(Player *ply) {
	UD *ud=(UD *)&ply->UserData;	


	
	
	if (check_special_ability(ply) ||
	    AF2 == 0x24 ||
		AF2 == 0x42 || 
		ud->x008d > 5
		) {
		
		ud->BladeStep = 0;
	} else {
		_GuileStartBlade(ply);
	}
}

static void _GuileSMBlade(Player *ply) {		// 2f216
	UD *ud=(UD *)&ply->UserData;	
	static char data_2f29a[] = {
		8,9,8,10,9,11,8,9,8,10,8,11,8,12,8,13,8,14,8,15,8,9,8,10,8,11,8,12,8,9,8,10
	};
	int d0;
	static char data_2f32e[] = {0, 36, 0, 21};
	
	switch (ud->BladeStep) {
		case 0:
			NEXT(ud->BladeStep);
			ud->BladeTimer = data_2f32e[g.x0320];
			ud->BladeTimer2 = 0;
			ud->x0089 = 0;
			ud->BladeMaxStr = 0;
			break;
		case 2:
			if (ply->JoyCorrect2 & 0x4) {
				if (--ud->BladeTimer == 0) {
					NEXT(ud->BladeStep);
				}
			} else {
				ud->BladeTimer = data_2f32e[g.x0320];
			}
			break;
		case 4:
			if ((ply->JoyCorrect2 & 0x4) == 0) {
				NEXT(ud->BladeStep);
				ud->BladeTimer2 = data_2f29a[RAND32];
			}
			break;
		case 6:
			if (--ud->BladeTimer2 == 0) {
				ud->BladeStep = 0;
			} else {
				if (ply->JoyCorrect2 & 0x8) {
					if(d0 = GuileNewButtonsDown(ply, 0x700)) {
						decode_buttons(ply, d0);
						sub_2f326(ply);
					} else if (d0 = GuileNewButtonsUp(ply, 0x700)) {
						decode_buttons(ply, d0);
						sub_2f326(ply);
					} else {
						ud->x0089 = 8;
						ud->BladeMaxStr = 6;
						NEXT(ud->BladeStep);
					}
					
				}
			}
			break;
		case 8:
			if(d0 = GuileNewButtonsDown(ply, 0x700)) {
				decode_buttons(ply, d0);
			} else if (d0 = GuileNewButtonsUp(ply, 0x700)) {
				decode_buttons(ply, d0);
			} else {
				if(--ud->x0089 == 0) {
					ud->BladeMaxStr -= 2;
					if (ud->BladeMaxStr == 0) {
						ud->BladeStep = 0;
					} else {
						ud->x0089 = 2;
					}
				}
				return;
			}
			if (ply->PunchKick == PLY_PUNCHING				||
				(ply->ButtonStrength + 2) > ud->BladeMaxStr) {
				
				ud->BladeStep = 0;
			} else {
				sub_2f326(ply);
			}
			break;
			
			break;
			FATALDEFAULT;
	}
}


static short sub_2f042(Player *ply) {	/* check if Guile can do Sonic boom */
	UD *ud=(UD *)&ply->UserData;

	if(check_special_ability(ply) || ply->Projectile != 0 || AF2 == 0x42 || ud->x008d > 5) {
		return 0;
	}
	return 1;
}
static short sub_2f066(Player *ply) {	/* check if Guile can do Sonic boom */
	UD *ud=(UD *)&ply->UserData;

	if(check_special_ability(ply) || AF2==0x42 || AF2 == 0x24 || ud->x008d > 5) {
		return 0;
	}
	return 1;
}
static void sub_2f1b0(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	
	if(check_special_ability(ply) || AF2==0x42 || AF2 == 0x24 || ud->x008d > 5) {
		sub_2f1d0(ply);
	} else {
		_GuileStartSonic(ply);
	}
}

static short _GuileStartSonic(Player *ply) {		/* 2f1ea */
	UD *ud=(UD *)&ply->UserData;

	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = ply->mode3 = 0;
	ud->SonicStep = 0;
	ud->x008b = 0;
	ud->SonicStrength = ply->ButtonStrength / 2;
	BumpDiff_PowerMove();
	g.HumanMoveCnt++;
	return -1;
}


static short _GuileStartBlade(Player *ply) {		/* 2f362 */
	UD *ud=(UD *)&ply->UserData;

	
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = ply->mode3 =0;
	ud->BladeStep = 0;
	ply->Attacking = TRUE;
	ud->x008b = 0;
	ud->x008c = ply->ButtonStrength / 2;
	BumpDiff_PowerMove();
	g.HumanMoveCnt++;
	return -1;
}
	
static short GuileButtons(Player *ply) {
	short d0;

	//g_d6 = ply->JoyDecode.full;
	d0 = ((~ply->JoyDecodeDash.full) & ply->JoyDecode.full) & BUTTON_MASK ;
	if (d0 == 0) {
		return 0;
	}
	decode_buttons(ply, d0);
	if (LBRareChance()) {
		switch (ply->PunchKick) {
			case PLY_PUNCHING:
				if (sub_2f042(ply)) {
					return _GuileStartSonic(ply);		/* free sonic boom */
				}
				break;
			case PLY_KICKING:
				if (sub_2f066(ply)) {
					return _GuileStartBlade(ply);		/* free bladekick */
				}
				break;
			FATALDEFAULT;
		}
		
	} 
	return 1;
}

static short _GuileMidPunch(Player *ply) {   /* 2f42c */
	/* 2f42c */
	ply->Move = 3;
	if (ply->OppXDist >= 0x28) {
		ply->Move = 3;
	} else {
		ply->Move = 2;
	}
	quirkysound(1);
	return 1;	
}
static short _GuileBigPunch(Player *ply) {   /* 2f47a */
	ply->Move = 3;
	if (ply->OppXDist >= 0x3c) {
		ply->Move = 5;
	} else {
		ply->Move = 4;
	}
	quirkysound(2);
	return 1;	
}

// Check if air-throw is valid
static short sub_2f6fa(Player *ply) {
	if (ply->VelY.full < 0) {
		if (ply->YPI < 40 || ply->YPI >= 88) {
			return 0;
		} else {
			return 1;
		}
	} else {
		if (ply->YPI < 40 || ply->YPI >= 71) {
			return 0;
		} else {
			return 1;
		}
	}
}

static short GuileStandMove(Player *ply) {		/* 2f3b2*/
	g.HumanMoveCnt++;
	ply->StandSquat = 0;
	
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			/* 2fc33 */
			switch (ply->ButtonStrength) {
				case 0:
					if (ply->OppXDist >= 0x14) {
						ply->Move = 1;
					} else {
						ply->Move = 0;
					}
					quirkysound(0);
					return 1;
					break;
				case 2:
					if ((ply->JoyDecode.full & 0x000b) == 0) {
						return _GuileMidPunch(ply);
					} else {
						/* 2f404 */
						PLY_THROW_SET(0xffe8, 0x0035, 0x0018, 0x0010);
						if (throwvalid(ply)) {
							/* Twist-throw, or Invisible Throw bug */
							ply->Move = 6;
							return 1;
						} else {
							return _GuileMidPunch(ply);
						}
					}
					break;
				case 4:
					if ((ply->JoyDecode.full & 0x000b) == 0) {
						return _GuileBigPunch(ply);
					} else {
						/* 2f452 */
						PLY_THROW_SET(0xffe8, 0x0035, 0x0018, 0x0010);
						if (throwvalid(ply)) {
							/* Backbreaker throw */
							ply->Move = 7;
							return 1;
						} else {
							return _GuileBigPunch(ply);
						}
					}
					break;
				FATALDEFAULT;
			}
			break;
		case PLY_KICKING:
			/* 2f49a */
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = 0;
					quirkysound(0);
					return 1;
					break;
				case 2:
					if (ply->JoyDecode.full == 0 || ply->OppXDist >= 0x33) {
						/* 2f4de */
						if (ply->OppXDist >= 0x26) {
							ply->Move = 2;
						} else {
							ply->Move = 1;
						}
						quirkysound(1);
						return 1;
					} else {
						ply->Move = 5;
						quirkysound(1);
						return 1;
					}
					break;
				case 4:
					if (ply->JoyDecode.full == 0 || ply->OppXDist >= 0x47) {
						/* 2f4de */
						if (ply->OppXDist >= 0x28) {
							ply->Move = 4;
						} else {
							ply->Move = 3;
						}
						quirkysound(2);
						return 1;
					} else {
						ply->Move = 6;
						quirkysound(2);
						return 1;
					}
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
	return 0;
}


static short GuileCrouchMove(Player *ply) {		/* 2f53e */
	g.HumanMoveCnt++;
	ply->StandSquat = 2;
	
	QUICKMOVE(ply->ButtonStrength / 2); return TRUE; 
}

static short GuileJumpMove(Player *ply) {		// 2f5e2
	UD *ud=(UD *)&ply->UserData;

	g.HumanMoveCnt++;
	ply->StandSquat = 4;
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			ud->AirThrow = FALSE;
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = ply->VelX.full == 0 ? 3 : 0;
					quirkysound(0);
					break;
				case 2:
					if (sub_2f6fa(ply) && (ply->JoyDecode.full & 0x7)) {
						PLY_THROW_SET(-30, 56, 37, 36);
						if (airthrowvalid(ply)) {
							ply->Move = 0;
							ud->AirThrow = TRUE;
						} else {
							ply->Move = ply->VelX.full == 0 ? 4 : 1;							quirkysound(1);
						}
					} else {
						ply->Move = ply->VelX.full == 0 ? 4 : 1;
						quirkysound(1);
					}
					break;
				case 4:
					// 2f6a8
					if (sub_2f6fa(ply) && (ply->JoyDecode.full & 7)) {
						PLY_THROW_SET(-30, 56, 37, 36);
						if (airthrowvalid(ply)) {
							ply->Move = 0;
							ud->AirThrow = TRUE;
						} else {
							ply->Move = ply->VelX.full == 0 ? 2 : 5;
							quirkysound(1);
						}
					} else {
						ply->Move = ply->VelX.full == 0 ? 2 : 5;
						quirkysound(1);
					}
					break;
				FATALDEFAULT;
			}
			if (ud->AirThrow != FALSE) {
				ply->Attacking = TRUE;
				ply->mode2 = ply->mode3 = 0;
				return -1;
			} else {
				CASetAnim2(ply, STATUS_JUMP_PUNCH, ply->Move);
				return 1;
			}
			break;
		case PLY_KICKING:
			ud->AirThrow2 = FALSE;
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = ply->VelX.full == 0 ? 0 : 2;
					quirkysound(0);
					break;
				case 2:
					if (sub_2f6fa(ply) && (ply->JoyDecode.full & 7)) {
						PLY_THROW_SET(-30, 56, 37, 36);
						if (airthrowvalid(ply)) {
							ply->Move = 1;
							ud->AirThrow2 = TRUE;
						} else {
							ply->Move = ply->VelX.full == 0 ? 1 : 3;	
							quirkysound(1);
						}
					} else {
						ply->Move = ply->VelX.full == 0 ? 1 : 3;	
						quirkysound(1);
					}
					break;
				case 4:
					if (sub_2f6fa(ply) && (ply->JoyDecode.full & 7)) {
						PLY_THROW_SET(-30, 56, 37, 36);
						if (airthrowvalid(ply)) {
							ply->Move = 1;
							ud->AirThrow2 = TRUE;
						} else {
							ply->Move = ply->VelX.full == 0 ? 4 : 5;	
							quirkysound(2);
						}
					} else {
						ply->Move = ply->VelX.full == 0 ? 4 : 5;	
						quirkysound(2);
					}
					break;
				FATALDEFAULT;
			}
			if (ud->AirThrow2 != FALSE) {
				ply->mode1 = PLSTAT_ATTACKING;
				ply->mode2 = ply->mode3 = 0;
				return -1;
			} else {
				CASetAnim2(ply, STATUS_JUMP_KICK, ply->Move);
				return 1;
			}
			break;
		FATALDEFAULT;
	}
	return 0;
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
