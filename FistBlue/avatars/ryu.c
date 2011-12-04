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


static void _RyuStartHurricane(Player *ply);
static void _RyuSMHurricane(Player *ply);

// opponent distances determine which kick/punch happens
short data_32cd0[]={0x19, 0x1f, 0x29};	// Little, mid, big punch
short data_32cec[]={ 0x0,  0x0, 0x28};	// Big Kick


static int _RyuAtApex(Player *ply);


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


#pragma mark Victory Calback
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
				if (obj=AllocActor()) {
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


KRBtns sub_2d0d8(Player *ply) {
	KRBtns retval;
	retval.d6 = ply->JoyDecode.full;
	retval.d0 = ~ply->JoyDecodeDash.full & ply->JoyDecode.full & BUTTON_MASK;
	return retval;
}

short doublebtn(Player *ply) {		/* 2d4f0 */
	short data_2d50e[]={
		0x10,  0x20,  0x40,
		0x100, 0x200, 0x400,
	};
	
	if (ply->AnimFlags & 0xff) {
		return buttonspressed(ply, data_2d50e[ply->ButtonStrength + (ply->PunchKick * 3)]);
	} else {
		return 0;
	}
}

static short sub_2d142(Player *ply, char *a2) {
	UD *ud=(UD *)&ply->UserData;
	
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = 0;
	ud->x00c2 = 5;
	a2[2]=0;
	ply->Attacking = 1;
	BumpDiff_PowerMove();		
	g.HumanMoveCnt++;
	return -1;
}

static short _RyuKenCheckThrow(Player *ply, short d6) {	//2cfb6
	d6 &= 0x3;
	if (d6==0 || ply->ButtonStrength==0) {
		return 0;
	}
	ply->Throw[0] = -0x18;
	ply->Throw[1] =  0x35;
	ply->Throw[2] =  0x18;
	ply->Throw[3] =  0x10;
	if(throwvalid(ply)) {
		ply->Flip = FACING_RIGHT;
		if (ply->JoyDecode.full & JOY_UP==0) {
			ply->Flip = FACING_LEFT;
		}
		return TRUE;
	}
	return FALSE;
}

static short sub_2d080(Player *ply) {
	ply->Move = ply->ButtonStrength >> 1;
	return 1;
}


#pragma mark Hadouken


// start hadouken action
static void _RyuKenStartHadouken(Player *ply) {			//2d766
	/* Ryu/Ken Hadouken state zero */
	NEXT(ply->mode2);
	CASetAnim2(ply, STATUS_HADOUKEN, ply->ButtonStrength >> 1);
}
// animate until seqence ends, then init projectile
static void _RyuSMHadouken(Player *ply) {		//2d778
	Object *obj;
	
	if (AF1) {
		NEXT(ply->mode2);
		if (obj=AllocProjectile()) {
			obj->exists=TRUE;
			obj->Sel = SF2_PROJ_HADOUKEN;
			obj->XPI = ply->XPI;
			obj->YPI = ply->YPI;
			obj->Flip = ply->Flip;
			obj->SubSel = ply->ButtonStrength;
			obj->Owner = ply;
			ply->Projectile = obj;
			
			queuesound(SOUND_HADOUKEN);		/* Hadouken! */
		}
		ply->LocalTimer = 0x28;
	} 
	actiontick((Object *)ply);
	
}
// hold timer at end
static short sub_2d7d2(Player *ply) {
	if (--ply->LocalTimer) {
		actiontick((Object *)ply);
		return 1;
	}
	return 0;
}
// End Hadouken shared


#pragma mark Shoryuken

/* 2d3a2 apex finder */

static int _RyuAtApex(Player *ply) {		//2d3a2
	UD *ud=(UD *)&ply->UserData;
	
	ply->X.full += ud->ShoryukenX.full;
	ply->Y.full += ud->ShoryukenY.full;
	ud->ShoryukenY.full += ud->ShoryukenYDash.full;
	
	return ud->ShoryukenYDash.full;
}

/* 2d7f0 Shoryuken Init */
static void _RyuStartShoryuken(Player *ply) {		//2d7f0
	UD *ud=(UD *)&ply->UserData;
	
	short data_2d82c[]={4, 6, 8};
	int	data_2d83a[]={0x50000, 0x70000, 0x90000};
	
	NEXT(ply->mode2);
	queuesound(0x6a);		/* Shoryuken! */
	ud->ShoryukenX.part.integer = data_2d82c[ply->ButtonStrength/2];
	ud->ShoryukenX.part.fraction = 0;
	ud->ShoryukenXDash.full = -0x8000;
	ud->ShoryukenY.full = data_2d83a[ply->ButtonStrength/2];
	ud->ShoryukenYDash.full = -0x6000;
	
	CASetAnim2(ply, STATUS_SHORYUKEN, ply->ButtonStrength/2);
}
/* 2d84a Shoryuken State Machine */
static void _RyuSMShoryuken(Player *ply) {		//2d84a
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	
	switch (ply->mode3) {
		case 0:
			if ((ply->AnimFlags & 0xff) == 0) {
				NEXT(ply->mode3);
				ply->Airborne = AIR_JUMPING;
			}
			actiontick((Object *)ply);
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
			actiontick((Object *)ply);
			break;
		case 4:
			ply->Y.full += ud->ShoryukenY.full;
			ud->ShoryukenY.full += ud->ShoryukenYDash.full;
			if (ud->ShoryukenY.full < 0) {
				if (check_ground_collision(ply)) {
					NEXT(ply->mode3);
					ply->Airborne = AIR_ONGROUND;
					soundsting(SOUND_IMPACT8);
					actiontick((Object *)ply);
					return;
				}
			}
			if ((AF2) == 0) {
				actiontick((Object *)ply);
			}
			break;
		case 6:
			if (AF1) {
				NEXT(ply->mode2);
				ply->mode3 = 0;
			} else {
				actiontick((Object *)ply);
			}
			break;
			FATALDEFAULT;
	}
}




#pragma mark ---- Move check callbacks ----

void PSCBPowerRyu(Player *ply) {		//2d714
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	
	if (ply->Timer2) {
		ply->Timer2--;
	} else {
		switch (ud->x00c0) {
			case 0:					//2d732
				switch (ply->mode2) {
					case 0:
						/* Tamper protection removed */
						_RyuKenStartHadouken(ply);
						break;
					case 2:
						_RyuSMHadouken(ply);
						break;
					case 4:
						//sub_2d7c8(ply);
						if (sub_2d7d2(ply)==0) {
							ply_exit_stand(ply);
						}
						break;
						FATALDEFAULT;
				}
				break;
			case 2:					//2d902
				switch (ply->mode2) {
					case 0:		
						_RyuStartHurricane(ply);
						break;
					case 2:
						_RyuSMHurricane(ply);
						break;
					case 4:
						ply_exit_stand(ply);
						break;
						FATALDEFAULT;
				}
				break;
			case 4:					//2d7de
				switch (ply->mode2) {
					case 0:
						_RyuStartShoryuken(ply);
						break;
					case 2:
						_RyuSMShoryuken(ply);
						break;
					case 4:
						ply_exit_stand(ply);	// comp doesn't do this step
						break;
						FATALDEFAULT;
				}
				
				break;
			FATALDEFAULT;
		}
	}
}

static void _RyuStandNoThrow(Player *ply) {
	ply->Move = ply->ButtonStrength;
	if (ply->PunchKick==PLY_PUNCHING) {
		if (ply->OppXDist > data_2cfaa[ply->ButtonStrength]) {
			ply->Move++;		/* do different punch if opponent is close */
		}
	}
	else {
		if (ply->OppXDist > data_2cfb0[ply->ButtonStrength]) {
			ply->Move++;		/* Do scissor kick instead of roundhouse */
		}
	}
	ply->StandSquat = PLY_STAND;
}

static void _RyuStandThrow(Player *ply, short d6) {
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	
	
	if (_RyuKenCheckThrow(ply, d6)) {
		/* 2d012 */
		ply->StandSquat = PLY_THROW;		/* Throwing */
		ud->x00c2 = 0;
		ply->Move = ply->PunchKick / 2;
	} else {
		_RyuStandNoThrow(ply);	
	}
}	


static void _RyuAttack0(Player *ply) {		// 2d1b4
	switch (ply->mode2) {
		case 0:
			quirkysound(ply->ButtonStrength / 2);
			if (ply->PunchKick != 0) {
				setstatus4(ply, 0x42);
			} else {
				setstatus4(ply, 0x40);
			}
			break;
		case 2:
			if (AF1) {
				ply_exit_stand(ply);
				return;
			}
			if(doublebtn(ply)!=0) {	
				if(PSSetNextAction(ply)) {
					plstat_do_nextaction(ply);
				} else {
					g.HumanMoveCnt++;
					_RyuStandNoThrow(ply);	
					ply->mode2 = 0;
				}
			}
			break;
		FATALDEFAULT;
	}
	actiontick((Object *)ply);
}
static void _RyuAttack2(Player *ply) {		//2d214
	switch (ply->mode2) {
		case 0:
			quirkysound(ply->ButtonStrength / 2);
			if (ply->PunchKick != 0) {
				setstatus4(ply, 0x46);
			} else {
				setstatus4(ply, 0x44);
			}
			break;
		case 2:
			if (ply->AnimFlags & 0x8000) {
				ply_exit_crouch(ply);
				return;
			}
			if(doublebtn(ply)!=0) {
				if(PSSetNextAction(ply)) {
					plstat_do_nextaction(ply);
				} else {
					g.HumanMoveCnt++;
					sub_2d080(ply);
					ply->mode2 = 0;
				}
			}
			break;
		FATALDEFAULT;
	}
	actiontick((Object *)ply);

}
static void _RyuAttack6(Player *ply) {		//2d28e
	UD *ud=(UD *)&ply->UserData;

	if (ply->FighterID) {
		// ken  sub_2d308
		switch (ply->mode2) {
			case 0:
				ply->mode2 += 2;
				ud->ShoryukenX.full = 0x48000;
				if (ply->Flip != FACING_LEFT) {
					ud->ShoryukenX.full = -ud->ShoryukenX.full;
				}
				ud->ShoryukenY.full = 0x20000;
				ud->ShoryukenYDash.full = 0xffffe000;
				soundsting(0x47);
				CASetAnim2(ply, 0x54, ply->Move);
				break;
			case 2:
				switch (ply->mode3) {
					case 0:
						if (AF2) {
							NEXT(ply->mode3);
						}
						actiontick((Object *)ply);
						break;
					case 2:
						if(_RyuAtApex(ply) < 0) { check_ground_collision(ply); }
						if (AF2) {
							/* inlined 2d2d0 */
							set_throw_trajectory(ply, 0, ply->Flip ^ 1, 0xd);
							NEXT(ply->mode3);
						}
						actiontick((Object *)ply);
						break;
					case 4:
						if(_RyuAtApex(ply) < 0) {
							if(check_ground_collision(ply)) {
								NEXT(ply->mode3);
								ActStartScreenWobble(); 
							}
						}
						break;
					case 6:
						/* reinlined 2d2e8 */
						if (AF1) {
							ply->Flip = ply->Flip ^ 1;
							ply->EnemyDirection = ply->Flip;
							ply_exit_stand(ply);
						} else {
							actiontick((Object *)ply);
						}
						break;
					FATALDEFAULT;
				}
				break;
			FATALDEFAULT;
		}
		
	} else {
		// RYU
		switch (ply->mode2) {
			case 0:
				ply->mode2 += 2;
				soundsting(0x47);
				CASetAnim2(ply, 0x54, ply->Move);
				break;
			case 2:
				switch (ply->mode3) {
					case 0:
						if (AF2) {
							set_throw_trajectory(ply, 0, ply->Flip ^ 1, 0xd);
							NEXT(ply->mode3)
						}
						actiontick((Object *)ply);
						break;
					case 2:
						if (AF1) {
							ply->Flip = ply->Flip ^ 1;
							ply->EnemyDirection = ply->Flip;
							ply_exit_stand(ply);
						} else {
							actiontick((Object *)ply);
						}
						break;
				}
				break;
			FATALDEFAULT;
		}
		
	}

}


short RyuPowerSuccess(Player *ply, char *a2) { // 2d6ee
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;

	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = 0;
	ud->x00c2 = 5;
	a2[2] = 0;
	ply->Attacking = TRUE;
	BumpDiff_PowerMove();
	g.HumanMoveCnt++;
	return 0;
}

static void _RyuPowerStrCheck(Player *ply, char *user, u16 d0){		//2d632
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;

	if(check_special_ability(ply)==0 && ud->x00c2) {
		decode_buttons(ply, d0);		
		if (ply->ButtonStrength >= user[1]) {
			user[0] = user[2] = 0;
		} else {
			user[2] = 1;
		}
	}

}
static void RyuCheckSequence(Player *ply, u16 *movedata, char *otherdata, char *user) {	// 2d58e
	u16 buttons;
	//printf("RCS %d %d %d\n", user[0], user[1], user[2]);
	switch (user[0]) {
		case 0:
			if ((ply->JoyCorrect2 & 0xf) == movedata[0]) {
				NEXT(user[0]);
				user[1]=otherdata[RAND32];
			} else {
				user[0]=0; user[2]=0;
			}
			break;
		case 2:
			if (--user[1] == 0) {
				user[0]=0; user[2]=0;
			} else if ((ply->JoyCorrect2 & 0xf) == movedata[1]) {
				NEXT(user[0]);
				user[1]=otherdata[RAND32];
			}
			break;
		case 4:
			if (--user[1] == 0) {
				user[0]=0; user[2]=0;
			} else if ((ply->JoyCorrect2 & 0xf) == movedata[2]) {
				if (buttons=buttonspressed(ply, movedata[3])) {		/* arg in %D1 @ 32c8 */
					_RyuPowerStrCheck(ply, user,buttons);
				} else if (buttons=buttonsreleased(ply, movedata[3])) {
					_RyuPowerStrCheck(ply, user,buttons);
				} else {
					NEXT(user[0]);
				}
			}
			break;
		case 6:
			if (--user[1] == 0) {
				user[0]=0; user[2]=0;
			} else {
				if (buttons=buttonspressed(ply, movedata[3])) {		/* arg in %D1 @ 32c8 */
					_RyuPowerStrCheck(ply, user,buttons);
				} else if (buttons=buttonsreleased(ply, movedata[3])) {
					_RyuPowerStrCheck(ply, user,buttons);
				}
			}
			break;
		FATALDEFAULT;
	}
}


void PSCBAttackRyu(Player *ply) {		//2d168
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	
	// suicide code removed 
	if (ply->Timer2) {
		ply->Timer2--;
	} else {
		if (ud->x00c2) {
			ud->x00c2--;
		}
		switch (ply->StandSquat) {
			case 0:	_RyuAttack0(ply);	break;
			case 2: _RyuAttack2(ply);	break;
			case 4:
				panic(0);
				/* Shouldn't get here */
				break;
			case 6:	_RyuAttack6(ply);	break;
				FATALDEFAULT;
		}
	}
}
short PLCBStandRyu(Player *ply) {		//2cf44
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	
	KRBtns buttons;		
	
	ud->x00c2 = 5;
	buttons = sub_2d0d8(ply);
	if(buttons.d0 == 0){ return 0; }						// KenRyu GetButtons
	ply->StandSquat = 0;
	g.HumanMoveCnt += 1;
	decode_buttons(ply, buttons.d0);
	if (LBRareChance()) {
		/* 2d0ec */
		if (ply->PunchKick != 0) {
			ud->x00c0=2;
			return sub_2d142(ply, ud->x0098);
		} else {
			if (0xbbbbbbbb & (1 << RAND32)) {
				/* 2d114 */
				if (ply->Projectile) {
					if (ply->StandSquat) {
						return sub_2d080(ply);
					}
					else {
						_RyuStandThrow(ply, buttons.d6);
						return TRUE;
					}
				} else {
					ud->x00c0= 0;
					return sub_2d142(ply, ud->x0090);
				}
			}
			else {
				ud->x00c0=4;
				return sub_2d142(ply, ud->x00a0);
			}
		}
	} else {
		_RyuStandThrow(ply, buttons.d6);
		return TRUE;
	}		
	
}
short PLCBCrouchRyu(Player *ply) {		//2d05a
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	KRBtns buttons;
	
	ud->x00c2 = 5;
	buttons=sub_2d0d8(ply);
	if (buttons.d0) {
		ply->StandSquat = 2;
		g.HumanMoveCnt++;
		decode_buttons(ply, buttons.d0);
		if (LBRareChance()) {
			// 2d0ec
			if (ply->PunchKick != 0) {
				ud->x00c0 = 2;
				return sub_2d142(ply, ud->x0098);
			} else if (0xbbbbbbbb & (1<<RAND32)) {
				if (ply->Projectile) {
					if (ply->StandSquat) {
						return sub_2d080(ply);
					} else {
						_RyuStandThrow(ply, buttons.d6);
						return TRUE;
					}
				} else {
					ud->x00c0 = 0;
					return sub_2d142(ply, ud->x0090);
				}
			} else {
				ud->x00c0 = 4;
				return sub_2d142(ply, ud->x00a0);
			}
		} else {
			return sub_2d080(ply);
		}
		
	}
	return 0;
}
short PLCBJumpRyu(Player *ply) {		//2d08e
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	KRBtns buttons;
	short temp;
	
	ud->x00c2 = 5;
	
	buttons = sub_2d0d8(ply);
	if (buttons.d0) {
		g.HumanMoveCnt++;
		decode_buttons(ply, buttons.d0);
		quirkysound(ply->ButtonStrength >> 1);
		ud->x00c2 = 0;
		if (ply->VelX.full) {
			temp = ply->ButtonStrength+1;
		} else {
			temp = ply->ButtonStrength;
		}
		ply->Move = temp;
		if (ply->PunchKick != 0) {
			CASetAnim2(ply, STATUS_JUMP_KICK, temp);
		} else {
			CASetAnim2(ply, STATUS_JUMP_PUNCH, temp);
		}
		return 1;
	}
	return 0;
}
void PLCBPowerRyu(Player *ply){		// 2d51a check Ryu / Ken powermoves
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;

	if (ply->PSFinishedParticipating) {
		return ;
	}
	/* check for hurricane */
	RyuCheckSequence(ply, RyuMoveHurricane, data_2d68c, ud->x0098);	//hurricane
	if (ud->x0098[2]) {
		ud->x00c0 = 2;
		 RyuPowerSuccess(ply, ud->x00a0);
		return;
	}
	RyuCheckSequence(ply, RyuMoveShoyuken, data_2d68c, ud->x00a0);	//shoryuken
	if (ud->x00a0[2]) {
		ud->x00c0 = 4;
		 RyuPowerSuccess(ply, ud->x00a0);
		return;
	}
	RyuCheckSequence(ply, RyuMoveFireball, data_2d68c, ud->x0090);	//fireball
	if (ud->x0090[2] && ply->Projectile==0) {
		ud->x00c0 = 0;
		 RyuPowerSuccess(ply, ud->x0090);
		return;
	}
	ud->x0090[2] = 0;
}


#pragma mark Hurricane
// start moving
static void _RyuStartHurricane(Player *ply) {		//2d914
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
static void _RyuSMHurricane(Player *ply) {		//2d96e
	UD *ud=(UD *)&ply->UserData;
	
	switch (ply->mode3) {
		case 0:
			if(_RyuAtApex(ply)<0){
				NEXT(ply->mode3);
				CASetAnim2(ply, STATUS_HURRICANE, (ply->ButtonStrength/2)+2);
			}
			actiontick((Object *)ply);
			break;
		case 2:
			if (ply->AnimFlags & 0x8000) {
				soundsting(SOUND_SWOOSH3);
				if(--ud->x00c1 < 0){
					NEXT(ply->mode3);
					CASetAnim2(ply, STATUS_HURRICANE, 1);
				}
			}			
			ply->X.full += ud->ShoryukenX.full;
			actiontick((Object *)ply);
			break;
		case 4:
			_RyuAtApex(ply);
			if (check_ground_collision(ply)) {
				NEXT(ply->mode2);
				ply->mode3=0;
				ply->Airborne = AIR_ONGROUND;
				ply->ThrowDisable = data_2d9fe[RAND16];
				if (ply->Human) {
					ply_exit_air(ply);
				}
			} else {
				actiontick((Object *)ply);
			}
			break;

		FATALDEFAULT;
	}
}

#pragma mark ---- COMPUTER ----

static void sub_32efa(Player *ply);

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
		actiontick((Object *)ply);
	}
}

