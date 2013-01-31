/*
 *  zangeif.c
 *  MT2
 *
 *  Created by Ben on 16/05/12.
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
#include "playerstate.h"
#include "actions.h"
#include "sound.h"
#include "sf2io.h"
#include "computer.h"

#include "lib.h"
#include "gfxlib.h"


#include "zangeif.h"
#include "zangeifdata.h"

extern Game g;
extern GState gstate_Scroll2;
int g_zangeif_d5;



#define ZANGEIF_USER_COMMON			\
signed char	x0094;					\


struct UserData_Zangeif {
	ZANGEIF_USER_COMMON
	signed char mode_power;
	signed char power_sel;
	signed char	power_potential;
	signed char	x0084;
	signed char	special_holdoff;
	signed char	x0086;
	signed char	x0087;
	DUAL		x0088;
	short		volley_joymask;		// Joymask for NextAction
	short		distance;
	signed char	closemove;		// 008e close move
	signed char farmove;		// 008f far move
	short		grip_select;	
};
struct UserDataComp_Zangeif {
	ZANGEIF_USER_COMMON
	signed char	x0084;
	
	signed char x0086;
	signed char x0087;
	short		x0088;		// overlaps with 89, checked
	signed char comp_closemove;		// close move
	signed char comp_farmove;		// far move
	signed char	x008b;
	short		comp_distance;		// distance
	short		x008e;		
};

typedef struct UserData_Zangeif UD;
typedef struct UserDataComp_Zangeif UDCOMP;

void pl_cb_setstatus2_zangeif(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_5de06[status / 2], argd0);
}
void pl_cb_setstatus3_zangeif(Player *ply, short status) {
	pl_cb_setstatus2_zangeif(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_zangeif(Player *ply, short status) {
	pl_cb_setstatus2_zangeif(ply, status, 0);
}


static int zang_setupthrow(Player *ply) {
	PLY_THROW_SET(0xffdb, 0x0035, 0x0025, 0x0010);
	return throwvalid(ply);
}
inline static void sub_315b8(Player *ply) {
	CASetAnim2(ply, 
			   (short[]){STATUS_PUNCH, STATUS_KICK, 0x44, 0x46}[((ply->StandSquat * 2) + ply->PunchKick)/2], 
			   ply->Move
			   );	
}
static void sub_3163e(Player *ply) {
	if (ply->ActionScript->Crouch) {
		ply_exit_crouch(ply);
	} else {
		ply_exit_stand(ply);
	}
}	
static void sub_315a4(Player *ply) {
	UD *ud = (UD*)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			sub_315b8(ply);
			break;
		case 2:
			if (AF1) {
				sub_3163e(ply);
			} else if (ud->x0088.part.p1 == 0 || AF2 == 0 ||
					   (PLY_NEWBUTTONS & ud->volley_joymask) == 0) {
				PLAYERTICK;
			} else if (PSSetNextAction(ply)) {
				plstat_do_nextaction(ply);
			} else {
				quirkysound(ply->PunchKick / 2);
				++g.HumanMoveCnt;
				ply->Opponent->SufferHB5 = 0;
				if (ply->OppXDist >= ud->distance) {
					ply->Move = ud->farmove;					
				} else {
					ply->Move = ud->closemove;
				}
				sub_315b8(ply);
			}
			break;
		FATALDEFAULT;
	}
}

#pragma mark Standing moves
static int sub_30f9e(Player *ply) {
	int buttons = PLY_NEWBUTTONS & BUTTON_MASK;
	if (buttons) {
		g_zangeif_d5 = buttons;
		if (buttons & 0x10) {
			ply->PunchKick      = PLY_PUNCHING;
			ply->ButtonStrength = 0;
		} else if (buttons & 0x20) {
			ply->PunchKick      = PLY_PUNCHING;
			ply->ButtonStrength = 2;			
		} else if (buttons & 0x40) {
			ply->PunchKick		= PLY_PUNCHING;
			ply->ButtonStrength = 4;
		} else if (buttons & 0x100) {
			ply->PunchKick		= PLY_KICKING;
			ply->ButtonStrength = 0;
		} else if (buttons & 0x200) {
			ply->PunchKick		= PLY_KICKING;
			ply->ButtonStrength = 2;
		} else {
			ply->PunchKick		= PLY_KICKING;
			ply->ButtonStrength = 4;
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

static void sub_31044(Player *ply) {
	ply->Move = 8;
	soundsting(SOUND_HUA);
}
static void sub_31056(Player *ply) {		// zang punch little
	UD *ud = (UD*)&ply->UserData;
	quirkysound(0);
	ply->Move = (ply->OppXDist > 40 ? 1 : 0);
	ud->x0088.part.p1 = 1;
	ud->distance = 1;
	ud->closemove = 0;
	ud->farmove = 1;
	ud->volley_joymask = BUTTON_A;
	// suicide code:
}
static void sub_310b4(Player *ply) {		// zang punch mid
	UD *ud = (UD*)&ply->UserData;
	if (ply->JoyDecode.full & 0xb) {
		if (ply->OppXDist <= 40 ) {
			if (zang_setupthrow(ply)) {
				ply->Move = 4;
				return;
			}
		}
		if (ply->OppXDist <= 0x38) {
			if (zang_setupthrow(ply)) {
				ud->grip_select = 0;
				ud->x0088.full = 0x00b4;
				ply->Move = 5;
				return;
			}
		}
	}
	quirkysound(1);
	ply->Move = 2;
}
static void sub_31134(Player *ply) {		// zang punch hi
	UD *ud = (UD*)&ply->UserData;
	if (ply->JoyDecode.full & 0xb) {
		if (ply->OppXDist <= 40 ) {
			if (zang_setupthrow(ply)) {
				ply->Move = 6;
				return;
			}
		}
		if (ply->OppXDist <= 0x38) {
			if (zang_setupthrow(ply)) {
				ud->grip_select = 1;
				ply->Move = 7;
				return;
			}
		}
	}
	quirkysound(2);
	ply->Move = 3;
}
static void sub_311c2(Player *ply) {		// zang kick lo
	quirkysound(0);
	ply->Move = (ply->OppXDist > 32 ? 0 : 1);
}
static void sub_311e0(Player *ply) {		// zang kick mid
	if (ply->JoyDecode.full & 0xb) {
		if (ply->OppXDist <= 0x30 ) {
			if (zang_setupthrow(ply)) {
				ply->Move = 6;
			}
		}
	}
	quirkysound(1);
	ply->Move = (ply->OppXDist > 0x38 ? 3 : 2);
}
static void sub_31234(Player *ply) {		// zang kick hi
	if (ply->JoyDecode.full & 0xb) {
		if (ply->OppXDist <= 0x30 ) {
			if (zang_setupthrow(ply)) {
				ply->Move = 7;
				return;
			}
		}
	}
	quirkysound(2);
	ply->Move = (ply->OppXDist > 0x38 ? 4 : 5);
}
static int sub_31d92(Player *ply, int d6) {
	UD *ud = (UD*)&ply->UserData;
	ud->x0084 = d6;
	if (check_special_ability(ply) || ud->special_holdoff == 0 || zang_setupthrow(ply)== 0) {
		return 0;
	}
	ud->x0087 = (ply->JoyDecode.full & 1 ? 0 : 1);
	ply->mode1 = 0xc;
	ply->mode2 = 0;
	ply->mode3 = 0;
	ply->Attacking = TRUE;
	return -1;
}
static int sub_31d56(Player *ply, int buttons) {
	UD *ud = (UD*)&ply->UserData;
	if (buttons & BUTTON_A) {
		return sub_31d92(ply, 0);
	} else if (buttons & BUTTON_B && ud->power_potential <= 10) {
		return sub_31d92(ply, 1);
	} else if (buttons & BUTTON_C && ud->power_potential <= 8) {
		return sub_31d92(ply, 2);
	} else {
		return 0;
	}
}
int PLCBStandZangeif(Player *ply) {
	UD *ud = (UD*)&ply->UserData;
	if (sub_30f9e(ply)) {
		ud->x0088.part.p1 = 0;
		++g.HumanMoveCnt;
		// d6= joydecode.full
		ply->StandSquat = PLY_STAND;
		switch (ply->PunchKick) {
			case PLY_PUNCHING:
				if (g_zangeif_d5 & 0x70) {
					if (LBRareChance()) {
						if (0x22844224 & (1<<RAND32)) {
							return sub_31d56(ply, g_zangeif_d5 & 0x70);
						} else {
							sub_31044(ply);
							return TRUE;
						}
					} else {
						if (g_zangeif_d5 & 0x70 == 0x70) {
							sub_31044(ply);
							return TRUE;
						}
					}
				}
				switch (ply->ButtonStrength) {
					case 0: sub_31056(ply); break;
					case 2: sub_310b4(ply); break;
					case 4: sub_31134(ply); break;
					FATALDEFAULT;
				}
				break;
			case PLY_KICKING:
				switch (ply->ButtonStrength) {
					case 0: sub_311c2(ply);	break;
					case 2: sub_311e0(ply); break;
					case 4: sub_31234(ply); break;
					FATALDEFAULT;
				}
				break;
			FATALDEFAULT;
		}
		return TRUE;
	} else {
		ud->special_holdoff = 5;
		return FALSE;
	}
}
#pragma mark Crouching moves
static void sub_312bc(Player *ply) {			// zang crouch punch little
	UD *ud = (UD*)&ply->UserData;
	quirkysound(0);
	ply->Move = 0;
	ud->x0088.part.p1 = 1;
	ud->distance = 0;
	ud->closemove = 0;
	ud->farmove = 9;
	ud->volley_joymask = 0;
}
static void sub_312ea(Player *ply) {			//zang crouch punch little
	UD *ud = (UD*)&ply->UserData;
	if (ply->OppXDist < 40) {
		if (zang_setupthrow(ply)) {
			ply->Move = 3;
			return;
		}
	}
	if (ply->OppXDist < 0x38) {
		if (zang_setupthrow(ply)) {
			ud->grip_select = 2;
			ply->Move = 5;
			return;
		}
	}
	quirkysound(1);
	ply->Move = 1;
}
static void sub_31360(Player *ply) {			// zang crouch punch big
	UD *ud = (UD*)&ply->UserData;
	if (ply->OppXDist < 40) {
		if (zang_setupthrow(ply)) {
			ply->Move = 4;
			return;
		}
	}
	if (ply->OppXDist < 0x38) {
		if (zang_setupthrow(ply)) {
			ud->grip_select = 2;
			ply->Move = 6;
			return;
		}
	}
	quirkysound(2);
	ply->Move = 2;
}
int PLCBCrouchZangeif(Player *ply) {			// 30f7a
	UD *ud = (UD*)&ply->UserData;
	if (sub_30f9e(ply)) {
		ud->x0088.part.p1 = 0;
		++g.GPCollX;
		ply->StandSquat = PLY_CROUCH;
		switch (ply->PunchKick) {
			case PLY_PUNCHING:
				switch (ply->ButtonStrength) {
					case 0:	sub_312bc(ply); break;
					case 2:	sub_312ea(ply); break;
					case 4: sub_31360(ply); break;
					FATALDEFAULT;
				}
				return TRUE;
				break;
			case PLY_KICKING:
				quirkysound(ply->ButtonStrength / 2);
				ud->x0088.part.p1 = 0;
				ply->Move = ply->ButtonStrength / 2;
				if (ply->Move == 0) {
					ud->x0088.part.p1 = 1;
					ud->distance = 0;
					ud->closemove = 0;
					ud->farmove = 0;
					ud->volley_joymask = BUTTON_A;
				}
				return TRUE;
				break;
			FATALDEFAULT;
		}
		return 1;
	} else {
		ud->special_holdoff = 5;
		return 0;
	}
}
#pragma mark Jumping moves
static void sub_3145c(Player *ply) {		// zang jump punch little
	quirkysound(0);
	ply->Move = (ply->VelX.full == 0 ? 0 : 7);
}
static void sub_31476(Player *ply) {
	if (ply->VelX.full == 0) {
		if (ply->JoyDecode.full & 8) {
			ply->Move = 4;
		} else {
			quirkysound(2);
			ply->Move = 3;
		}
	} else {
		if (ply->JoyDecode.full & 6) {
			ply->Move = 6;
		} else {
			quirkysound(2);
			ply->Move = 5;	
		}
	}
}
static void sub_314a4(Player *ply) {
	if (ply->VelX.full == 0) {
		if (ply->JoyDecode.full & 8) {
			ply->Move = 2;
		} else {
			quirkysound(1);
			ply->Move = 8;
		}
	} else {
		quirkysound(1);
		ply->Move = 1;
	}	
}
static void sub_31500(Player *ply) {
	quirkysound(0);
	ply->Move = (ply->VelX.full == 0 ? 0 : 3);
}
static void sub_3151a(Player *ply) {
	quirkysound(1);
	ply->Move = (ply->VelX.full == 0 ? 1 : 4);
}	
static void sub_31534(Player *ply) {
	quirkysound(2);
	ply->Move = (ply->VelX.full == 0 ? 2 : 5);
}	
int PLCBJumpZangeif(Player *ply) {			// 30f8c
	UD *ud = (UD*)&ply->UserData;
	if (sub_30f9e(ply)) {
		++g.GPCollX;
		ply->StandSquat = 4;
		switch (ply->PunchKick) {
			case PLY_PUNCHING:
				ud->x0088.part.p1 = 0;
				switch (ply->ButtonStrength) {
					case 0:	sub_3145c(ply); break;
					case 2:	sub_31476(ply); break;
					case 4: sub_314a4(ply); break;
						FATALDEFAULT;
				}
				CASetAnim2(ply, STATUS_JUMP_PUNCH, ply->Move);
				return TRUE;
				break;
			case PLY_KICKING:
				switch (ply->ButtonStrength) {
					case 0: sub_31500(ply); break;
					case 2:	sub_3151a(ply); break;
					case 4: sub_31534(ply); break;
					FATALDEFAULT;
				}
				CASetAnim2(ply, STATUS_JUMP_KICK, ply->Move);
				return TRUE;
				break;
			FATALDEFAULT;
		}
		return 1;
	} else {
		ud->special_holdoff = 5;
		return 0;
	}	
}
#pragma mark Power checkers
inline static void sub_31f18(Player *ply) {
	UD *ud = (UD*)&ply->UserData;
	
	ud->power_potential = (char[]){
		8,8,9,8,8,10,13,8,9,9,8,8,11,8,10,14,
		8,12,10,8,8,12,11,11,9,8,8,8,8,10,8,15
	}[RAND32];
}


static void sub_31c84(Player *ply, int d1) {
	UD *ud = (UD*)&ply->UserData;
	NEXT(ud->mode_power);
	ud->power_sel = d1;
	sub_31f18(ply);
}

int PLCBPowerZangeif(Player *ply) {			// 31c40
	UD *ud = (UD*)&ply->UserData;
	const static char data_31cd0[] = {
		8, 4, 8, 4, 1, 2, 1, 2
	};
	const static char data_31cfc[] = {
		2, 2, 1, 1, 4, 4, 8, 8,
	};
	const static char data_31d2e[] = {
		4, 8, 4, 8, 2, 1, 2, 1
	};
	switch (ud->mode_power) {
		case 0:
			if (ply->JoyDecode.full & JOY_MOVEMASK == 0) {
				ud->mode_power = 0;
				return 0;
			} else {
				switch (ply->JoyDecode.part.p1) {
					case 1: sub_31c84(ply, 0); break;
					case 2: sub_31c84(ply, 2); break;
					case 8: sub_31c84(ply, 4); break;
					case 4: sub_31c84(ply, 6); break;
					default:
						return 0;
						break;
				}
			}
			break;
		case 2:
			if (--ud->power_potential == 0) {
				ud->mode_power = 0;
				return 0;
			}
			if (ply->JoyDecode.full & 0xf) {
				if ((ply->JoyDecode.full & JOY_MOVEMASK) == data_31cd0[ud->power_sel]) {
					NEXT(ud->mode_power);
					ud->power_sel = 0;
					sub_31f18(ply);
				} else if ((ply->JoyDecode.full & JOY_MOVEMASK) == data_31cd0[ud->power_sel+1]) {
					NEXT(ud->mode_power);
					ud->power_sel = 1;
					sub_31f18(ply);
				}
			}
			break;
		case 4:
			if (--ud->power_potential == 0) {
				ud ->mode_power = 0;
				return 0;
			}
			if (ply->JoyDecode.full & JOY_MOVEMASK) {
				if ((ply->JoyDecode.full & JOY_MOVEMASK) == data_31cfc[ud->power_sel]) {
					NEXT(ud->mode_power);
				}
			}
			break;
		case 6:
			if (--ud->power_potential == 0) {
				ud ->mode_power = 0;
				return 0;
			}
			if (ply->JoyDecode.full & JOY_MOVEMASK) {
				if ((ply->JoyDecode.full & JOY_MOVEMASK) == data_31d2e[ud->power_sel]) {
					NEXT(ud->mode_power);
				}
			}
			break;
		case 8:
			if (PLY_NEWBUTTONS & BUTTON_PUNCHES) {
				return sub_31d56(ply, PLY_NEWBUTTONS & BUTTON_PUNCHES);
			} else if (~ply->JoyDecode.full & ply->JoyDecodeDash.full & BUTTON_PUNCHES) {
				return sub_31d56(ply, PLY_NEWBUTTONS & BUTTON_PUNCHES);
			} else {
				++ud->power_potential; 
				if (ud->power_potential >= 0xe) {
					ud->mode_power = 0;
				}
				return 0;
			}
			break;
		FATALDEFAULT;
	}
	return 0;
}
static void sub_31654(Player *ply) {		// lariat punch
	if (ply->mode2 == 0) {
		NEXT(ply->mode2);
		ply->LocalTimer = 1 * TICKS_PER_SECOND;
		CASetAnim2(ply, STATUS_PUNCH, ply->Move);
		BumpDiff_PowerMove();
	} else {
		if (--ply->LocalTimer) {
			if ((g.libsplatter & 0xf) == 0) {
				soundsting(SOUND_SWOOSH3);
			}
			PLAYERTICK;
		} else {
			ply_exit_stand(ply);
		}

	}
}
static void sub_31696(Player *ply) {
	UD *ud = (UD*)&ply->UserData;
	const static short data_316f4[] = {0xb4, 0x96, 0xb4, 0xb4, };
	const static char data_316fc[] = { 1, 2, 1, 2, };
	const static char data_31700[] = {0xab, 0xd6, 0xb1, 0xb1};
	const static char data_31704[] = {0x55, 0x52, 0x48, 0x48};
	const static char data_31708[] = {0x46, 0x2c, 0x47, 0x47};
	
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			random_damage_adjust_2(ply, 0x23);
			random_damage_adjust_1(ply, 12, 5);
			ud->x0088.full = data_316f4[ud->grip_select];
			CASetAnim2(ply, (ply->StandSquat ? STATUS_CROUCH_PUNCH : STATUS_PUNCH), ply->Move);
			break;
		case 2:
			if (--ud->x0088.full == 0) {
				ply_grip_release(ply, ply->Flip);
				sub_3163e(ply);
			} else if(ply_opp_has_struggled_free(ply)) {
				ply_grip_release(ply, ply->Flip);
				sub_3163e(ply);				
			} else {
				if (ply_opp_has_struggled_2(ply)) {
					ply->Timer = 1;
				}
				if (AF2 == 0) {
					PLAYERTICK;
				} else {
					ply->AnimFlags &= 0xff00;
					if (ud->grip_select == 1) {
						ActBlankaBiteBlood(ply);
					}
					if(ply_opp_apply_grip_damage(ply, 0, 
								data_316fc[ud->grip_select], 
								data_31700[ud->grip_select],
								data_31704[ud->grip_select], 
								data_31708[ud->grip_select]
								)) {
						sub_3163e(ply);
					} else {
						PLAYERTICK;
					}
				}
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_31780(Player *ply) {
	UD *ud = (UD*)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CASetAnim2(ply, STATUS_PUNCH, ply->Move);
			soundsting(SOUND_GINK);
			break;
		case 2:
			PLAYERTICK;
			if (AF2) {
				NEXT(ply->mode2);
				ud->x0086 = 0;
				ply->VelX.full = ((ply->JoyDecode.full & 1) ? 0x0200 :-0x200);
				ply->VelY.full = 0x0600;
				ply->AclX.full = 0;
				ply->AclY.full = 0x0048;
			}
			break;
		case 4:
			if (ud->x0086 == 0) {
				PLAYERTICK;
				if (AF2 == 2) {
					ud->x0086 = 1;
				}
			}
			CATrajectory((Object *)ply);
			if (ply->VelY.full < 0) {
				if (PLAYERGROUND) {
					NEXT(ply->mode2);
					ply->Timer2          = 12;
					ply->Opponent->Timer = 12;
					if (ud->x0086 == 0) {
						ply->Timer = 1;
						PLAYERTICK;
					}
					ply->EnemyDirection = ply->Flip;
					sub_36d6(ply, (ply->Flip ? 16 : -16), 0, 2, ply->Flip, 13, 0x2e, 2);
					ActStartScreenWobble();
				}
			}
			break;
		case 6:
			NEXT(ply->mode2);
			ply->Timer = 1;
			PLAYERTICK;
			ply->VelX.full = (ply->Flip ? -0x0280 : 0x0280);
			ply->VelY.full = 0x0280;
			ply->AclX.full = 0;
			ply->AclY.full = 0x0048;
			break;
		case 8:
			CATrajectory((Object *)ply);
			if (ply->VelY.full < 0) {
				if (PLAYERGROUND) {
					ply_exit_air(ply);
				}
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_318b0(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->Flip = ((ply->JoyDecode.full & 1) ? 0 : 1);
			CASetAnim2(ply, STATUS_PUNCH, ply->Move);
			soundsting(SOUND_HUA);
			break;
		case 2:
			if (AF2) {
				NEXT(ply->mode2);
				ply->Timer2           = 12;
				ply->Opponent->Timer2 = 12;
				ply->EnemyDirection   = ply->Flip;
				sub_36d6(ply, (ply->Flip ? -0x77 : 0x77), 0, 0, ply->Flip, 13, 0x2e, 2);
				ply->LocalTimer = 20;
				ActStartScreenWobble();
			} else {
				PLAYERTICK;
			}
			break;
		case 4:
			if (--ply->LocalTimer == 0) {
				NEXT(ply->mode2);
				ply->Timer = 1;
				PLAYERTICK;
			}
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
}
static void sub_3198c(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->Flip = ((ply->JoyDecode.full & 2) ? 0 : 1);
			CASetAnim2(ply, STATUS_KICK, ply->Move);
			soundsting(SOUND_HUA);
			break;
		case 2:
			if (AF2) {
				NEXT(ply->mode2);
				ply->Timer2           = 12;
				ply->Opponent->Timer2 = 12;
				ply->EnemyDirection   = ply->Flip;
				sub_36d6(ply, (ply->Flip ? 0x64 : -0x64), 0, 2, ply->Flip, 13, 0x2e, 2);
				ply->LocalTimer = 20;
				ActStartScreenWobble();
			} else {
				PLAYERTICK;
			}
			break;
		case 4:
			if (--ply->LocalTimer == 0) {
				NEXT(ply->mode2);
				ply->Timer = 1;
				PLAYERTICK;
			}
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
}
static void sub_31a46(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->Flip = ((ply->JoyDecode.full & 2) ? 0 : 1);
			CASetAnim2(ply, STATUS_KICK, ply->Move);
			soundsting(SOUND_HUA);
			break;
		case 2:
			if (AF2) {
				NEXT(ply->mode2);
				ply->Timer2           = 12;
				ply->Opponent->Timer2 = 12;
				ply->EnemyDirection   = ply->Flip;
				sub_36d6(ply, (ply->Flip ? 0x60 : -0x60), 0, 2, ply->Flip, 13, 0x2e, 2);
				ply->LocalTimer = 20;
				ActStartScreenWobble();
			} else {
				PLAYERTICK;
			}
			break;
		case 4:
			if (--ply->LocalTimer == 0) {
				NEXT(ply->mode2);
				ply->Timer = 1;
				PLAYERTICK;
			}
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
}
static void sub_31b2c(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->Flip = ((ply->JoyDecode.full & 2) ? 0 : 1);
			CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
			soundsting(SOUND_GINK);
			break;
		case 2:
			PLAYERTICK;
			if (AF2) {
				NEXT(ply->mode2);
			}
			break;
		case 4:
			NEXT(ply->mode2);
			ply->LocalTimer = 20;
			set_throw_trajectory(ply, 4, ply->Flip ^ 1, 12);
			ActStartScreenWobble();
			break;
		case 6:
			if (--ply->LocalTimer == 0) {
				ply->Flip ^= 1;
				ply->EnemyDirection = ply->Flip;
				ply_exit_stand(ply);
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_31bbc(Player *ply) {
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
			soundsting(SOUND_GINK);
			break;
		case 2:
			if (AF1) {
				NEXT(ply->mode2);
			}
			PLAYERTICK;
			break;
		case 4:
			NEXT(ply->mode2);
			ply->LocalTimer = 20;
			set_throw_trajectory(ply, 6, ply->Flip ^ 1, 12);
			break;
		case 6:
			if (--ply->LocalTimer == 0) {
				ply->Flip ^= 1;
				ply->EnemyDirection = ply->Flip;
				ply_exit_crouch(ply);
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_31584(Player *ply) {
	switch (ply->Move) {
		case 0:
		case 1:
		case 2:
		case 3:
			sub_315a4(ply);
			break;
		case 4:
			sub_31780(ply);
			break;
		case 5:
		case 7:
			sub_31696(ply);
			break;
		case 6:
			sub_318b0(ply);
			break;
		case 8:
			sub_31654(ply);
			break;
		FATALDEFAULT;
	}	
}
static void sub_3196e(Player *ply) {
	switch (ply->Move) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			sub_315a4(ply);
			break;
		case 6:
			sub_3198c(ply);
			break;
		case 7:
			sub_31a46(ply);
			break;
		FATALDEFAULT;
	}
}
static void sub_31b10(Player *ply) {
	switch (ply->Move) {
		case 0:
		case 1:
		case 2:
			sub_315a4(ply);
			break;
		case 3:
			sub_31b2c(ply);
			break;
		case 4:
			sub_31bbc(ply);
			break;
		case 5:
		case 6:
			sub_31696(ply);
			break;
		FATALDEFAULT;
	}
}

void PSCBAttackZangeif(Player *ply) {		// 3154e
	UD *ud = (UD*)&ply->UserData;
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		if (ud->special_holdoff) {
			--ud->special_holdoff;
		}
		switch (ply->StandSquat) {
			case PLY_STAND:
				switch (ply->PunchKick) {
					case PLY_PUNCHING:	sub_31584(ply);	break;
					case PLY_KICKING:	sub_3196e(ply);	break;
					FATALDEFAULT;
				}
				break;
			case PLY_CROUCH:
				switch (ply->PunchKick) {
					case PLY_PUNCHING:	sub_31b10(ply);	break;
					case PLY_KICKING:	sub_315a4(ply);	break;
					FATALDEFAULT;
				}
				break;
			FATALDEFAULT;
		}
	}
}

void PSCBPowerZangeif(Player *ply) {		// 31dec
	UD *ud = (UD*)&ply->UserData;
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		switch (ply->mode2) {
			case 0:
				NEXT(ply->mode2);
				CASetAnim2(ply, 0x4c, 0);
				soundsting(SOUND_HUA);
				break;
			case 2:
				if (AF2) {
					NEXT(ply->mode2);
					ply->Jumping = 1;
					ply->VelX.full = (ud->x0087 ? -0x0100 : 0x0100);
					ply->AclX.full = 0x0000;
					ply->VelY.full = 0x0800;
					ply->AclY.full = 0x0048;
				}
				PLAYERTICK;
				break;
			case 4:
				CATrajectory((Object *)ply);
				if (ply->VelY.full < 0) {
					if (PLAYERGROUND) {
						NEXT(ply->mode2);
						CASetAnim2(ply, 0x4c, 1);
					} else {
						if (AF2) {
							ply->AnimFlags &= 0xff00;
							soundsting(SOUND_PUNCH3);
						}
						PLAYERTICK;
					}
				} else {
					PLAYERTICK;
				}
				break;
			case 6:
				ply->Timer2 = 12;
				ply->Opponent->Timer2 = 12;
				sub_36d6(ply, 0, 0, 2, ply->Flip, 16, 0x2e, ud->x0084);
				ply->VelX.full = (ply->Flip ? -0x0280 : 0x0280);
				ply->VelY.full = 0x0300;
				ply->AclX.full = 0;
				ply->AclY.full = 0x0048;
				ActStartScreenWobble();
				break;
			case 8:
				NEXT(ply->mode2);
				ply->Timer2 = 1;
				PLAYERTICK;
				break;
			case 10:
				CATrajectory((Object *)ply);
				if (ply->VelY.full < 0) {
					if (PLAYERGROUND) {
						ply_exit_air(ply);
					}
				}
			default:
				break;
		}
	}
}
void PSCBVictoryZangeif(Player *ply) {		// 31f48
	int d2;
	UD *ud = (UD*)&ply->UserData;
	Object *obj;
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			g.FightOver = TRUE;
			ply->PSFinishedParticipating = TRUE;
			if (g.OnBonusStage) {
				d2 = 0x5555;
			} else if (ply->RoundsWon == 1) {
				d2 = 0x1044;
			} else {
				d2 = 0xefbb;
			}
			if (d2 & (1 << RAND16)) {		// yes, RAND16
				CASetAnim2(ply, 0x4e, 0);
			} else {
				CASetAnim2(ply, 0x4e, 1);
			}
			break;
		case 2:
			if ((ply->AnimFlags & 0xff00) && ply->RoundsWon == 2 && g.OnFinalStage
				&& g.NotUsed == 0) {
				ply->LocalTimer = 0x2d;
				NEXT(ply->mode3);
			} else {
				if (AF2) {
					soundsting(AF2);
				}
				PLAYERTICK;
			}
			break;
		case 4:
			if (g.Pause_9e1 == 2) {
				if (ply->LocalTimer) {
					--ply->LocalTimer;
				} else {
					switch (ud->x0094) {
						case 0:
							NEXT(ud->x0094);
							if (obj = AllocActor()) {
								obj->exists = TRUE;
								obj->Sel    = 0x25;
								obj->SubSel = 0xf;
								ply->Owner = (Player *)obj;
								if(ply->XPI - gstate_Scroll2.XPI >= 0x90) {
									obj->XPI = ply->XPI -0x80;
									obj->Flip = 1;
								} else {
									obj->XPI = ply->XPI + 0x80;
									obj->Flip = 0;
								}
								obj->YPI = 0xf0;
							}
							break;
						case 2:
							if (ply->Owner->Owner->UserData[0x10]) { //XXX
								NEXT(ud->x0094);
								ply->Flip = ply->Owner->Owner->Flip ^ 1;
								// XXX setaction_direct(ply, actlist_32074);
							}
							break;
						case 4:
							PLAYERTICK;
							
						FATALDEFAULT;
					}
				}

			}
			break;
		default:
			break;
	}
}

/////////////////////
#pragma mark COMPUTER 
/////////////////////


int PLCBCompJumpZangeif (Player *ply) {		// 3500e
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->ButtonStrength) {
				case 0:
					CASetAnim2(ply, STATUS_JUMP_PUNCH, (ply->VelX.full ? 7 : 0));
					quirkysound(0);
					break;
				case 2:
					if (ply->CompDoAirThrow) {
						CASetAnim2(ply, STATUS_JUMP_PUNCH, 8);
					} else {
						CASetAnim2(ply, STATUS_JUMP_PUNCH, (ply->VelX.full ? 2 : 1));						
					}
					quirkysound(1);
					break;
				case 4:
					if (ply->CompDoAirThrow) {
						CASetAnim2(ply, STATUS_JUMP_PUNCH, (ply->VelX.full ? 6 : 4));
					} else {
						CASetAnim2(ply, STATUS_JUMP_PUNCH, (ply->VelX.full ? 5 : 3));
					}
					quirkysound(2);
					break;
				FATALDEFAULT;
			}
			break;
		case PLY_KICKING:
			switch (ply->ButtonStrength) {
				case 0:
					CASetAnim2(ply, STATUS_JUMP_KICK, (ply->VelX.full ? 3 : 0));
					quirkysound(0);
					break;
				case 2:
					CASetAnim2(ply, STATUS_JUMP_KICK, (ply->VelX.full ? 4 : 1));
					quirkysound(1);
					break;
				case 4:
					CASetAnim2(ply, STATUS_JUMP_KICK, (ply->VelX.full ? 5 : 2));
					quirkysound(2);
					break;
					FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
	return 0;
}
static void sub_35af8(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley    = FALSE;
	exit_comp_normal(ply);
}
static void sub_35b06(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley    = FALSE;
	exit_to_compdisp1(ply);
}
static void sub_35b14(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley    = FALSE;
	comp_setjumping_main(ply);
}
static void sub_35bfa(Player *ply) {
	if (AF2) {
		ply->AnimFlags = 0;
		soundsting(SOUND_PUNCH3);		
	}
}
static void sub_35aee(Player *ply) {
	if (ply->ActionScript->Crouch) {
		sub_35b06(ply);
	} else {
		sub_35af8(ply);
	}
}

static void sub_35b22(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	
	if (ply->PunchKick == PLY_PUNCHING) {
		if (ply->mode2 == 0) {
			NEXT(ply->mode2);
			ply->LocalTimer = 1 * TICKS_PER_SECOND;
			ply->Move = 8;
			CASetAnim2(ply, STATUS_PUNCH, ply->Move);
			BumpDiff_PowerMove();
			soundsting(SOUND_HUA);
		} else {
			if (--ply->LocalTimer == 0) {
				sub_35af8(ply);
			} else {
				if ((g.libsplatter & 0xf) == 0) {
					soundsting(SOUND_SWOOSH3);
				}
				PLAYERTICK;
			}
		}
	} else {
		switch (ply->mode2) {
			case 0:
				ud->x0084 = ply->ButtonStrength / 2;
				if (zang_setupthrow(ply) == 0) {
					sub_35aee(ply);
				} else {
					NEXT(ply->mode2);
					CASetAnim1(ply, 0x4c);
					soundsting(SOUND_HUA);
				}
				break;
			case 2:
				if (AF2) {
					NEXT(ply->mode2);
					ply->Airborne = 1;
					PLY_TRAJ0(0x0100, 0x0000, 0x0800, 0x0048);
					if (ud->x0087) {
						ply->VelX.full = -ply->VelX.full;
					}
					sub_35bfa(ply);
				} else {
					PLAYERTICK;
				}
				break;
			case 4:
				CATrajectory((Object *)ply);
				if (ply->VelY.full > 0) {
					sub_35bfa(ply);
				} else if (PLAYERGROUND == 0) {
					sub_35bfa(ply);
				} else {
					NEXT(ply->mode2);
					CASetAnim2(ply, 0x4c, 1);
				}
				break;
			case 6:
				NEXT(ply->mode2);
				ply->Timer2 = 12;
				ply->Opponent->Timer2 = 12;
				sub_36d6(ply, 0, 0, 2, ply->Flip, 16, 0x2e, ud->x0084);
				PLY_TRAJ0(0x0280, 0x0300, 0x0000, 0x0048);
				if (ply->Flip) {
					ply->VelX.full = -ply->VelX.full;
				}
				ActStartScreenWobble();
				break;
			case 8:
				NEXT(ply->mode2);
				ply->Timer = 1;
				PLAYERTICK;
				break;
			case 10:
				CATrajectory((Object *)ply);
				if (ply->VelY.full < 0) {
					if (PLAYERGROUND) {
						sub_35b14(ply);
					}
				}
				break;
			FATALDEFAULT;
		}
	}
}
static void sub_35ab6(Player *ply) {
	CASetAnim2(ply, 
			   (short []){STATUS_PUNCH, STATUS_KICK, 0x44,0x46}[(ply->PunchKick/2) + ply->StandSquat], 
			   ply->Move);
}
static void sub_35ad6(Player *ply)  { 
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	if (ply->OppXDist < ud->comp_distance) {
		ply->Move = ud->comp_closemove;		
	} else {
		ply->Move = ud->comp_farmove;
	}
	sub_35ab6(ply);
}
static void sub_35a7a(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	if (AF1) {
		sub_35aee(ply);
	} else if (AF2 == 0 || ud->x008b == 0) {
		PLAYERTICK;
	} else if (comp_diceroll(ply) == 0) {
		PLAYERTICK;
	} else if (comp_setnextaction(ply)) {
		comp_do_next_action(ply);
	} else {
		quirkysound(ply->ButtonStrength/2);
		sub_35ad6(ply);
	}
}
static void sub_3513e(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ud->comp_closemove         = 0;
			ud->comp_farmove         = 1;
			ud->x008b         = 1;
			ud->comp_distance         = 40;
			quirkysound(0);
			sub_35ad6(ply);
			break;
		case 2:
			sub_35a7a(ply);
			break;
		FATALDEFAULT;
	}
}
static void sub_35240(Player *ply) {
	const static short data_3527a[] = {0xb4, 0x96, 0xb4, 0xb4, };	// 316f4  
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	NEXT(ply->mode2);
	random_damage_adjust_2(ply, 0x23);
	random_damage_adjust_1(ply, 12, 5);
	ud->x0088 = data_3527a[ud->x008e];
}
static void sub_3529a(Player *ply) {
	const static char data_3528a[] = { 1, 2, 1, 2, };				// 316fc   
	const static char data_3528e[] = {0xab, 0xd6, 0xb1, 0xb1};		// 31700   
	const static char data_35292[] = {0x55, 0x52, 0x48, 0x48};		// 31704   
	const static char data_35296[] = {0x46, 0x2c, 0x47, 0x47};		// 31708   
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	if (--ud->x0088 == 0) {
		ply_grip_release(ply, ply->Flip);
	} else {
		if (ply_opp_has_struggled_free(ply)) {
			ply_grip_release(ply, ply->Flip);
		} else {
			if (ply_opp_has_struggled_2(ply)) {
				ply->Timer = 1;
			}
			if (AF2) {
				ply->AnimFlags &= 0xff00;
				if (ud->x008e == 1) {
					ActBlankaBiteBlood(ply);
				}
				if(ply_opp_apply_grip_damage(ply, 0, data_3528a[ud->x008e], data_3528e[ud->x008e],
							data_35292[ud->x008e], data_35296[ud->x008e])) {
					
					sub_35aee(ply);
					return;
				}
			}
			PLAYERTICK;
		}
	}	
}
static void sub_35174(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	
	switch (ply->mode2) {
		case 0:
			if (ply->CompDoThrow) {
				if (ply->OppXDist <= 40) {
					if (zang_setupthrow(ply)) {
						ply->mode2 = 8;
						ply->Move = 4;
						CASetAnim2(ply, STATUS_PUNCH, ply->Move);
						soundsting(SOUND_GINK);
						return;
					}
				} else if (ply->OppXDist <= 0x38) {
					if (zang_setupthrow(ply)) {
						ply->mode2 = 4;
						ud->x008e = 0;
						ply->Move = 5;
						CASetAnim2(ply, STATUS_PUNCH, ply->Move);
						return;
					}
				}
			}
			NEXT(ply->mode2);
			ud->comp_closemove = 2;
			ud->comp_farmove = 2;
			ud->x008b = 0;
			ud->comp_distance = 0;
			quirkysound(1);
			sub_35ad6(ply);
			break;
		case 2:
			sub_35a7a(ply);
			break;
		case 4:				// GRIPS  35240
			sub_35240(ply);
			break;
		case 6:
			sub_3529a(ply);
			break;
		case 8:
			PLAYERTICK;
			if (AF2) {
				NEXT(ply->mode2);
				ud->x0086 = 0;
				PLY_TRAJ0(0x0200, 0x0600, 0x0, 0x0048);
				if (ply->IsWithinBounds == 0) {
					ply->VelX.full = -ply->VelX.full;
				}
			}
			break;
		case 10:
			if (ud->x0086 == 0) {
				PLAYERTICK;
				if (AF2 == 2) {
					ud->x0086 = 1;
				}
			}
			CATrajectory((Object *)ply);
			if (ply->VelY.full < 0) {
				if (PLAYERGROUND) {
					NEXT(ply->mode2);
					ply->Timer2 = 12;
					ply->Opponent->Timer2 = 12;
					if (ud->x0086 == 0) {
						ply->Timer = 1;
						PLAYERTICK;
					}
					ply->EnemyDirection = ply->Flip;
					sub_36d6(ply, (ply->Flip ? 16 : -16), 0, 2, ply->Flip, 13, 0x2e, 2);
					ActStartScreenWobble();
				}
			}
			break;
		case 12:
			NEXT(ply->mode1);
			ply->Timer = 1;
			PLAYERTICK;
			PLY_TRAJ0(0x0280, 0x0280, 0x0, 0x0048);
			if (ply->Flip) {
				ply->VelX.full = -ply->VelX.full;
			}
			break;
		case 14:
			CATrajectory((Object *)ply);
			if (ply->VelY.full < 0) {
				if (PLAYERGROUND) {
					sub_35b14(ply);
				}
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_35400(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;

	switch (ply->mode2) {
		case 0:
			if (ply->CompDoThrow) {
				if (ply->OppXDist <= 40) {
					if (zang_setupthrow(ply)) {
						ply->mode2 = 8;
						ply->Flip = ply->IsWithinBounds ^ 1;
						ply->Move = 6;
						CASetAnim2(ply, STATUS_PUNCH, ply->Move);
						soundsting(SOUND_HUA);
						return;
					}
				} else if (ply->OppXDist <= 0x38) {
					if (zang_setupthrow(ply)) {
						ply->mode2 = 4;
						ud->x008e = 1;
						ply->Move = 5;
						CASetAnim2(ply, STATUS_PUNCH, ply->Move);
						return;
					}
				}
			}
			NEXT(ply->mode2);
			ud->comp_closemove = 3;
			ud->comp_farmove = 3;
			ud->x008b = 0;
			ud->comp_distance = 0;
			quirkysound(2);
			sub_35ad6(ply);
			break;
		case 2:
			sub_35a7a(ply);
			break;
		case 4:
			sub_35240(ply);
			break;
		case 6:
			sub_3529a(ply);
			break;
		case 8:
			if (AF2) {
				NEXT(ply->mode2);
				ply->Timer2 = 12;
				ply->Opponent->Timer2 = 12;
				ply->EnemyDirection = ply->Flip;
				sub_36d6(ply, (ply->Flip ? -0x77 : 0x77), 0, 
						 0, ply->Flip ^ 1, 13, 0x2e, 2);
				ply->LocalTimer = 20;
				ActStartScreenWobble();
			} else {
				PLAYERTICK;
			}
			break;
		case 10:
			if (--ply->LocalTimer == 0) {
				NEXT(ply->mode2);
				ply->Timer = 1;
				PLAYERTICK;
			}
			break;
		case 12:
			if (AF1) {
				sub_35af8(ply);
			} else {
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_35566(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ud->comp_closemove = 0;
			ud->comp_farmove = 1;
			ud->x008b = 0;
			ud->comp_distance = 32;
			quirkysound(0);
			sub_35ad6(ply);			
			break;
		case 2:
			sub_35a7a(ply);
			break;
		FATALDEFAULT;
	}
}
static void sub_3559c(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			if (ply->CompDoThrow) {
				if (ply->OppXDist <= 48) {
					if (zang_setupthrow(ply)) {
						ply->mode2 = 4;
						ply->Flip = ply->IsWithinBounds;
						ply->Move = 6;
						CASetAnim2(ply, STATUS_KICK, ply->Move);
						soundsting(SOUND_HUA);
						return;
					}
				}
			}
			NEXT(ply->mode2);
			ud->comp_closemove = 2;
			ud->comp_farmove = 3;
			ud->x008b = 0;
			ud->comp_distance = 0x38;
			quirkysound(1);
			sub_35ad6(ply);			
			break;
		case 2:
			sub_35a7a(ply);
			break;
		case 4:
			if (AF2) {
				NEXT(ply->mode2);
				ply->Timer2 = 12;
				ply->Opponent->Timer2 = 12;
				ply->EnemyDirection = ply->Flip;
				sub_36d6(ply, (ply->Flip ? 0x64 : -0x64), 0, 2, ply->Flip, 13, 0x2e, 2);
				ply->LocalTimer = 20;
				ActStartScreenWobble();
			} else {
				PLAYERTICK;
			}
			break;
		case 6:
			if (--ply->LocalTimer == 0) {
				NEXT(ply->mode2);
				ply->Timer = 1;
				PLAYERTICK;
			}
			break;
		case 8:
			if (AF1) {
				sub_35af8(ply);
			} else {
				PLAYERTICK;
			}			
			break;
		FATALDEFAULT;
	}
}
static void sub_356a0(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			if (ply->CompDoThrow) {
				if (ply->OppXDist <= 48) {
					if (zang_setupthrow(ply)) {
						ply->mode2 = 4;
						ply->Flip = ply->IsWithinBounds;
						ply->Move = 7;
						CASetAnim2(ply, STATUS_KICK, ply->Move);
						soundsting(SOUND_HUA);
						return;
					}
				}
			}
			NEXT(ply->mode2);
			ud->comp_closemove = 5;
			ud->comp_farmove = 4;
			ud->x008b = 0;
			ud->comp_distance = 0x26;
			quirkysound(2);
			sub_35ad6(ply);			
			break;
		case 2:
			sub_35a7a(ply);
			break;
		case 4:
			if (AF2) {
				NEXT(ply->mode2);
				ply->Timer2 = 12;
				ply->Opponent->Timer2 = 12;
				ply->EnemyDirection = ply->Flip;
				sub_36d6(ply, (ply->Flip ? -0x60 : 0x60), 0, 2, ply->Flip, 13, 0x2e, 2);
				ply->LocalTimer = 20;
				ActStartScreenWobble();
			} else {
				PLAYERTICK;
			}
			break;
		case 6:
			if (--ply->LocalTimer == 0) {
				NEXT(ply->mode2);
				ply->Timer = 1;
				PLAYERTICK;
			}
			break;
		case 8:
			if (AF1) {
				sub_35af8(ply);
			} else {
				PLAYERTICK;
			}			
			break;
			FATALDEFAULT;
	}
}
#pragma mark Computer Crouching
static void sub_357c6(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ud->comp_closemove = 0;
			ud->comp_farmove = 0;
			ud->x008b = 1;
			ud->comp_distance = 0;
			quirkysound(0);
			sub_35ad6(ply);						
			break;
		case 2:
			sub_35a7a(ply);
			break;
		default:
			break;
	}
}
static void sub_357fc(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			if (ply->CompDoThrow) {
				if (ply->OppXDist <= 40) {
					if (zang_setupthrow(ply)) {
						ply->mode2 = 4;
						ply->Move  = 3;
						ply->Flip  = ply->IsWithinBounds ^ 1;
						CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
						soundsting(SOUND_GINK);
						return;
					}
				} else if (ply->OppXDist <= 0x38) {
					if (zang_setupthrow(ply)) {
						ply->mode2 = 10;
						ud->x008e  = 2;
						ply->Move  = 5;
						CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
						return;
					}
				}
			}
			NEXT(ply->mode2);
			ud->comp_closemove = 1;
			ud->comp_farmove = 1;
			ud->x008b = 0;
			ud->comp_distance = 0;
			quirkysound(1);
			sub_35ad6(ply);
			break;
		case 2:
			sub_35a7a(ply);
			break;
		case 4:
			PLAYERTICK;
			if (AF2) {
				NEXT(ply->mode2);
			}
			break;
		case 6:
			NEXT(ply->mode2);
			ply->LocalTimer = 20;
			set_throw_trajectory(ply, 4, ply->Flip ^ 1, 12);
			ActStartScreenWobble();
			break;
		case 8:
			if (--ply->LocalTimer == 0) {
				ply->Flip ^= 1;
				ply->EnemyDirection = ply->Flip;
				sub_35af8(ply);
			}
			break;
		case 10:
			sub_35240(ply);
			break;
		case 12:
			sub_3529a(ply);
			break;
		FATALDEFAULT;
	}
}
static void sub_35922(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			if (ply->CompDoThrow) {
				if (ply->OppXDist <= 40) {
					if (zang_setupthrow(ply)) {
						ply->mode2 = 4;
						ply->Move  = 4;
						ply->Flip  = ply->IsWithinBounds ^ 1;
						CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
						soundsting(SOUND_GINK);
						return;
					}
				} else if (zang_setupthrow(ply)) {
						ply->mode2 = 10;
						ud->x008e  = 3;
						ply->Move  = 6;
						CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
						return;
					
				}
			}
			NEXT(ply->mode2);
			ud->comp_closemove = 2;
			ud->comp_farmove = 2;
			ud->x008b = 0;
			ud->comp_distance = 0;
			quirkysound(2);
			sub_35ad6(ply);
			break;
		case 2:
			sub_35a7a(ply);
			break;
		case 4:
			if (AF1) {
				NEXT(ply->mode2);
			}
			PLAYERTICK;
			break;
		case 6:
			NEXT(ply->mode2);
			ply->LocalTimer = 20;
			set_throw_trajectory(ply, 6, ply->Flip ^ 1, 12);
			break;
		case 8:
			if (--ply->LocalTimer == 0) {
				ply->EnemyDirection = ply->Flip ^ 1;
				sub_35b06(ply);
			}
			break;
		case 10:
			sub_35240(ply);
			break;
		case 12:
			sub_3529a(ply);
			break;
		FATALDEFAULT;
	}
}
static void sub_35a36(Player *ply) {
	UDCOMP *ud = (UDCOMP *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ud->comp_closemove = ply->ButtonStrength / 2;
			ud->comp_farmove = ply->ButtonStrength / 2;
			ud->x008b = (ply->ButtonStrength ? 1 : 0);
			ud->comp_distance = 0;
			quirkysound(ply->ButtonStrength/2);
			sub_35ad6(ply);
			break;
		case 2:
			sub_35a7a(ply);
			break;
		FATALDEFAULT;
	}
}


void PLCBCompAttackZangeif(Player *ply) {			// 350f8
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		if (ply->AISigSpecial) {
			sub_35b22(ply);
		} else {
			switch (ply->StandSquat) {
				case PLY_STAND: switch (ply->PunchKick) {
					case PLY_PUNCHING: switch (ply->ButtonStrength) {
						case 0:		sub_3513e(ply); break;
						case 2:		sub_35174(ply); break;
						case 4:		sub_35400(ply); break;
						FATALDEFAULT;}
						break;
					case PLY_KICKING: switch (ply->ButtonStrength) {
						case 0:		sub_35566(ply); break;
						case 2:		sub_3559c(ply); break;
						case 4:		sub_356a0(ply); break;
						FATALDEFAULT;}
						break;
					FATALDEFAULT;}
					break;
				case PLY_CROUCH: switch (ply->PunchKick) {
					case PLY_PUNCHING: switch (ply->ButtonStrength) {
						case 0:		sub_357c6(ply); break;
						case 2:		sub_357fc(ply); break;
						case 4:		sub_35922(ply); break;
						FATALDEFAULT;}
						break;
					case PLY_KICKING: 
						sub_35a36(ply);
						break;
					FATALDEFAULT;}
					break;
				FATALDEFAULT;
			}
		}
	}
}





