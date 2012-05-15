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
//#include "zangeifdata.h"

extern Game g;

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
	//signed char x0089;
	short		x008c;
	signed char	x008e;
	signed char x008f;
	short		x008a;
	short		x0092;
	
};

typedef struct UserData_Zangeif UD;

static int zang_setupthrow(Player *ply) {
	ply->Throw[0] = 0xffdb;
	ply->Throw[1] = 0x35;
	ply->Throw[2] = 0x25;
	ply->Throw[3] = 0x10;
	return throwvalid(ply);
}

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
						//3102c
						if (g_zangeif_d5 & 0x70 == 0x70) {
							sub_31044(ply);
							return TRUE;
						}
					}
				}
				// 31032
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