static void sub_32dda(Player *ply){		/* 32dda throw animation except ken's roller */
	switch (ply->mode3) {
		case 0:
			if (AF2) {
				set_throw_trajectory(ply, 0, ply->Flip ^ 1, 13);
				NEXT(ply->mode3);
			}
			actiontick((Object *)ply);
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
	actiontick((Object *)ply);
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
						queuesound(SOUND_HUA);	/* Hua! */
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
									if (ply->AnimFlags & 0xff) {
										NEXT(ply->mode3);
									} 
									actiontick((Object *)ply);
									break;
								case 2:
									if(_RyuAtApex(ply) < 0) {
										check_ground_collision(ply);
									}
									if (ply->AnimFlags & 0xff) {
										ryu_throwtrajectory(ply);
									} else {
										actiontick((Object *)ply);
									}
									break;
								case 4:
									if(_RyuAtApex(ply)<0) {
										if(check_ground_collision(ply)){
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
						case 0:	_RyuKenStartHadouken(ply); break;
						case 2: _RyuSMHadouken(ply); break;
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
						case 0: _RyuStartHurricane(ply); break;
						case 2: _RyuSMHurricane(ply); break;
						case 4: RyuExitStandAttack(ply); break;
						FATALDEFAULT;
					}
					break;
				case 4:
					switch (ply->mode2) {
						case 0: _RyuStartShoryuken(ply); break;
						case 2: _RyuSMShoryuken(ply); break;
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

short sub_32d7e (Player *ply) {	/* callback from 2c9be */
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

static void sub_32efa(Player *ply) {	
	/* simplified from original */
	if(ply->CompDoThrow == 0) { return; }
	ply->Throw[0] = -0x18;
	ply->Throw[1] =  0x35;
	ply->Throw[2] =  0x18;
	ply->Throw[3] =  0x10;
	if(throwvalid(ply)) {		/* 0x3338 */
		ply->StandSquat = 0x6;
		ply->Move = ply->PunchKick << 1;	
		ply->Step = ply->IsWithinBounds ^ 1;
		ply->Flip = ply->Step;
	}
}