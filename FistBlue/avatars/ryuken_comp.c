/*
 *  ryuken_comp.c
 *  MT2
 *
 *  Created by Ben on 22/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */

#include "sf2.h"
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

extern Game g;

typedef struct KenRyuButtons KRBtns;
typedef struct UserDataComp_RyuKen UDCOMP;



static void sub_32efa(Player *ply) {	
	/* simplified from original */
	if(ply->CompDoThrow == 0) { return; }
	PLY_THROW_SET(-0x18, 0x35, 0x18, 0x10);
	if(throwvalid(ply)) {		/* 0x3338 */
		ply->StandSquat = 0x6;
		ply->Move = ply->PunchKick << 1;	
		ply->Step = ply->IsWithinBounds ^ 1;
		ply->Flip = ply->Step;
	}
}
static void RyuExitStandAttack(Player *ply) {		/* 32d08 */
	ply->AISigAttack = 0;
	ply->AIVolley = FALSE;
	exit_comp_normal(ply);
}	

static void RyuAnimateThrow(Player *ply) {	//32e08
	if (AF1) {
		ply->Flip ^= 1;
		ply->EnemyDirection = ply->Flip;
		RyuExitStandAttack(ply);	/* exit */
	} else {
		PLAYERTICK;
	}
}

static void sub_32dda(Player *ply){		/* 32dda throw animation except ken's roller */
	switch (ply->mode3) {
		case 0:
			if (AF2) {
				set_throw_trajectory(ply, 0, ply->Flip ^ 1, 13);
				NEXT(ply->mode3);
			}
			PLAYERTICK;
			break;
		case 2:
			RyuAnimateThrow(ply);
			break;
			FATALDEFAULT;
	}
}	

static short sub_32f56(Player *ply) {
	if (ply->AISigSpecial == 0) {
		return 0;
	}
	if (ply->PunchKick == PLY_PUNCHING && ply->Projectile != NULL) {
		return 0;
	}
	ply->StandSquat = 8;		
	return 1;
}

static void ryu_throwtrajectory(Player *ply) {		// 32df0
	set_throw_trajectory(ply, 0, ply->Flip ^ 1, 13);
	NEXT(ply->mode3);
	PLAYERTICK;
}

static short ryu_checkvolley(Player *ply) {		// 32f74
	if (AF2) {
		if (comp_diceroll(ply)) {		
			ply->mode2 = 0;
			return comp_setnextaction(ply);
		}
	}
	return 0;
}

