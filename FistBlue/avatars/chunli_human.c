/*
 *  chunli_human.c
 *  MT2
 *
 *  Created by Ben on 25/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */


#include "sf2.h"
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

typedef struct UserData_ChunLi UD;
extern Game g;


inline static void chunli_exit(Player *ply) {	//301a6
	if (ply->ActionScript->Crouch) {
		ply_exit_crouch(ply);
	} else {
		ply_exit_stand(ply);
	}
}


static int _ChunLiButtons(Player *ply) {				// 3015c
	u16 buttons = (~ply->JoyDecodeDash.full) & ply->JoyDecode.full & BUTTON_MASK;
	if(buttons) {
		if (buttons & BUTTON_A) {
			ply->ButtonStrength = STRENGTH_LOW;
			ply->PunchKick		= PLY_PUNCHING;
		} else if (buttons & BUTTON_B) {
			ply->ButtonStrength = STRENGTH_MED;
			ply->PunchKick		= PLY_PUNCHING;			
		} else if (buttons & BUTTON_C) {
			ply->ButtonStrength = STRENGTH_HIGH;
			ply->PunchKick		= PLY_PUNCHING;			
		} else if (buttons & BUTTON_D) {
			ply->ButtonStrength = STRENGTH_LOW;
			ply->PunchKick		= PLY_KICKING;			
		} else if (buttons & BUTTON_E) {
			ply->ButtonStrength = STRENGTH_MED;
			ply->PunchKick		= PLY_KICKING;			
		} else if (buttons & BUTTON_F) {
			ply->ButtonStrength = STRENGTH_HIGH;
			ply->PunchKick		= PLY_KICKING;			
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

inline static int chunli_holding_down(Player *ply) {		//30302
	if (ply->JoyDecode.full & JOY_DOWN) {
		return TRUE;
	} else {
		return FALSE;
	}
}
inline static int chunli_pressing_up(Player *ply) {		//302f8
	if (ply->JoyDecode.full & JOY_UP) {
		return TRUE;
	} else {
		return FALSE;
	}
}


static u16 sub_301bc(Player *ply, u16 mask) {
	return (PLY_NEWBUTTONS & mask);
}
static u16 sub_301ca(Player *ply, u16 mask) {
	return (~ply->JoyDecode.full & ply->JoyDecodeDash.full & mask);
}

static u16 sub_302ec(Player *ply, u16 mask) {
	if (sub_301bc(ply, mask)== 0) {
		return sub_301ca(ply, mask);
	} else {
		return sub_301bc(ply, mask);
	}
}
inline static void chunli_reset_birdkick(Player *ply) {		//3030c
	UD *ud=(UD *)&ply->UserData;
	
	ud->mode_birdkick = 0;
}
static int sub_302b4(Player *ply, short strength) {
	UD *ud=(UD *)&ply->UserData;
	
	if (check_special_ability(ply)) {
		return 0;
	}
	if (ud->x0085 == 0) {
		return 0;
	}
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = ply->mode3 = 0;
	ud->x0086 = 0;
	ply->Attacking = TRUE;
	ud->x0084 = strength;		//XXX
	CASetAnim1(ply, 0x52);
	return -1;
}
static int sub_303e0(Player *ply, short strength) {
	UD *ud=(UD *)&ply->UserData;
	ud->x0098 = strength;		// only use
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = ply->mode3 = 0;
	ply->Attacking = TRUE;
	ud->x0086 = 2;
	CASetAnim2(ply, 0x58, strength);
	return -1;
}

static void sub_3040a(Player *ply, ChunLiMove *clm, u16 d0, char d1) {
	UD *ud=(UD *)&ply->UserData;
	if (ud->newbuttons & d0) {
		clm->b++;
		clm->a = d1;
	} else {
		if(--clm->a == 0) {
			clm->b = 0;
			clm->a = d1;
		}
	}
}

static int sub_3039a(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	
	if (ud->newbuttons & BUTTON_D) {
		if (ud->x00a5 || ud->cm1.b >= 10) {
			return sub_303e0(ply, 0);
		}
	}
	if (ud->newbuttons & BUTTON_E) {
		if (ud->x00a5 || ud->cm2.b >= 10) {
			return sub_303e0(ply, 0);
		}
	}
	if (ud->newbuttons & BUTTON_F) {
		if (ud->x00a5 || ud->cm2.b >= 10) {
			return sub_303e0(ply, 0);
		}
	}
	return 0;
}	

int chunli_check_birdkick(Player *ply) {			// 301d8 chunli bird kick checker
	UD *ud=(UD *)&ply->UserData;
	
	static const char data_3023a[] = {
		8,9,8,10,9,11,8,9,8,10,8,11,8,12,8,13,8,14,8,15,8,9,8,10,8,11,8,12,8,9,8,10
	};
	
	switch (ud->mode_birdkick) {
		case 0:
			if (chunli_holding_down(ply)) {
				NEXT(ud->mode_birdkick);
				ud->timer_birdkick0 = (char []){0x3c, 0x23, 0xc, 0x1}[g.x0320];
			}
			break;
		case 2:
			if (chunli_holding_down(ply)) {
				if (--ud->timer_birdkick0 == 0) {
					NEXT(ud->mode_birdkick);
				}
			} else {
				chunli_reset_birdkick(ply);
			}
			break;
		case 4:
			if (chunli_holding_down(ply) == 0) {
				NEXT(ud->mode_birdkick);
				ud->timer_birdkick1 = data_3023a[RAND32];
			}
			break;
		case 6:
			if (--ud->timer_birdkick1 == 0) {
				chunli_reset_birdkick(ply);
			} else {
				if (chunli_pressing_up(ply)) {
					NEXT(ud->mode_birdkick);
					ud->birdkick_potential = -1;
				} else {
					return 0;
				}
			}
			/* FALL THRU */
		case 8:
			++ud->birdkick_potential;
			if (ud->birdkick_potential <= 8) {
				if (sub_302ec(ply, BUTTON_F)) {
					return sub_302b4(ply, 2);
				}
			}
			if (ud->birdkick_potential <= 10) {
				if (sub_302ec(ply, BUTTON_E)) {
					return sub_302b4(ply, 1);
				}
			}
			if (ud->birdkick_potential <= 12) {
				if (sub_302ec(ply, BUTTON_D)) {
					return sub_302b4(ply, 0);
				}
			}
			return 0;
			break;
			FATALDEFAULT;
	}
	return 0;
}
int sub_30312(Player *ply) {			// 30312 chunli thousand foot kick
	UD *ud=(UD *)&ply->UserData;
	
	switch (ud->mode_lightningkick) {
		case 0:
			NEXT(ud->mode_lightningkick);
			ud->cm1.a = 1;
			ud->cm1.b = 0;
			ud->cm2.a = 1;
			ud->cm3.a = 1;
			break;
		case 2:
			ud->newbuttons = (~ply->JoyCorrectDash & ply->JoyCorrect);
			sub_3040a(ply, &ud->cm1, BUTTON_D, 15);
			sub_3040a(ply, &ud->cm2, BUTTON_E, 10);
			sub_3040a(ply, &ud->cm3, BUTTON_F,  5);
			if (check_special_ability(ply)) {
				return 0;
			}
			if (ud->x0085 == 0) {
				return 0;
			}
			ud->x00a5 = 0;
			return sub_3039a(ply);
			break;
			FATALDEFAULT;
	}
	return 0;
}



static int sub_304a8(Player *ply, u16 joys) {
	UD *ud=(UD *)&ply->UserData;
	switch (ply->ButtonStrength) {
		case 0:
			ply->Move        = (ply->OppXDist >= 0x15 ? 1 : 0);
			ud->x009f        = 1;
			ud->movedistance = 0x15;
			ud->closemove    = 0;
			ud->farmove      = 1;
			ud->x00a4        = 0;
			ud->x00a5        = 0x10;
			quirkysound(0);			
			break;
		case 2:
			if ((joys & JOY_LRU) && ply->OppXDist < 0x23) {
				PLY_THROW_SET(0xffe0, 0x0035, 0x0020, 0x0010);
				if (throwvalid(ply)) {
					ply->Move = 6;
					return 1;
				}
			}
			ply->Move = ply->OppXDist >= 0x24 ? 3 : 2;
			quirkysound(1);
			return 0;
			break;
		case 4:
			if ((joys & JOY_LRU) && ply->OppXDist < 35) {
				PLY_THROW_SET(0xffe0, 0x0035, 0x0020, 0x0010);
				if (throwvalid(ply)) {
					ply->Move = 7;
					return 1;
				}
			}
			ply->Move = ply->OppXDist >= 0x29 ? 5 : 4;
			quirkysound(2);
			return 0;
			break;
			FATALDEFAULT;
	}
	return 0;
}

static int sub_30592(Player *ply, u16 joys) {
	UD *ud=(UD *)&ply->UserData;
	
	switch (ply->ButtonStrength) {
		case 0:
			ply->Move			= ply->OppXDist >= 0x24 ? 1 : 0;
			ud->x009f			= 1;
			ud->movedistance	= 0x24;
			ud->closemove		= 0;
			ud->farmove			= 1;
			ud->x00a4			= 0x01;
			ud->x00a5			= 0x00;
			quirkysound(0);			
			break;
		case 2:
			ply->Move = ply->OppXDist >= 0x29 ? 4 : 2;
			quirkysound(1);
			break;
		case 4:
			ply->Move = ply->OppXDist >= 0x29 ? 7 : 5;
			quirkysound(2);
			break;
			FATALDEFAULT;
	}
	return 0;
}

static int sub_3042c(Player *ply) {	
	UD *ud=(UD *)&ply->UserData;
	char rand = RAND32;
	
	if (0x5bb1654a & (1<<rand)) {
		if (check_special_ability(ply)) {
			return 0;
		} else if (ud->x0085 == 0) {
			return 0;
		} else {
			ud->x00a5 = TRUE;
			return sub_3039a(ply);
		}
	} else {
		if (rand & 0x40) {	// can't happen?! BTST #10
			return sub_302b4(ply, 2);
		} else if (rand & 0x20) {		// again, can't happen?
			return sub_302b4(ply, 1); 
		} else {
			return sub_302b4(ply, 0);
		}
	}
}	

static int sub_30830(Player *ply) {
	if (ply->YPI >= 0x20 && ply ->YPI < 0x30) {
		return 0;
	} else {
		return 1;
	}	
	
}

static void sub_306ee(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	if (sub_30830(ply) && (ply->JoyDecode.full & JOY_LRD)) {
		PLY_THROW_SET(0xffe2, 0x0038, 0x0035, 0x002e);
		if (airthrowvalid(ply)) {
			ud->didairthrow = TRUE;
			ply->Move       = 6;
			return;
		}
	}
	ply->Move = ply->VelX.full == 0 ? 1 : 4;
	quirkysound(1);
}	
static void sub_3073e(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	if (sub_30830(ply) && (ply->JoyDecode.full & JOY_LRD)) {
		PLY_THROW_SET(0xffe2, 0x0038, 0x0025, 0x0024);
		if (airthrowvalid(ply)) {
			ud->didairthrow = TRUE;
			ply->Move       = 7;
			return;
		}
	}
	ply->Move = ply->VelX.full == 0 ? 2 : 5;
	quirkysound(2);
}	
static int sub_30476(Player *ply, u16 buttons_d5) {
	UD *ud=(UD *)&ply->UserData;
	buttons_d5 &= BUTTON_KICKS;
	
	if(buttons_d5 && LBRareChance()) {
		return sub_3042c(ply);
	} else {
		ud->x009f       = FALSE;
		ply->StandSquat = PLY_STAND;
		switch (ply->PunchKick) {
			case 0:
				return sub_304a8(ply, ply->JoyCorrect);
				break;
			case 2:
				return sub_30592(ply, ply->JoyCorrect);
				break;
				FATALDEFAULT;
		}
		g.HumanMoveCnt++;
		return 1;
	}
}
static int sub_30614(Player *ply, u16 buttons_d5) {
	UD *ud=(UD *)&ply->UserData;
	buttons_d5 &= BUTTON_KICKS;
	
	if(buttons_d5 && LBRareChance()) {
		return sub_3042c(ply);
	} else {
		ply->StandSquat = PLY_CROUCH;
		ply->Move = ply->ButtonStrength / 2;
		if (ply->Move) {
			ud->x009f			= TRUE;
			ud->movedistance	= 0x33;
			ud->closemove		= 0;
			ud->farmove			= 0;
			if (ply->PunchKick) {
				ud->x00a4 = 0x10;
				ud->x00a5 = 0;
			} else {
				ud->x00a4 = 0;
				ud->x00a5 = 0x10;
			}
		}
		quirkysound(ply->Move);
		g.HumanMoveCnt++;
		return 1;
	}
}


#pragma mark Human move callbacks

int PLCBStandChunLi(Player *ply) {		// 3011e
	UD *ud=(UD *)&ply->UserData;
	
	if (_ChunLiButtons(ply)) {
		return sub_30476(ply, _ChunLiButtons(ply));
	}
	ud->x0085 = 5;
	return 0;
}
int PLCBCrouchChunLi(Player *ply) {		// 30130
	UD *ud=(UD *)&ply->UserData;
	
	if (_ChunLiButtons(ply)) {
		return sub_30614(ply, _ChunLiButtons(ply));
	}
	ud->x0085 = 5;
	return 0;
}
int PLCBJumpChunLi(Player *ply) {		// 30142
	UD *ud=(UD *)&ply->UserData;
	
	if (_ChunLiButtons(ply)) {
		ply->StandSquat = PLY_JUMP;
		switch (ply->PunchKick) {
			case PLY_PUNCHING:
				ud->didairthrow = 0;
				switch (ply->ButtonStrength) {
					case STRENGTH_LOW:
						ply->Move = ply->VelX.full == 0 ? 0 : 3;
						quirkysound(0);
						break;
					case STRENGTH_MED:
						sub_306ee(ply);
						break;
					case STRENGTH_HIGH:
						sub_3073e(ply);
						break;
						FATALDEFAULT;
				}
				CASetAnim2(ply, STATUS_JUMP_PUNCH, ply->Move);
				break;
			case PLY_KICKING:
				ud->didairthrow = 0;
				switch (ply->ButtonStrength) {
					case STRENGTH_LOW:
						ply->Move = ply->VelX.full == 0 ? 0 : 5;
						quirkysound(0);
						break;
					case STRENGTH_MED:
						if (ply->VelX.full == 0) {
							ply->Move = 3;
							if (ply->JoyDecode.full & JOY_DOWN) {
								ply->Move       = 1;
								ud->didairthrow = TRUE;
								ud->x0092       = 0;
							}
							quirkysound(1);
						} else {
							ply->Move = 6;
							if (ply->JoyDecode.full & JOY_DOWN) {
								ply->Move       = 8;
								ud->didairthrow = TRUE;
								ud->x0090       = 0;		// XXX checkme
							}
							quirkysound(1);
						}
						break;
					case STRENGTH_HIGH:
						if (ply->VelX.full == 0) {
							ply->Move = 4;
							quirkysound(2);
						} else {
							ply->Move = 7;
							quirkysound(2);
						}
						break;
						FATALDEFAULT;
				}
				CASetAnim2(ply, STATUS_JUMP_KICK, ply->Move);
				break;
				FATALDEFAULT;
		}
		g.GPCollX += 1;
		if (ud->didairthrow) {
			ply->mode1 = 10;
			return -1;
		} else {
			return 1;
		}		
	}
	ud->x0085 = 5;
	return 0;
}

int PLCBPowerChunLi(Player *ply) {			// 30154 chunli powermove
	chunli_check_birdkick(ply);
	return sub_30312(ply);
}
static void chunli_attack_crouch(Player *ply) {		// 308b2 
	UD *ud=(UD *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CASetAnim2(ply, (short []){STATUS_PUNCH, STATUS_KICK, 0x44,0x46}
					   [ply->StandSquat + (ply->PunchKick/2)],
					   ply->Move);
			break;
		case 2:
			if (AF1) {
				chunli_exit(ply);
			} else if (ud->x009f != 0 && AF2 != 0 ) {		// look at code for possible bugs here
				if (PSSetNextAction(ply)) {
					plstat_do_nextaction(ply);
				} else {
					g.HumanMoveCnt++;
					ply->Opponent->SufferHB5 = 0;
					if (ply->OppXDist < ud->movedistance) {
						ply->Move = ud->closemove;
					} else {
						ply->Move = ud->farmove;
					}
					CASetAnim2(ply, (short []){
						STATUS_PUNCH, STATUS_KICK,
						STATUS_CROUCH_PUNCH, STATUS_CROUCH_KICK,
					}
						[ply->StandSquat + (ply->PunchKick/2)],
						ply->Move
					);
					
				}
			} else {
				PLAYERTICK;
			}
			break;
			FATALDEFAULT;
	}
}
static void sub_30942(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->Flip = ply->JoyDecode.full & JOY_RIGHT;
			CASetAnim2(ply, STATUS_PUNCH, ply->Move);
			soundsting(SOUND_CHUNLI_YUP);
			break;
		case 2:
			PLAYERTICK;
			if (AF2 & 0x80) {
				NEXT(ply->mode2);
				ply->EnemyDirection = ply->Flip;
				ply->LocalTimer = 10;
				set_throw_trajectory(ply, 12, ply->Flip, 14);
			}
			break;
		case 4:
			if (--ply->LocalTimer) {
				NEXT(ply->mode2);
			}
			break;
		case 6:
			PLAYERTICK;
			if (AF1) {
				ply_exit_stand(ply);
			}
			break;
			FATALDEFAULT;
	}
}
static void sub_309ee(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CASetAnim2(ply, STATUS_KICK, ply->Move);
			break;
		case 2:
			PLAYERTICK;
			if (AF2 == 0) {
				NEXT(ply->mode2);
				ply->VelY.full = 0x780;
				ply->VelX.full = ply->Flip ? -0x180 : 0x180;
				ply->AclY.full = 0x68;
				ply->AclX.full = 0;
				ply->Jumping = 1;
				ply->YPI += 0x10;
			}
			break;
		case 4:
			CATrajectory((Object *)ply);
			if (PLAYERGROUND) {
				soundsting(SOUND_IMPACT8);
				ply->Jumping = 0;
				ply_exit_stand(ply);
			} else {
				PLAYERTICK;
			}
			break;
			FATALDEFAULT;
	}
}
static void sub_30a6a(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->VelX.full = ply->Flip ? 0x780 : - 0x780;
			ply->VelY.full = 0x180;
			ply->AclY.full = 0x50;
			ply->AclX.full = 0;
			ply->Jumping = 1;
			ply->YPI += 0x10;
			CASetAnim2(ply, STATUS_KICK, ply->Move);
			break;
		case 2:
			CATrajectory((Object *)ply);
			if (PLAYERGROUND) {
				soundsting(SOUND_IMPACT8);
				ply->Jumping = 0;
				ply->Flip ^= 1;
				ply_exit_stand(ply);
			} else {
				PLAYERTICK;
			}
		FATALDEFAULT;
	}
}
static void sub_30884(Player *ply) {
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->Move) {
				case 0:		case 1:		case 2:		case 3:
				case 4:		case 5:
					chunli_attack_crouch(ply);
					break;
				case 6:		case 7:		// throws
					sub_30942(ply);
					break;
					FATALDEFAULT;
			}
			break;
		case PLY_KICKING:
			switch (ply->Move) {
				case 0:		case 1:		case 2:		case 4:
				case 5:		case 7:
					chunli_attack_crouch(ply);
					break;
				case 3:
					sub_309ee(ply);
					break;
				case 6:
					sub_30a6a(ply);
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}

