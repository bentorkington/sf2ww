/*
 *  chunli.c
 *  GLUTBasics
 *
 *  Created by Ben on 10/02/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
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
#include "chunlidata.h"

typedef struct UserData_ChunLi UD;
typedef struct UserDataComp_ChunLi UDCOMP;
extern Game g;


void pl_cb_setstatus2_chunli(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_56dbc[status / 2], argd0);
}
void pl_cb_setstatus3_chunli(Player *ply, short status) {
	pl_cb_setstatus2_chunli(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_chunli(Player *ply, short status) {
	pl_cb_setstatus2_chunli(ply, status, 0);
}


static void sub_301a6(Player *ply) {
	if (ply->ActionScript->Crouch) {
		ply_exit_crouch(ply);
	} else {
		ply_exit_stand(ply);
	}
}


static int _ChunLiButtons(Player *ply) {				// 3015c
	u16 buttons = (~ply->JoyDecodeDash.full) & ply->JoyDecode.full & BUTTON_MASK;
	if(buttons) {
		if (buttons & 0x10) {
			ply->ButtonStrength = STRENGTH_LOW;
			ply->PunchKick		= PLY_PUNCHING;
		} else if (buttons & 0x20) {
			ply->ButtonStrength = STRENGTH_MED;
			ply->PunchKick		= PLY_PUNCHING;			
		} else if (buttons & 0x40) {
			ply->ButtonStrength = STRENGTH_HIGH;
			ply->PunchKick		= PLY_PUNCHING;			
		} else if (buttons & 0x100) {
			ply->ButtonStrength = STRENGTH_LOW;
			ply->PunchKick		= PLY_KICKING;			
		} else if (buttons & 0x200) {
			ply->ButtonStrength = STRENGTH_MED;
			ply->PunchKick		= PLY_KICKING;			
		} else if (buttons & 0x400) {
			ply->ButtonStrength = STRENGTH_HIGH;
			ply->PunchKick		= PLY_KICKING;			
		}
		return 1;
	} else {
		return 0;
	}
}

inline static int chunli_holding_down(Player *ply) {		//30302
	if (ply->JoyDecode.full & 4) {
		return 1;
	} else {
		return 0;
	}
}
inline static int chunli_pressing_up(Player *ply) {		//302f8
	if (ply->JoyDecode.full & 8) {
		return 1;
	} else {
		return 0;
	}
}


u16 sub_301bc(Player *ply, u16 mask) {
	return (~ply->JoyDecodeDash.full & ply->JoyDecode.full & mask);
}
u16 sub_301ca(Player *ply, u16 mask) {
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

int sub_301d8(Player *ply) {			// 301d8 chunli bird kick checker
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
			ply->Move = ply->OppXDist >= 0x15 ? 1 : 0;
			ud->x009f = 1;
			ud->movedistance = 0x15;
			ud->closemove = 0;
			ud->farmove = 1;
			ud->x00a4 = 0;
			ud->x00a5 = 0x10;
			quirkysound(0);			
			break;
		case 2:
			if ((joys & 0xb) && ply->OppXDist < 0x23) {
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
			if ((joys & 0xb) && ply->OppXDist < 0x23) {
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
			ply->Move = ply->OppXDist >= 0x24 ? 1 : 0;
			ud->x009f = 1;
			ud->movedistance = 0x24;
			ud->closemove = 0;
			ud->farmove = 1;
			ud->x00a4 = 0x01;
			ud->x00a5 = 0x00;
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
			sub_3039a(ply);
		}
	} else {
		if (rand & 0x40) {	// can't happen?! BTST #10
			sub_302b4(ply, 2);
		} else if (rand & 0x20) {		// again, can't happen?
			sub_302b4(ply, 1); 
		} else {
			sub_302b4(ply, 0);
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
	if (sub_30830(ply) && (ply->JoyDecode.full & 7)) {
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
	if (sub_30830(ply) && (ply->JoyDecode.full & 7)) {
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
	buttons_d5 &= 0x700;
	
	if(buttons_d5 && LBRareChance()) {
		return sub_3042c(ply);
	} else {
		// 30482
		ud->x009f = FALSE;
		ply->StandSquat = 0;
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
	buttons_d5 &= 0x700;
	
	if(buttons_d5 && LBRareChance()) {
		return sub_3042c(ply);
	} else {
		ply->StandSquat = 2;
		ply->Move = ply->ButtonStrength / 2;
		if (ply->Move) {
			ud->x009f = TRUE;
			ud->movedistance = 0x33;
			ud->closemove = 0;
			ud->farmove = 0;
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
		ply->StandSquat = 4;
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
							if (ply->JoyDecode.full & 4) {
								ply->Move = 1;
								ud->didairthrow = 1;
								ud->x0092 = 0;
							}
							quirkysound(1);
						} else {
							ply->Move = 6;
							if (ply->JoyDecode.full & 4) {
								ply->Move = 8;
								ud->didairthrow = 1;
								ud->x0090 = 0;		// XXX checkme
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
	sub_301d8(ply);
	return sub_30312(ply);
}
static void sub_308b2(Player *ply) {		// 308b2 sm for most moves
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
				sub_301a6(ply);
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
					CASetAnim2(ply, (short []){STATUS_PUNCH,STATUS_KICK,0x44,0x46}
							   [ply->StandSquat + (ply->PunchKick/2)],
							   ply->Move);
					
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
			ply->Flip = ply->JoyDecode.full & 1;
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
					sub_308b2(ply);
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
					sub_308b2(ply);
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
static void sub_30afe(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	short d6;
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					if (ply->JoyDecode.full & 1) {
						ply->Flip = 1;
					} else {
						ply->Flip = 0;
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
							if (ply->Flip == 0) {
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
					sub_308b2(ply);
					break;
				case PLY_JUMP:
					sub_30afe(ply);
					break;
				FATALDEFAULT;
			}
		}
	}
}

void PSCBVictoryChunLi(Player *ply) {
	u16 d2;
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->Flip ^= 1;
			d2 = 0xaaaa;
			if (g.OnBonusStage) {
				d2 = 0x1044;
				if (ply->RoundsWon != 1) {
					d2 = 0xefbb;
				}
			}
			if (d2 & (1 << (RAND16))) {
				CASetAnim2(ply, 0x50, 1);
			} else {
				CASetAnim2(ply, 0x50, 0);
			}
			break;
		case 2:
			PLAYERTICK;
			if (AF2) {
				NEXT(ply->mode3);
				g.FightOver = TRUE;
				ply->PSFinishedParticipating = TRUE;
			}
			break;
		case 4:
			if (AF2 & 0x80) {
				PLAYERTICK;
			} else {
				soundsting(AF2);
			}
			break;
		FATALDEFAULT;
	}
}
#pragma mark Computer ChunLi

static void sub_34d9a(Player *ply) {		// 34d9a
	CASetAnim2(
			   ply, 
			   (short []){STATUS_PUNCH, 0x42, 0x44, 0x46}[ply->StandSquat + ply->PunchKick/2],
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
	short y = ply->YPI - 0x28;
	
	if (y < 0 || y >= 0x30) {
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

