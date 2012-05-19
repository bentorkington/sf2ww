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

#include "computer.h"

#include "lib.h"
#include "gfxlib.h"


#include "zangeif.h"
#include "zangeifdata.h"

extern Game g;
extern GState gstate_Scroll2;
int g_zangeif_d5;

struct UserData_Zangeif {
	signed char x0080;
	signed char x0081;
	signed char	x0082;
	signed char	x0084;
	signed char	x0085;
	signed char	x0086;
	signed char	x0087;
	DUAL		x0088;
	short		x008a;		// Joymask for NextAction
	short		x008c;
	signed char	x008e;		// close move
	signed char x008f;		// far move
	short		x0092;	
	signed char	x0094;		//victory submode
};

typedef struct UserData_Zangeif UD;


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
	ply->Throw[0] = 0xffdb;
	ply->Throw[1] = 0x35;
	ply->Throw[2] = 0x25;
	ply->Throw[3] = 0x10;
	return throwvalid(ply);
}
inline static void sub_315b8(Player *ply) {
	CASetAnim2(ply, 
			   (short[]){0x40,0x42,0x44,0x46}[((ply->StandSquat * 2) + ply->PunchKick)/2], 
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
					   (~ply->JoyDecodeDash.full & ply->JoyDecode.full & ud->x008a) == 0) {
				PLAYERTICK;
			} else if (PSSetNextAction(ply)) {
				plstat_do_nextaction(ply);
			} else {
				quirkysound(ply->PunchKick / 2);
				++g.HumanMoveCnt;
				ply->Opponent->SufferHB5 = 0;
				if (ply->OppXDist >= ud->x008c) {
					ply->Move = ud->x008f;					
				} else {
					ply->Move = ud->x008e;
				}
				sub_315b8(ply);
			}
			break;
		FATALDEFAULT;
	}
}

#pragma mark Standing moves
static int sub_30f9e(Player *ply) {
	int buttons = ~ply->JoyDecodeDash.full & ply->JoyDecode.full & BUTTON_MASK;
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
	soundsting(0x47);
}
static void sub_31056(Player *ply) {		// zang punch little
	UD *ud = (UD*)&ply->UserData;
	quirkysound(0);
	ply->Move = (ply->OppXDist > 40 ? 1 : 0);
	ud->x0088.part.p1 = 1;
	ud->x008c = 1;
	ud->x008e = 0;
	ud->x008f = 1;
	ud->x008a = 0x10;
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
				ud->x0092 = 0;
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
				ud->x0092 = 1;
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
	if (check_special_ability(ply) || ud->x0085 == 0 || zang_setupthrow(ply)== 0) {
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
	if (buttons & 0x10) {
		return sub_31d92(ply, 0);
	} else if (buttons & 0x20 && ud->x0082 <= 10) {
		return sub_31d92(ply, 1);
	} else if (buttons & 0x40 && ud->x0082 <= 8) {
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
		ud->x0085 = 5;
		return FALSE;
	}
}
#pragma mark Crouching moves
static void sub_312bc(Player *ply) {			// zang crouch punch little
	UD *ud = (UD*)&ply->UserData;
	quirkysound(0);
	ply->Move = 0;
	ud->x0088.part.p1 = 1;
	ud->x008c = 0;
	ud->x008e = 0;
	ud->x008f = 9;
	ud->x008a = 0;
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
			ud->x0092 = 2;
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
			ud->x0092 = 2;
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
					ud->x008c = 0;
					ud->x008e = 0;
					ud->x008f = 0;
					ud->x008a = 0x10;
				}
				return TRUE;
				break;
			FATALDEFAULT;
		}
		return 1;
	} else {
		ud->x0085 = 5;
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
				CASetAnim2(ply, 0x48, ply->Move);
				return TRUE;
				break;
			case PLY_KICKING:
				switch (ply->ButtonStrength) {
					case 0: sub_31500(ply); break;
					case 2:	sub_3151a(ply); break;
					case 4: sub_31534(ply); break;
					FATALDEFAULT;
				}
				CASetAnim2(ply, 0x4a, ply->Move);
				return TRUE;
				break;
			FATALDEFAULT;
		}
		return 1;
	} else {
		ud->x0085 = 5;
		return 0;
	}	
}
#pragma mark Power checkers
inline static void sub_31f18(Player *ply) {
	UD *ud = (UD*)&ply->UserData;
	
	ud->x0082 = (char[]){
		8,8,9,8,8,10,13,8,9,9,8,8,11,8,10,14,
		8,12,10,8,8,12,11,11,9,8,8,8,8,10,8,15
	}[RAND32];
}


