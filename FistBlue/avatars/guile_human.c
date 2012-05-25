/*
 *  guile_human.c
 *  MT2
 *
 *  Created by Ben on 25/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
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

extern Game g;
typedef struct UserData_Guile UD;

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

static short GuileStandMove(Player *ply) {		/* 2f3b2*/
	++g.HumanMoveCnt;
	ply->StandSquat = PLY_STAND;
	
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->ButtonStrength) {
				case 0:
					if (ply->OppXDist >= 20) {
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
			switch (ply->ButtonStrength) {
				case 0:
					ply->Move = 0;
					quirkysound(0);
					return 1;
					break;
				case 2:
					if (ply->JoyDecode.full == 0 || ply->OppXDist >= 0x33) {
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
						if (ply->OppXDist >= 40) {
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
	ply->StandSquat = PLY_CROUCH;
	
	QUICKMOVE(ply->ButtonStrength / 2); return TRUE; 
}
static short guile_check_air_throw_allowed(Player *ply) {		// 2f6fa
	if (ply->VelY.full < 0) {
		if (ply->YPI < 40 || ply->YPI >= 88) {
			return FALSE;
		} else {
			return TRUE;
		}
	} else {
		if (ply->YPI < 40 || ply->YPI >= 71) {
			return FALSE;
		} else {
			return TRUE;
		}
	}
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
					if (guile_check_air_throw_allowed(ply) && (ply->JoyDecode.full & 0x7)) {
						PLY_THROW_SET(-30, 56, 37, 36);
						if (airthrowvalid(ply)) {
							ply->Move = 0;
							ud->AirThrow = TRUE;
						} else {
							ply->Move = (ply->VelX.full == 0 ? 4 : 1);							quirkysound(1);
						}
					} else {
						ply->Move = (ply->VelX.full == 0 ? 4 : 1);
						quirkysound(1);
					}
					break;
				case 4:
					// 2f6a8
					if (guile_check_air_throw_allowed(ply) && (ply->JoyDecode.full & 7)) {
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
					if (guile_check_air_throw_allowed(ply) && (ply->JoyDecode.full & 7)) {
						PLY_THROW_SET(-30, 56, 37, 36);
						if (airthrowvalid(ply)) {
							ply->Move     = 1;
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
					if (guile_check_air_throw_allowed(ply) && (ply->JoyDecode.full & 7)) {
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
static void _GuileStartSonic(Player *ply) {		/* 2f1ea */
	UD *ud=(UD *)&ply->UserData;
	
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = ply->mode3 = 0;
	ud->SonicStep = 0;
	ud->x008b = 0;
	ud->SonicStrength = ply->ButtonStrength / 2;
	BumpDiff_PowerMove();
	g.HumanMoveCnt++;
}


static void _GuileStartBlade(Player *ply) {		/* 2f362 */
	UD *ud=(UD *)&ply->UserData;
	
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = ply->mode3 =0;
	ud->BladeStep = 0;
	ply->Attacking = TRUE;
	ud->x008b = 0;
	ud->x008c = ply->ButtonStrength / 2;
	BumpDiff_PowerMove();
	g.HumanMoveCnt++;
}
inline static short guile_check_free_sonic(Player *ply) {	/* 2f042 check if Guile can do Sonic boom */
	UD *ud=(UD *)&ply->UserData;
	
	if(check_special_ability(ply) || ply->Projectile != 0 || AF2 == 0x42 || ud->x008d > 5) {
		return FALSE;
	}
	return TRUE;
}
inline static short guile_check_free_blade(Player *ply) {	/* 2f066 */
	UD *ud=(UD *)&ply->UserData;
	
	if(check_special_ability(ply) || AF2==0x42 || AF2 == 0x24 || ud->x008d > 5) {
		return FALSE;
	}
	return TRUE;
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
				if (guile_check_free_sonic(ply)) {
					_GuileStartSonic(ply);		/* free sonic boom */
				}
				break;
			case PLY_KICKING:
				if (guile_check_free_blade(ply)) {
					_GuileStartBlade(ply);		/* free bladekick */
				}
				break;
				FATALDEFAULT;
		}
		return -1;
		
	} 
	return 1;
}

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
static void guile_reset_sonic_sm(Player *ply) {			//2f1d0
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
static void sub_2f1b0(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	
	if(check_special_ability(ply) || AF2==0x42 || AF2 == 0x24 || ud->x008d > 5) {
		guile_reset_sonic_sm(ply);
	} else {
		_GuileStartSonic(ply);
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
		guile_reset_sonic_sm(ply);
	} else {
		sub_2f1b0(ply);
	}	
}
static short GuileNewButtonsDown(Player *ply, short mask ) {		/* 2f396 */
	return (PLY_NEWBUTTONS & mask);
}
static short GuileNewButtonsUp(Player *ply, short mask ) {		/* 2f396 */
	return (~ply->JoyDecode.full & ply->JoyDecodeDash.full & mask);
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
			if (ply->JoyCorrect2 & JOYCO_AWAY) {
				if(--ud->SonicTimer == 0) {
					NEXT(ud->SonicStep);
				}
			} else {
				ud->SonicTimer = data_2f0c0[g.x0320];
			}
			break;
		case 4:
			if ((ply->JoyCorrect2 & JOYCO_AWAY) == 0) {
				NEXT(ud->SonicStep);
				ud->SonicTimer2 = data_2f12c[RAND32];
			}
			break;
		case 6:
			if (--ud->SonicTimer2 == 0) {
				ud->SonicStep = 0;
			} else if (ply->JoyCorrect2 == JOYCO_TOWARD) {
				if(GuileNewButtonsDown(ply, BUTTON_PUNCHES)) {
					sub_2f1b0(ply);
				} else if (GuileNewButtonsUp(ply, BUTTON_PUNCHES)) {
					sub_2f1b0(ply);
				} else {
					ud->x0083 = 8;
					ud->x0084 = 6;
					NEXT(ud->SonicStep);
				}
			}
			break;
		case 8:
			d0 = GuileNewButtonsDown(ply, BUTTON_PUNCHES); 
			if (d0) {
				sub_2f19a(ply, d0);
			} else {
				d0 = GuileNewButtonsUp(ply, BUTTON_PUNCHES);
				if (d0) {
					sub_2f19a(ply, d0);
				} else {
					guile_reset_sonic_sm(ply);
				}
			}
			break;
			FATALDEFAULT;
	}
}
static void sub_2f326(Player *ply) {		// 2f326
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
			if (ply->JoyCorrect2 & JOYCO_DOWN) {
				if (--ud->BladeTimer == 0) {
					NEXT(ud->BladeStep);
				}
			} else {
				ud->BladeTimer = data_2f32e[g.x0320];
			}
			break;
		case 4:
			if ((ply->JoyCorrect2 & JOYCO_DOWN) == 0) {
				NEXT(ud->BladeStep);
				ud->BladeTimer2 = data_2f29a[RAND32];
			}
			break;
		case 6:
			if (--ud->BladeTimer2 == 0) {
				ud->BladeStep = 0;
			} else {
				if (ply->JoyCorrect2 & JOYCO_UP) {
					if(d0 = GuileNewButtonsDown(ply, BUTTON_KICKS)) {
						decode_buttons(ply, d0);
						sub_2f326(ply);
					} else if (d0 = GuileNewButtonsUp(ply, BUTTON_KICKS)) {
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
			if(d0 = GuileNewButtonsDown(ply, BUTTON_KICKS)) {
				decode_buttons(ply, d0);
			} else if (d0 = GuileNewButtonsUp(ply, BUTTON_KICKS)) {
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
		FATALDEFAULT;
	}
}

void PLCBPowerGuile(Player *ply) {	//2effa
	_GuileSMSonic(ply);
	_GuileSMBlade(ply);
	
}
void PSCBPowerGuile(Player *ply) {		/* 2fe26 */
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
								obj->exists		= TRUE;
								obj->Sel		= SF2_PROJ_SONICBOOM;
								obj->XPI		= ply->XPI;
								obj->YPI		= ply->YPI;
								obj->Flip		= ply->Flip;
								obj->SubSel		= ply->ButtonStrength;
								obj->Owner      = ply;
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
						ply->VelY.full = (short[]){0x800, 0x900, 0xa00}[ud->x008c];
						ply->AclY.full = 0x60;	// redundant table
						ply->Airborne  = AIR_JUMPING;
						CASetAnim2(ply, 0x4e, ud->x008c);
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
						NEXT(ply->mode2);	
						set_throw_trajectory(ply, 0, ply->Flip, 13);
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
						ply->Timer2           = 12;
						ply->Opponent->Timer2 = 12;
						NEXT(ply->mode2);
						sub_36d6(ply, 
								 (ply->Flip ? 128 : -128),
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
			STDANIM(STATUS_KICK, _GuileExitStand);
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
			STDANIM(STATUS_CROUCH_PUNCH, _GuileExitCrouch);
			break;
		FATALDEFAULT;
	}
}
//Guile Crouching Kicks 
static void sub_2fbac(Player *ply) {		// 2fbac
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

static void guile_attack_jump(Player *ply) {		// 2fc80
	switch (ply->Move) {
		case 0:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					if ((ply->JoyCorrect & 0xb) || (ply->JoyCorrect & JOYCO_TOWARD) == 0) {
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
					if ((ply->JoyCorrect & 0xb) || (ply->JoyCorrect & JOYCO_TOWARD) == 0) {
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

void PSCBAttackGuile(Player *ply) {		
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
			case PLY_JUMP:
				guile_attack_jump(ply);
				break;
			FATALDEFAULT;
		}
	}
}