static int sub_30c46(Player *ply) {
	if(get_scr1x() + 0x60 > ply->XPI) {
		return 1;
	}
	if (get_scr1x() + 0x60 + 0xc0 < ply->XPI) {
		ply->VelX.full = - ply->VelX.full;
		ply->AclX.full = -ply->AclX.full;
		return 1;
	}
	return 0;
}
static void chunli_attack_jump(Player *ply) {			// 30afe
	UD *ud=(UD *)&ply->UserData;
	short d6;
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					if (ply->JoyDecode.full & JOY_RIGHT) {
						ply->Flip = FACING_RIGHT;
					} else {
						ply->Flip = FACING_LEFT;
					}
					ply->Timer2 = 12;
					soundsting(SOUND_CHUNLI_YUP);
					break;
				case 2:
					CATrajectory((Object *)ply);
					if (ply->VelY.full < 0) {
						if (PLAYERGROUND) {
							NEXT(ply->mode2);
							CASetAnim2(ply, STATUS_JUMP_PUNCH, 8);
							return;
						}
					}
					if (AF2 & 0x80) {
						ply->mode1 = 4;
						ply->mode2 = 6;
						ply->mode3 = 0;
						ply->Attacking = 0;
						ply->EnemyDirection = ply->Flip;
						set_throw_trajectory(ply, 8, ply->Flip, 14);
					} else {
						PLAYERTICK;
					}
					break;
				case 4:
					NEXT(ply->mode2);
					break;
				case 6:
					if (AF1) {
						ply_exit_stand(ply);
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case PLY_KICKING:
			if (g.GPHitBoxCoords[1][0] == 0) {
				if (ud->x0092) {
					ply->mode1 = 4;
					ply->mode2 = 2;
					ply->mode3 = 0;
					ply->Attacking = FALSE;
					
					d6 = ply->VelX.full;
					
					ply->VelX.full = 0x400;
					ply->VelY.full = 0x600;
					ply->AclX.full = 0x5;
					ply->AclY.full = 0x48;
					if (sub_30c46(ply)== 0) {
						if (d6 == 0) {
							if (ply->Flip == FACING_LEFT) {
								ply->VelX.full = -ply->VelX.full;
							}
						} else if (d6 < 0) {
							ply->VelX.full = -ply->VelX.full;
							ply->AclX.full = -ply->AclX.full;
						}
					}
					return;
				}
			}
			CATrajectory((Object *)ply);
			if (ply->VelY.full < 0 && PLAYERGROUND) {
				ply_exit_air(ply);
			} else {
				if (AF1) {
					return;
				}
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
	
}

void PSCBAttackChunLi(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	if (ply->Timer2) {
		if (--ply->Timer2) {
			ud->x0092 = ply->Timer2;
		}
	} else {
		if (ud->x0085) {
			--ud->x0085;
			switch (ply->StandSquat) {
				case PLY_STAND:
					sub_30884(ply);
					break;
				case PLY_CROUCH:
					chunli_attack_crouch(ply);
					break;
				case PLY_JUMP:
					chunli_attack_jump(ply);
					break;
				FATALDEFAULT;
			}
		}
	}
}
