/*
 *  ryuken_human.c
 *  MT2
 *
 *  Created by Ben on 22/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
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

extern Game g;

typedef struct KenRyuButtons KRBtns;
typedef struct UserData_RyuKen UD;

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
						RyuKenStartHadouken(ply);
						break;
					case 2:
						RyuSMHadouken(ply);
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
						RyuStartHurricane(ply);
						break;
					case 2:
						RyuSMHurricane(ply);
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
						RyuStartShoryuken(ply);
						break;
					case 2:
						RyuSMShoryuken(ply);
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
	static const short data_2cfaa[]={0x19, 0x1f, 0x29};
	static const short data_2cfb0[]={ 0,  0, 40};

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
static short _RyuKenCheckThrow(Player *ply, short d6) {	//2cfb6
	d6 &= 0x3;
	if (d6==0 || ply->ButtonStrength==0) {
		return 0;
	}
	PLY_THROW_SET(-0x18, 0x35, 0x18, 0x10);
	if(throwvalid(ply)) {
		if ((ply->JoyDecode.full & JOY_UP)==0) {
			ply->Flip = FACING_LEFT;
		} else {
			ply->Flip = FACING_RIGHT;
		}
		return TRUE;
	}
	return FALSE;
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

short doublebtn(Player *ply) {		/* 2d4f0 */
	short data_2d50e[]={
		BUTTON_A,	BUTTON_B,	BUTTON_C,
		BUTTON_D,	BUTTON_E,	BUTTON_F,
	};
	
	if (ply->AnimFlags & 0xff) {
		return buttonspressed(ply, data_2d50e[ply->ButtonStrength + (ply->PunchKick * 3)]);
	} else {
		return 0;
	}
}




static void _RyuAttack0(Player *ply) {		// 2d1b4
	switch (ply->mode2) {
		case 0:
			quirkysound(ply->ButtonStrength / 2);
			if (ply->PunchKick != 0) {
				setstatus4(ply, STATUS_KICK);
			} else {
				setstatus4(ply, STATUS_PUNCH);
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
	PLAYERTICK;
}
static short sub_2d080(Player *ply) {
	ply->Move = ply->ButtonStrength >> 1;
	return 1;
}

static void _RyuAttack2(Player *ply) {		//2d214
	switch (ply->mode2) {
		case 0:
			quirkysound(ply->ButtonStrength / 2);
			if (ply->PunchKick) {
				setstatus4(ply, STATUS_CROUCH_KICK);
			} else {
				setstatus4(ply, STATUS_CROUCH_PUNCH);
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
	PLAYERTICK;
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
				ud->ShoryukenY.full     = 0x00020000;
				ud->ShoryukenYDash.full = 0xffffe000;
				soundsting(SOUND_HUA);
				CASetAnim2(ply, 0x54, ply->Move);
				break;
			case 2:
				switch (ply->mode3) {
					case 0:
						if (AF2) {
							NEXT(ply->mode3);
						}
						PLAYERTICK;
						break;
					case 2:
						if(RyuAtApex(ply) < 0) { PLAYERGROUND; }
						if (AF2) {
							/* inlined 2d2d0 */
							set_throw_trajectory(ply, 0, ply->Flip ^ 1, 13);
							NEXT(ply->mode3);
						}
						PLAYERTICK;
						break;
					case 4:
						if(RyuAtApex(ply) < 0) {
							if(PLAYERGROUND) {
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
							PLAYERTICK;
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
				soundsting(SOUND_HUA);
				CASetAnim2(ply, 0x54, ply->Move);
				break;
			case 2:
				switch (ply->mode3) {
					case 0:
						if (AF2) {
							set_throw_trajectory(ply, 0, ply->Flip ^ 1, 13);
							NEXT(ply->mode3)
						}
						PLAYERTICK;
						break;
					case 2:
						if (AF1) {
							ply->Flip = ply->Flip ^ 1;
							ply->EnemyDirection = ply->Flip;
							ply_exit_stand(ply);
						} else {
							PLAYERTICK;
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
static void RyuCheckSequence(Player *ply, const u16 *movedata, const char *otherdata, char *user) {	// 2d58e
	u16 buttons;
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
				if ((buttons=buttonspressed(ply, movedata[3]))) {		/* arg in %D1 @ 32c8 */
					_RyuPowerStrCheck(ply, user,buttons);
				} else if ((buttons=buttonsreleased(ply, movedata[3]))) {
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
				if ((buttons=buttonspressed(ply, movedata[3]))) {		/* arg in %D1 @ 32c8 */
					_RyuPowerStrCheck(ply, user,buttons);
				} else if ((buttons=buttonsreleased(ply, movedata[3]))) {
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
KRBtns sub_2d0d8(Player *ply) {
	KRBtns retval;
	retval.d6 = ply->JoyDecode.full;
	retval.d0 = PLY_NEWBUTTONS & BUTTON_MASK;
	return retval;
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
#define D 0x4
#define B 0x1
#define F 0x2
#define K 0x700
#define P 0x070
	
	static const u16 RyuMoveFireball[4]  = {  D  , D|F,  F,  P };		//2d674 fireball
	static const u16 RyuMoveHurricane[4] = {  D  , D|B,  B,  K };		//2d67c hurricane
	static const u16 RyuMoveShoyuken[4]  = {  F  , D,  D|F,  P };		//2d684 shoryken
	
#undef B
#undef K
#undef D
#undef F
#undef P
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	static const char data_2d68c[32]={
		10,   8,   8,  11,   8,   8,   9,   8,
		9,   8,   8,  10,   8,   8,  11,  13, 
		8,  15,   9,   8,  10,  12,   8,   8,  
		11,   8,   9,   8,  14,  12,   8,  10, 
	};
	
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