static void sub_31c84(Player *ply, int d1) {
	UD *ud = (UD*)&ply->UserData;
	NEXT(ud->x0080);
	ud->x0081 = d1;
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
	switch (ud->x0080) {
		case 0:
			if (ply->JoyDecode.full & 0xf == 0) {
				ud->x0080 = 0;
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
			if (--ud->x0082 == 0) {
				ud->x0080 = 0;
				return 0;
			}
			if (ply->JoyDecode.full & 0xf) {
				if ((ply->JoyDecode.full & 0xf) == data_31cd0[ud->x0081]) {
					NEXT(ud->x0080);
					ud->x0081 = 0;
					sub_31f18(ply);
				} else if ((ply->JoyDecode.full & 0xf) == data_31cd0[ud->x0081+1]) {
					NEXT(ud->x0080);
					ud->x0081 = 1;
					sub_31f18(ply);
				}
			}
			break;
		case 4:
			if (--ud->x0082 == 0) {
				ud ->x0080 = 0;
				return 0;
			}
			if (ply->JoyDecode.full & 0xf) {
				if ((ply->JoyDecode.full & 0xf) == data_31cfc[ud->x0081]) {
					NEXT(ud->x0080);
				}
			}
			break;
		case 6:
			if (--ud->x0082 == 0) {
				ud ->x0080 = 0;
				return 0;
			}
			if (ply->JoyDecode.full & 0xf) {
				if ((ply->JoyDecode.full & 0xf) == data_31d2e[ud->x0081]) {
					NEXT(ud->x0080);
				}
			}
			break;
		case 8:
			if (~ply->JoyDecodeDash.full & ply->JoyDecode.full & 0x70) {
				return sub_31d56(ply, ~ply->JoyDecodeDash.full & ply->JoyDecode.full & 0x70);
			} else if (~ply->JoyDecode.full & ply->JoyDecodeDash.full & 0x70) {
				return sub_31d56(ply, ~ply->JoyDecodeDash.full & ply->JoyDecode.full & 0x70);
			} else {
				++ud->x0082; 
				if (ud->x0082 >= 0xe) {
					ud->x0080 = 0;
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
		ply->LocalTimer = 60;
		CASetAnim2(ply, 0x40, ply->Move);
		BumpDiff_PowerMove();
	} else {
		if (--ply->LocalTimer) {
			if ((g.libsplatter & 0xf) == 0) {
				soundsting(0x3c);
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
			ud->x0088.full = data_316f4[ud->x0092];
			CASetAnim2(ply, (ply->StandSquat ? 0x44 : 0x40), ply->Move);
			break;
		case 2:
			if (--ud->x0088.full == 0) {
				ply_grip_release(ply, ply->Flip);
				sub_3163e(ply);
			} else if(ply_opp_has_struggled_free(ply)) {
				ply_grip_release(ply, ply->Flip);
				sub_3163e(ply);				
			} else {
				if (sub_3fee(ply)) {
					ply->Timer = 1;
				}
				if (AF2 == 0) {
					PLAYERTICK;
				} else {
					ply->AnimFlags &= 0xff00;
					if (ud->x0092 == 1) {
						ActBlankaBiteBlood(ply);
					}
					if(sub_3466(ply, 0, data_316fc[ud->x0092], data_31700[ud->x0092],
								data_31704[ud->x0092], data_31708[ud->x0092])) {
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
			CASetAnim2(ply, 0x40, ply->Move);
			soundsting(0x46);
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
				if (check_ground_collision(ply)) {
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
				if (check_ground_collision((Object *)ply)) {
					ply_exit_air(ply);
				}
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_318b0(Player *ply) {
	UD *ud = (UD*)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->Flip = ((ply->JoyDecode.full & 1) ? 0 : 1);
			CASetAnim2(ply, 0x40, ply->Move);
			soundsting(0x47);
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
			CASetAnim2(ply, 0x42, ply->Move);
			soundsting(0x47);
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
			CASetAnim2(ply, 0x42, ply->Move);
			soundsting(0x47);
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
			CASetAnim2(ply, 0x44, ply->Move);
			soundsting(0x46);
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
			CASetAnim2(ply, 0x44, ply->Move);
			soundsting(0x46);
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
		if (ud->x0085) {
			--ud->x0085;
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
				soundsting(0x47);
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
					if (check_ground_collision((Object *)ply)) {
						NEXT(ply->mode2);
						CASetAnim2(ply, 0x4c, 1);
					} else {
						if (AF2) {
							ply->AnimFlags &= 0xff00;
							soundsting(0x27);
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
					if (check_ground_collision((Object *)ply)) {
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
								obj->Sel = 0x25;
								obj->SubSel = 0xf;
								ply->Owner = obj;
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
								ply->Flip = obj->Owner->Owner->Flip ^ 1;
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