/* comp_attack_plycallback Ryu/Ken */
void PLCBCompAttackRyuKen(Player *ply) {
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	static const short data_32cd0[]={0x19, 0x1f, 0x29};	// Little, mid, big punch
	static const short data_32cec[]={ 0x0,  0x0, 0x28};	// Big Kick

	
	if (ply->Timer2) {
		ply->Timer2--;
		return;
	}
	if (ply->mode2==0) {
		if(sub_32f56(ply) == 0) { 
			sub_32efa(ply);
		}
	}
	switch (ply->StandSquat) {
		case PLY_STAND:
			/* 32c8a Ryu Standing */
			switch (ply->mode2) {
				case 0:
					/* 32c9e */
					quirkysound(ply->ButtonStrength / 2);
					ply->Move = ply->ButtonStrength;
					if (ply->PunchKick==0) {
						if (ply->OppXDist > data_32cd0[ply->Move]) {	
							ply->Move++;	
						}
						setstatus4(ply, STATUS_PUNCH);
					} else {
						if (ply->OppXDist > data_32cec[ply->Move]) {
							ply->Move++;
						}
						setstatus4(ply, STATUS_KICK);
						/* 32cd6 */
					}
					break;
				case 2:
					/* 32cf2 */
					if (AF1) {
						RyuExitStandAttack(ply);
					} else {
						/* 32cfa */
						if(ryu_checkvolley(ply)) {
							comp_do_next_action(ply);
						}
					}
					break;
					FATALDEFAULT;
			}
			PLAYERTICK;
			break;
		case PLY_CROUCH:
			/* 32d24 Ryu Crouching */
			switch (ply->mode2) {
				case 0:
					/* 32d38 */
					ply->Move = ply->ButtonStrength >> 1;
					quirkysound(ply->Move);
					if (ply->PunchKick) {
						setstatus4(ply, STATUS_CROUCH_KICK);
					} else {
						setstatus4(ply, STATUS_CROUCH_PUNCH);
					}
					break;
				case 2:	/* 32d5a */
					if (AF1) {
						ply->AISigAttack = 0;
						ply->AIVolley = FALSE;
						exit_to_compdisp1(ply);
					} else if (ryu_checkvolley(ply)) {
						comp_do_next_action(ply);
					} 
					break;
					FATALDEFAULT;
			}
			PLAYERTICK;
			break;
		case 4:
			/* Does nothing */
			break;
		case PLY_THROW:
			/* 32dae throwing */
			if (ply->FighterID == FID_RYU) {
				switch (ply->mode2) {
					case 0:
						NEXT(ply->mode2);
						queuesound(SOUND_HUA);
						CASetAnim2(ply, STATUS_RYUKEN_THROW, ply->Move);
						break;
					case 2:
						sub_32dda(ply);
						break;
						FATALDEFAULT;
				}
			} 
			else {
				/* 32e58 Ken */
				switch (ply->mode2) {
					case 0:
						NEXT(ply->mode2);
						ud->ShoryukenX.full = 0x00048000L;			
						if (ply->Flip != FACING_LEFT) {
							ud->ShoryukenX.full = -ud->ShoryukenX.full;
						}
						ud->ShoryukenY.full = 0x00020000L;
						ud->ShoryukenYDash.full =-0x2000L;
						
						queuesound(SOUND_HUA);	/* Hua! */
						CASetAnim2(ply, STATUS_RYUKEN_THROW, ply->Move);
						break;
					case 2:		// 32e9e
						if (ply->PunchKick == PLY_PUNCHING) {
							sub_32dda(ply);	// same as Ryu
						} else {
							switch (ply->mode3) {
								case 0:
									if (AF2) {
										NEXT(ply->mode3);
									} 
									PLAYERTICK;
									break;
								case 2:
									if(RyuAtApex(ply) < 0) {
										PLAYERGROUND;
									}
									if (AF2) {
										ryu_throwtrajectory(ply);
									} else {
										PLAYERTICK;
									}
									break;
								case 4:
									if(RyuAtApex(ply)<0) {
										if(PLAYERGROUND){
											NEXT(ply->mode3);
											ActStartScreenWobble();
										}
									}
									break;
								case 6:
									RyuAnimateThrow(ply);
									break;
								FATALDEFAULT;
							}
						}
						break;
						FATALDEFAULT;
						
				}
			}
			break;
		case 8:
			/* 32f92 */
			switch (ply->PunchKick) {
				case PLY_PUNCHING:
					switch (ply->mode2) {
						case 0:	RyuKenStartHadouken(ply); break;
						case 2: RyuSMHadouken(ply); break;
						case 4: 
							if(sub_2d7d2(ply)==0) {
								RyuExitStandAttack(ply);
							}
							break;
							FATALDEFAULT;
					}
					break;
				case PLY_KICKING:
					switch (ply->mode2) {
						case 0: RyuStartHurricane(ply); break;
						case 2: RyuSMHurricane(ply); break;
						case 4: RyuExitStandAttack(ply); break;
							FATALDEFAULT;
					}
					break;
				case 4:
					switch (ply->mode2) {
						case 0: RyuStartShoryuken(ply); break;
						case 2: RyuSMShoryuken(ply); break;
						case 4:	RyuExitStandAttack(ply);	break;
							FATALDEFAULT;
					}
					break;
					FATALDEFAULT;
			}
			break;
			FATALDEFAULT;
	}
	
}

int PLCBCompJumpRyuKen (Player *ply) {	/* callback from 2c9be */
	quirkysound(ply->ButtonStrength >> 1);
	if(ply->VelX.full == 0) {
		ply->Move = ply->ButtonStrength;
	} 
	else {
		ply->Move = ply->ButtonStrength+1;
	}
	if(ply->PunchKick) {
		CASetAnim2(ply, STATUS_JUMP_KICK, ply->Move);
	} 
	else {
		CASetAnim2(ply, STATUS_JUMP_PUNCH, ply->Move);
	}
	return 1;
}
