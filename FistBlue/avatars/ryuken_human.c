/*
 *  ryuken_human.c
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

typedef struct KenRyuInputs KRBtns;
typedef struct UserData_RyuKen UD;

/**
 * @brief Power move state machine
 * @note sf2ua/0x2d714
 */
void PSCBPowerRyu(Player *ply) {
	struct UserData_RyuKen *ud = (struct UserData_RyuKen *)&ply->UserData;
	
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		switch (ud->current_power_move) {
			case RYUKEN_POWER_MOVE_HADOUKEN:
				switch (ply->mode2) {
					case 0:
						/* Tamper protection removed */
						RyuKenStartHadouken(ply);
						break;
					case 2:
						RyuSMHadouken(ply);
						break;
					case 4:
						if (ryuken_power_move_recovery_timer(ply) == FALSE) {
							ply_exit_stand(ply);
						}
						break;
                    FATALDEFAULT;
				}
				break;
			case RYUKEN_POWER_MOVE_HURRICANE:                         //2d902
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
			case RYUKEN_POWER_MOVE_SHORYUKEN:                        //2d7de
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
	static const short data_2cfaa[]={25, 31, 41};
	static const short data_2cfb0[]={ 0,  0, 40};

	ply->Move = ply->ButtonStrength;
	if (ply->PunchKick == PLY_PUNCHING) {
		if (ply->OppXDist > data_2cfaa[ply->ButtonStrength / 2]) {
			++ply->Move;		/* do different punch if opponent is close */
		}
	}
	else {
		if (ply->OppXDist > data_2cfb0[ply->ButtonStrength / 2]) {
			ply->Move++;		/* Do scissor kick instead of roundhouse */
		}
	}
	ply->StandSquat = PLY_STAND;
}

static short _RyuKenCheckThrow(Player *ply, short d6) {	//2cfb6
    if (((d6 & JOY_LEFT) || (d6 & JOY_RIGHT)) && ply->ButtonStrength != 0) {
        PLY_THROW_SET(-0x18, 0x35, 0x18, 0x10);
        if (throwvalid(ply)) {
            if ((ply->JoyDecode.full & JOY_LEFT) == 0) {
                ply->Flip = FACING_LEFT;
            } else {
                ply->Flip = FACING_RIGHT;
            }
            return TRUE;
        }
    }
	return FALSE;
}


static void _RyuKenStandMove(Player *ply, short d6) {     // XXX misnamed
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
	
	if (AF2) {
		return buttonspressed(ply, data_2d50e[ply->ButtonStrength + (ply->PunchKick * 3)]);
	} else {
		return 0;
	}
}

static void _RyuAttack0(Player *ply) {		// 2d1b4
	switch (ply->mode2) {
		case 0:
			quirkysound(ply->ButtonStrength / 2);
            setstatus4(ply, ply->PunchKick == PLY_PUNCHING ? STATUS_PUNCH : STATUS_KICK);
            break;
		case 2:
			if (AF1) {
				ply_exit_stand(ply);
				return;
			}
			if (doublebtn(ply) != 0) {
				if (PSSetNextAction(ply)) {
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
            setstatus4(ply, ply->PunchKick ? STATUS_CROUCH_KICK : STATUS_CROUCH_PUNCH);
			break;
		case 2:
			if (AF1) {
				ply_exit_crouch(ply);
				return;
			}
			if (doublebtn(ply) != 0) {
				if (PSSetNextAction(ply)) {
					plstat_do_nextaction(ply);
				} else {
					++g.HumanMoveCnt;
					sub_2d080(ply);
					ply->mode2 = 0;
				}
			}
			break;
			FATALDEFAULT;
	}
	PLAYERTICK;
}
/*!
 sf2ua: 2d2e8
 */
static void _RyuKenFinishThrow(Player *ply) {
    if (AF1) {
        ply->Flip = ply->Flip ^ 1;
        ply->EnemyDirection = ply->Flip;
        ply_exit_stand(ply);
    } else {
        PLAYERTICK;
    }
}
/*!
 * sf2ua: 2d2ba
 */
static void _RyuKenTopThrow(Player *ply) {
    switch (ply->mode3) {
        case 0:
            if (AF2) {
                set_throw_trajectory(ply, 0, ply->Flip ^ 1, 13);
                NEXT(ply->mode3)
            }
            PLAYERTICK;
            break;
        case 2:
            _RyuKenFinishThrow(ply);
            break;
        FATALDEFAULT;
    }
}

static void _RyuAttack6(Player *ply) {		//2d28e RyuKenThrow
	UD *ud=(UD *)&ply->UserData;
	
	if (ply->FighterID != FID_RYU) {
		// ken  sub_2d308
		switch (ply->mode2) {
			case 0:
                NEXT(ply->mode2)
                ud->ShoryukenX.full     = FLT2FP16(-4.5 * FLIP(ply));
				ud->ShoryukenY.full     = FLT2FP16(2);
				ud->ShoryukenYDash.full = FLT2FP16(-0.125);

                soundsting(SOUND_HUA);
				CASetAnim2(ply, 0x54, ply->Move);
				break;
			case 2:
                if (ply->PunchKick == PLY_PUNCHING) {
                    _RyuKenTopThrow(ply);
                } else {
                    switch (ply->mode3) {
                        case 0:
                            if (AF2 == 0) {
                                NEXT(ply->mode3);
                            }
                            PLAYERTICK;
                            break;
                        case 2:
                            if (KenTrajectory(ply) < 0) { PLAYERGROUND; }
                            if (AF2) {
                                /* inlined 2d2d0 */
                                set_throw_trajectory(ply, 0, ply->Flip ^ 1, 13);
                                NEXT(ply->mode3);
                            }
                            PLAYERTICK;
                            break;
                        case 4:
                            if (KenTrajectory(ply) < 0) {
                                if (PLAYERGROUND) {
                                    NEXT(ply->mode3);
                                    ActStartScreenWobble(); 
                                }
                            }
                            break;
                        case 6:
                            _RyuKenFinishThrow(ply);
                            break;
                        FATALDEFAULT;
                    }
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
                _RyuKenTopThrow(ply);
				break;
            FATALDEFAULT;
		}
	}
}

short RyuPowerSuccess(Player *ply, struct ryuken_power_move_state *a2) { // 2d6ee
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = 0;
	ud->x00c2 = 5;
	a2->x02 = 0;
	ply->Attacking = TRUE;
	BumpDiff_PowerMove();
	++g.HumanMoveCnt;
	return 0;
}

static void _RyuPowerStrCheck(Player *ply, struct ryuken_power_move_state *user, u16 d0) {		//2d632
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	
	if (check_special_ability(ply) == 0 && ud->x00c2) {
		decode_buttons(ply, d0);		
        if (ply->ButtonStrength >= user->input_timer) { /* more powerful specials are slightly harder to input */
            user->sequence_index = 0;
            user->x02 = 0;
		} else {
			user->x02 = 1;
		}
	}
}

static void RyuCheckSequence(Player *ply, const u16 *input_sequence, struct ryuken_power_move_state *user) {	// 2d58e
    static const char otherdata[32]={
        10,   8,   8,  11,   8,   8,   9,   8,
         9,   8,   8,  10,   8,   8,  11,  13,
         8,  15,   9,   8,  10,  12,   8,   8,
        11,   8,   9,   8,  14,  12,   8,  10,
    };
    
	u16 buttons;
	switch (user->sequence_index) {
		case 0:
			if ((ply->JoyCorrect2 & JOY_MOVEMASK) == input_sequence[0]) {
				NEXT(user->sequence_index);
				user->input_timer = otherdata[RAND32];
			} else {
				user->sequence_index = 0;
                user->x02 = 0;
			}
			break;
		case 2:
			if (--user->input_timer == 0) {
				user->sequence_index = 0;
                user->x02 = 0;
			} else if ((ply->JoyCorrect2 & JOY_MOVEMASK) == input_sequence[1]) {
				NEXT(user->sequence_index);
				user->input_timer = otherdata[RAND32];
			}
			break;
		case 4:
			if (--user->input_timer == 0) {
				user->sequence_index = 0;
                user->x02 = 0;
			} else if ((ply->JoyCorrect2 & JOY_MOVEMASK) == input_sequence[2]) {
				if ((buttons = buttonspressed(ply, input_sequence[3]))) {		/* arg in %D1 @ 32c8 */
					_RyuPowerStrCheck(ply, user, buttons);
				} else if ((buttons=buttonsreleased(ply, input_sequence[3]))) {
					_RyuPowerStrCheck(ply, user, buttons);
				} else {
					NEXT(user->sequence_index);
				}
			}
			break;
		case 6:
			if (--user->input_timer == 0) {
				user->sequence_index = 0;
                user->x02 = 0;
			} else {
				if ((buttons = buttonspressed(ply, input_sequence[3]))) {		/* arg in %D1 @ 32c8 */
					_RyuPowerStrCheck(ply, user,buttons);
				} else if ((buttons=buttonsreleased(ply, input_sequence[3]))) {
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
		--ply->Timer2;
	} else {
		if (ud->x00c2) {
			--ud->x00c2;
		}
		switch (ply->StandSquat) {
			case PLY_STAND:	_RyuAttack0(ply);	break;
			case PLY_CROUCH: _RyuAttack2(ply);	break;
			case PLY_JUMP:
				FBPanic(0);
				/* Shouldn't get here */
				break;
			case PLY_THROW:	_RyuAttack6(ply);	break;
				FATALDEFAULT;
		}
	}
}

KRBtns sub_2d0d8(Player *ply) {
	KRBtns retval;
	retval.stick = ply->JoyDecode.full;
	retval.buttons = PLY_NEWBUTTONS & BUTTON_MASK;
	return retval;
}

static void sub_2d142(Player *ply, struct ryuken_power_move_state *a2) {
	UD *ud=(UD *)&ply->UserData;
	
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = 0;
	ud->x00c2 = 5;
	a2->x02 = 0;
	ply->Attacking = TRUE;
	BumpDiff_PowerMove();		
	g.HumanMoveCnt++;
}

short PLCBStandRyu(Player *ply) {		//2cf44
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	
	KRBtns buttons;		
	
	ud->x00c2 = 5;
    buttons = sub_2d0d8(ply);                   /* KenRyu GetButtons */
	if (buttons.buttons == 0){
        return 0;
    }
    
	ply->StandSquat = PLY_STAND;
	++g.HumanMoveCnt;
	decode_buttons(ply, buttons.buttons);

	if (LBRareChance()) {                       /* 1 in 512 lucky free power move @2d0ec */
		if (ply->PunchKick == PLY_PUNCHING) {
            if (0xbbbbbbbb & (1 << RAND32)) {   /* 75% Hadouken, 25% Shoryuken*/
				/* 2d114 */
				if (ply->Projectile) {
					if (ply->StandSquat) {
						return sub_2d080(ply);
					}
					else {
						_RyuKenStandMove(ply, buttons.stick);
						return 1;
					}
				} else {
					ud->current_power_move = RYUKEN_POWER_MOVE_HADOUKEN;
					sub_2d142(ply, &ud->FireballTriggerState);
                    return -1;
				}
			}
			else {
				ud->current_power_move = RYUKEN_POWER_MOVE_SHORYUKEN;
				sub_2d142(ply, &ud->ShoryukenTriggerState);
                return -1;
			}
        } else {
            ud->current_power_move = RYUKEN_POWER_MOVE_HURRICANE;
            sub_2d142(ply, &ud->HurricaneTriggerState);
            return -1;
        }
	} else {
		_RyuKenStandMove(ply, buttons.stick);
		return 1;
	}
}

short PLCBCrouchRyu(Player *ply) {		//2d05a
	struct UserData_RyuKen *ud=(struct UserData_RyuKen *)&ply->UserData;
	KRBtns buttons;
	
	ud->x00c2 = 5;
	buttons = sub_2d0d8(ply);
	if (buttons.buttons) {
		ply->StandSquat = PLY_CROUCH;
		g.HumanMoveCnt++;
		decode_buttons(ply, buttons.buttons);
		if (LBRareChance()) {
			// 2d0ec
			if (ply->PunchKick != PLY_PUNCHING) {
				ud->current_power_move = RYUKEN_POWER_MOVE_HURRICANE;
				sub_2d142(ply, &ud->HurricaneTriggerState);
                return -1;
            } else if (0xbbbbbbbb & (1<<RAND32)) { /* 75% Hadouken, 25% Shoryuken */
                if (ply->Projectile) {          /* can't fire a second Hadouken, so */
					if (ply->StandSquat) {      /* do and ordinary move instead     */
						return sub_2d080(ply);
					} else {
						_RyuKenStandMove(ply, buttons.stick);
						return TRUE;
					}
				} else {
					ud->current_power_move = RYUKEN_POWER_MOVE_HADOUKEN;
					sub_2d142(ply, &ud->FireballTriggerState);
                    return -1;
				}
			} else {
				ud->current_power_move = RYUKEN_POWER_MOVE_SHORYUKEN;
				sub_2d142(ply, &ud->ShoryukenTriggerState);
                return -1;
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
	if (buttons.buttons) {
		++g.HumanMoveCnt;
		decode_buttons(ply, buttons.buttons);
		quirkysound(ply->ButtonStrength >> 1);
		ud->x00c2 = 0;
		if (ply->VelX.full) {
			temp = ply->ButtonStrength + 1;
		} else {
			temp = ply->ButtonStrength;
		}
		ply->Move = temp;
        if (ply->PunchKick == PLY_PUNCHING) {
            CASetAnim2(ply, STATUS_JUMP_PUNCH, temp);
		} else {
            CASetAnim2(ply, STATUS_JUMP_KICK, temp);
		}
		return TRUE;
	}
	return FALSE;
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

	if (ply->PSFinishedParticipating) {
		return;
	}
	/* check for hurricane */
	RyuCheckSequence(ply, RyuMoveHurricane, &ud->HurricaneTriggerState);
	if (ud->HurricaneTriggerState.x02) {
		ud->current_power_move = RYUKEN_POWER_MOVE_HURRICANE;
		RyuPowerSuccess(ply, &ud->ShoryukenTriggerState);
		return;
	}
    RyuCheckSequence(ply, RyuMoveShoyuken, &ud->ShoryukenTriggerState);
	if (ud->ShoryukenTriggerState.x02) {
		ud->current_power_move = RYUKEN_POWER_MOVE_SHORYUKEN;
		RyuPowerSuccess(ply, &ud->ShoryukenTriggerState);
		return;
	}
	RyuCheckSequence(ply, RyuMoveFireball, &ud->FireballTriggerState);
	if (ud->FireballTriggerState.x02 && ply->Projectile == 0) {
		ud->current_power_move = RYUKEN_POWER_MOVE_HADOUKEN;
		RyuPowerSuccess(ply, &ud->FireballTriggerState);
		return;
	}
	ud->FireballTriggerState.x02 = 0;
}

