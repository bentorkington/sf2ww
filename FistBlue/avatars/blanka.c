/*
 *  blanka.c
 *  GLUTBasics
 *
 *  Created by Ben on 7/02/11.
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


#include "blanka.h"
#include "blankadata.h"

extern Game g;

typedef struct UserData_Blanka UD;

static int _IDFromButton(Player *ply);
static void sub_33ada(Player *ply, short sel );

void pl_cb_setstatus2_blanka(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_43b90[status / 2], argd0);
}
void pl_cb_setstatus3_blanka(Player *ply, short status) {
	pl_cb_setstatus2_blanka(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_blanka(Player *ply, short status) {
	pl_cb_setstatus2_blanka(ply, status, 0);
}


static void _BlankaCatchLU(Player *ply) {		//2e886
	UD *ud=(UD *)&ply->UserData;
	static const char data_939a0[]={
		0xcf, 0x00, 0x20, 0x04, 0xcf, 0x00, 0x20, 0x04, 0xcf, 0x00, 0x20, 0x04, 0xcf, 0x00, 0x20, 0x05, 
		0xcf, 0x00, 0x20, 0x04, };
		
	static const char data_939b4[] = {
			                    0xbd, 0x00, 0x20, 0x00, 0xca, 0xf6, 0x20, 0x00, 0xc8, 0xf8, 0x20, 0x00, 
		0xe9, 0x03, 0x20, 0x01, 0xe9, 0x03, 0x20, 0x02, };
		
	static const char data_939c8[] = { 0x00, 0x00, 0x00, 0x00, };	/* Blanka can't catch himself */
		
	static const char data_939cc[] = {
																				0xc0, 0x00, 0x20, 0x00, 
		0xc0, 0x00, 0x20, 0x00, 0xc0, 0x00, 0x20, 0x00, 0xc0, 0x00, 0x20, 0x05, 0xc0, 0x00, 0x20, 0x04, };
	static const char data_939e0[]={
		0xc5, 0x00, 0x20, 0x00, 0xc5, 0x00, 0x20, 0x00, 0xc5, 0x00, 0x20, 0x00, 0xd4, 0x03, 0x20, 0x05, 
		0xd4, 0x03, 0x20, 0x04, };
	static const char data_939f4[]={
								0xba, 0x00, 0x20, 0x04, 0xba, 0x00, 0x20, 0x04, 0xba, 0x00, 0x20, 0x04, 
		0xb8, 0xf1, 0x20, 0x06, 0xb8, 0xee, 0x20, 0x05, };
		
	static const char data_93a08[]={
														0xc5, 0x00, 0x20, 0x00, 0xbf, 0xf5, 0x21, 0x00, 
		0xbd, 0xf9, 0x20, 0x00, 0xcd, 0xef, 0x20, 0x00, 0xcd, 0xef, 0x20, 0x01,};
	static const char data_93a1c[]={
																				0xbd, 0x00, 0x20, 0x00, 
		0xbc, 0xf6, 0x20, 0x00, 0xb6, 0xf9, 0x20, 0x05, 0xc8, 0xf5, 0x20, 0x06, 0xc8, 0xf5, 0x20, 0x05, };
	static const char data_93a30[]={
		0xc7, 0x00, 0x20, 0x00, 0xca, 0xdf, 0x20, 0x00, 0xca, 0xe0, 0x20, 0x00, 0xdc, 0xe3, 0x20, 0x01, 
		0xdc, 0xe3, 0x20, 0x02, };
	static const char data_93a44[]={
								0xc9, 0x00, 0x20, 0x00, 0xc2, 0xe7, 0x21, 0x00, 0xc1, 0xed, 0x20, 0x00, 
		0xd9, 0xe5, 0x20, 0x01, 0xd9, 0xe5, 0x20, 0x02, };
	static const char data_93a58[]={
														0xc8, 0x00, 0x20, 0x03, 0xbf, 0xf3, 0x21, 0x03, 
		0xbf, 0xf3, 0x20, 0x03, 0xd8, 0xf4, 0x20, 0x04, 0xd5, 0xf4, 0x20, 0x03, 0xd4, 0x00, 0x20, 0x00, 
	/* probably ends here */
		0xf0, 0x00, 0x20, 0x02, 0xe0, 0x19, 0x20, 0x05, 0x16, 0x25, 0x00, 0x0f,  };
	/* next address 00093a7c */
	
	const static char *data_93488[12]={
		data_939a0, data_939b4, data_939c8, data_939cc, 
		data_939a0, data_939e0, data_939f4, data_93a08, 
		data_93a1c, data_93a30, data_93a44, data_93a58, 
	};
	/* next address 000934a0 */
	ud->x0096 = data_93488[ply->Opponent->FighterID];
}
static void _BlankaCatchSet(Player *ply) {		//2e8a6
	UD *ud=(UD *)&ply->UserData;
	short d1;
	const char *bc= &ud->x0096[ply->Opponent->ActionScript->Catch * 4];
	d1 = ply->Flip ? -bc[0] : bc[0];
	d1 -= ply->Opponent->XPI;
	ply->XPI = -d1;
	d1 = bc[1] - ply->Opponent->YPI;
	ply->YPI = -d1;
}

static void _BlankaStartMoveAnim(Player *ply, short sel_d0) {		// 2ef90
	UD *ud=(UD *)&ply->UserData;
	ud->x008a = 0;
	ud->x0094 = 0;
	ply->Move = sel_d0;
	sub_33ada(ply, ply->Move);		// copy of 2ef56
	g.HumanMoveCnt++;
	// returns 1
}
static u16  _ButtonMask(Player *ply, u16 d1) {		//2ef3c
	UD *ud=(UD *)&ply->UserData;

	if (ud->x0086) {
		ud->x0086 = 0;
		return (ud->x0084 & d1);
	}
	return (ud->x0082 & d1);	
}
static void _BlankaSetAnim(Player *ply, u16 d0, u16 d1) {		// 2ef68
	setaction_direct((Object *)ply, data_46df4[d0] + (d1 / 18));
}		
static int  _CheckBuzz1(Player *ply) {		//2eb88
	UD *ud=(UD *)&ply->UserData;
	if (ud->x0082 & BUTTON_A) {
		if (ud->pm1.b < 10) {
			if (ud->x0088 != 0x1ff) {
				return 0;
			}
		}
		if (check_special_ability(ply)) {
			return 0;
		}
		return 1;
	} else {
		return 0;
	}
}
static int  _CheckBuzz2(Player *ply) {		//2eb9c
	UD *ud=(UD *)&ply->UserData;
	if (ud->x0082 & BUTTON_B) {
		if (ud->pm1.b < 10) {		// xxx check it's really pm1.b
			if (ud->x0088 != 0x1ff) {
				return 0;
			}
		}
		if (check_special_ability(ply)) {
			return 0;
		}
		return 1;
	} else {
		return 0;
	}
}
static int  _CheckBuzz3(Player *ply) {		//2ebb0
	UD *ud=(UD *)&ply->UserData;
	if (ud->x0082 & BUTTON_C) {
		if (ud->pm1.b < 10) {		// xxx check it's really pm1.b
			if (ud->x0088 != 0x1ff) {
				return 0;
			}
		}
		if (check_special_ability(ply)) {
			return FALSE;
		}
		return TRUE;
	} else {
		return FALSE;
	}
}
/* xxx refactor */
static void _StartBuzz1(Player *ply) {	// 2ebd8
	UD *ud=(UD *)&ply->UserData;
	ply->mode2 = 0;
	ud->x00a1 = 1;
	_BlankaStartMoveAnim(ply, 0x3e);
}
static void _StartBuzz2(Player *ply) {	// 2ebea
	UD *ud=(UD *)&ply->UserData;
	ply->mode2 = 0;
	ud->x00a1 = 1;
	_BlankaStartMoveAnim(ply, 0x40);
}
static void _StartBuzz3(Player *ply) {	// 2ebfc
	UD *ud=(UD *)&ply->UserData;
	ply->mode2 = 0;
	ud->x00a1 = 1;
	_BlankaStartMoveAnim(ply, 0x42);
}

static void _BlankaChargeCheck(Player *ply, BlankaMove *bm, short buttons_d0, short d1) {	//2eb06
	UD *ud=(UD *)&ply->UserData;
	if (ud->x0082 & buttons_d0) {
		if(--bm->a == 0) {
			bm->b = 0;
			bm->a = d1;
		} 
	} else {
		bm->b++;
		bm->a = d1;
	}
}
static void _BlankaCheckCannonBall(Player *ply) {		// 2ea3a
	u8 data_2ea78[]={0x46, 0x23, 0x0c, 0x1};
	u8 data_2ea9a[]={ 8, 9, 8,10, 8,11, 8, 12, 8, 9, 8,10, 8,11, 8,13,
		8, 9, 8,10, 8,11, 8,14, 8, 9, 8,10, 8,12, 8,15,
	};
	UD *ud=(UD *)&ply->UserData;

	
	switch (ud->x0090) {
		case 0:
			ud->x0092 = 0;
			if(ply->JoyCorrect & 1) {
				if(--ud->x0091 == 0){
					NEXT(ud->x0090);
				}
			} else {
				ud->x0091 = data_2ea78[g.x0320];
			}
			break;
		case 2:
			if((ply->JoyCorrect & 1)==0) {
				NEXT(ud->x0090);
				ud->x0091 = data_2ea9a[RAND32];
			}
			break;
		case 4:
			if (ply->JoyCorrect & 2) {
				NEXT(ud->x0090);
				ud->x0091 = 8;
				ud->x0092 = 6;
			} else {
				if (--ud->x0091 == 0) {
					ud->x0090 = 0;
					ud->x0091 = 60;
				}
			}
			break;
		case 6:
			if (--ud->x0091 == 0) {
				ud->x0091 = 4;
				if (--ud->x0092 == 0) {
					ud->x0090 = 0;
					ud->x0091 = 60;
				}
			}
			break;
		FATALDEFAULT;
	}

}
static u16  _BlankaCheckCannonStart(Player *ply) {		//2eb28
	UD *ud=(UD *)&ply->UserData;
	if((ply->JoyDecode.full ^ ply->JoyDecodeDash.full) & 0x70) {
		decode_buttons(ply, (ply->JoyDecode.full ^ ply->JoyDecodeDash.full) & 0x70);
		if (ply->ButtonStrength + 2 > ud->x0092) {
			if (ud->x0088 != 0xff) {
				return 0;
			}
		}
		if(check_special_ability(ply)){
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}
static void _BlankaStartCannon(Player *ply) {		// 2eb5e
	ply->VelX.full = (short []){0x600, 0x800, 0xa00}[ply->ButtonStrength/2];
	if (ply->Flip == FACING_LEFT) {
		ply->VelX.full = -ply->VelX.full;
	}
	_BlankaStartMoveAnim(ply, 0x46 + ply->ButtonStrength);
}
static void _BlankaExit(Player *ply) {	// 2e550
	UD *ud=(UD *)&ply->UserData;

	ud->x0086 = 0;
	switch (ply->StandSquat) {
		case 0:
			ply_exit_stand(ply);
			break;
		case 2:
			ply_exit_crouch(ply);
			break;
		case 4:
			/* nothing */
			break;
			FATALDEFAULT;
	}
}	
static void sub_2e4fe(Player *ply) {		//2e4fe
	UD *ud=(UD *)&ply->UserData;
	if (ud->x0094 != 5) {
		++ud->x0094;
		if (_BlankaCheckCannonStart(ply)) {
			_BlankaStartCannon(ply);
			return;
		}
	}
	if (AF1) {
		/* suicide was here */
		_BlankaExit(ply);
	} else {
		PLAYERTICK;
	}

}
static void sub_2e496(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	if (_CheckBuzz1(ply)) { _StartBuzz1(ply); return; }
	if (_CheckBuzz2(ply)) { _StartBuzz2(ply); return; }
	if (_CheckBuzz3(ply)) { _StartBuzz3(ply); return; }
	if (AF2) {
		if(PSSetNextAction(ply)) {
			if (_ButtonMask(ply, ud->x0084 & BUTTON_MASK)) {
				ud->x0086 = TRUE;
				plstat_do_nextaction(ply);
				return;
			}
		}
	}
	sub_2e4fe(ply);
}

#pragma mark PL Callbacks

void PLCBPowerBlanka(Player *ply) {
	Object *obj;	// only used for suicide
	UD *ud=(UD *)&ply->UserData;
	if (ud->x0080 == 0) {
		ud->x0091 = 0x3c;
		ud->x0090 = 0;
		ud->x0094 = 0;
		ud->x0086 = 0;
		ud->x0088 = 0;
		ud->x008c = 0;
		ud->pm1.a = 1;
		ud->pm1.b = 0;
		ud->pm2.a = 1;		//probable bug, not zeroing pm[2,3].b
		ud->pm3.a = 1;
		ud->x0080 = TRUE;
	}
	ud->x0082 = (~ply->JoyCorrectDash & ply->JoyCorrect);
	
	_BlankaCheckCannonBall(ply);
	_BlankaChargeCheck(ply, &ud->pm1, BUTTON_A, 0xf);
	_BlankaChargeCheck(ply, &ud->pm2, BUTTON_B, 0xa);
	_BlankaChargeCheck(ply, &ud->pm3, BUTTON_C, 0x5);
	if (ud->x0082 & 0x70) {
		ud->x0088 = (sf2rand() & 1) << 16 + sf2rand();
	} else {
		return;
	}

	if (ud->pm1.b == 0) {
		return;
	}
	if (--ud->pm1.b) {
		return;
	}
	/* suicide, shouldn't get here */
	panic(111);
	if (obj = AllocActor()) {
		obj->Sel = sf2rand();	// !!!
		obj->exists = TRUE;
		obj->mode0 = sf2rand();
	}
}
static struct blankathrow _BlankaCheckThrow(Player *ply) {		// 2ec0e
	UD *ud=(UD *)&ply->UserData;
	short airthrow_d6 = FALSE;		// xxx
	static const char data_2ec8a[6][8]={
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{ 0x60, 0,    0,    0,    0, 2,0x3c, 0, },
			{ 0, 0xe0, 0x35, 0x20, 0x10, 0,0x44, 0, },
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{    0, 0,    0,    0,    0, 0,   0, 0, },
	};
	const char *data;
	short d0;
	struct blankathrow retval;

	retval.success = FALSE;
	
	if ((ply->JoyDecode.full & 4)==0) { return retval; }
	if ((ud->x0082 & 0x60) == 0)	  { return retval; }
	d0 = _IDFromButton(ply);
	if (d0 == 0)					  { return retval; }
	data = data_2ec8a[d0-1];
	if (data[6] == 0) {
		retval.success = FALSE;
		return retval; 
	}
	if (data[0]) {
		if (ply->OppXDist > data[0]) {
			return retval;
		} else {
			retval.d2 = data[4];
			retval.success = TRUE;
			retval.d0 = data[5];
			return retval;
		}
	} else {
		PLY_THROW_SET(data[1], data[2], data[3], data[4]);
		if (_check_throw(airthrow_d6, ply)) {
			retval.d2 = data[5];
			retval.success = TRUE;
			retval.d0 = data[6];
			return retval;
		}
		return retval;
	}
}
static u16 _AnyButtons(Player *ply) {		// 2ef38
	return _ButtonMask(ply, BUTTON_MASK);
}
static void _BlankaCheckMoves(Player *ply, u16 d0, short initial_d2) {		//2ee5c
	UD *ud=(UD *)&ply->UserData;
	const char *data, *data2;
	short d2;
	struct blankathrow BT;
	const static char data_2eecc[4][6][4] = {		// same as 33a58
		{ 
			{ 0x28, 0x01, 0x04, 0x0a,  },
			{ 0x28, 0x02, 0x02, 0x08,  },
			{ 0x28, 0x03, 0x00, 0x06,  },
			{ 0x2b, 0x01, 0x12, 0x0e,  },
			{ 0x2e, 0x02, 0x14, 0x10,  },
			{ 0x32, 0x03, 0x0c, 0x16,  },
		},{ 
			{ 0xff, 0x01, 0x28, 0x28,  },
			{ 0xff, 0x02, 0x2a, 0x2a,  },
			{ 0xff, 0x03, 0x2c, 0x2c,  },
			{ 0xff, 0x01, 0x24, 0x24,  },
			{ 0xff, 0x02, 0x26, 0x26,  },
			{ 0xff, 0x03, 0x18, 0x18,  },
		},{ 
			{ 0xff, 0x01, 0x1c, 0x1c,  },
			{ 0xff, 0x02, 0x1e, 0x1e,  },
			{ 0xff, 0x03, 0x2e, 0x2e,  },
			{ 0xff, 0x01, 0x30, 0x30,  },
			{ 0xff, 0x02, 0x32, 0x32,  },
			{ 0xff, 0x03, 0x38, 0x38,  },
		},{ 
			{ 0xff, 0x01, 0x20, 0x20,  },
			{ 0xff, 0x02, 0x22, 0x22,  },
			{ 0xff, 0x03, 0x1a, 0x1a,  },
			{ 0xff, 0x01, 0x34, 0x34,  },
			{ 0xff, 0x02, 0x36, 0x36,  },
			{ 0xff, 0x03, 0x3a, 0x3a,  },
		}, };
	/* next address 0002ef2c */
	ud->x0084 = d0;
	if (d0 & BUTTON_A) {
		d2 = 0;
	} else if (d0 & BUTTON_B) {
		d2 = 4;
	} else if (d0 & BUTTON_C) {
		d2 = 8;
	} else if (d0 & BUTTON_D) {
		d2 = 0xc;
	} else if (d0 & BUTTON_E) {
		d2 = 0x10;
	} else {
		d2 = 0x14;
	}
	data = data_2eecc[initial_d2/4][d2/4];
	data2 = data;
	if (data[0] < ply->OppXDist) {
		data2++;
	}
	BT.d0 = data2[2];
	BT.d2 = data[1];
	// 2ef7e
	if (BT.d2) {
		quirkysound(BT.d2 - 1);
	}
	_BlankaStartMoveAnim(ply, BT.d0);
}
short PLCBStandBlanka(Player *ply) {		// 2e8e6
	UD *ud=(UD *)&ply->UserData;
	struct blankathrow BT;
	short d0;
	ply->StandSquat = 0;
	ud->x0094 = 0;
	decode_buttons(ply, ud->x0082);
	if (_BlankaCheckCannonStart(ply)) { _BlankaStartCannon(ply); return TRUE; }
	if (_CheckBuzz1(ply)) { _StartBuzz1(ply); return TRUE; }
	if (_CheckBuzz2(ply)) { _StartBuzz2(ply); return TRUE; }
	if (_CheckBuzz3(ply)) {
		_StartBuzz3(ply);
		return TRUE;
	}
	BT = _BlankaCheckThrow(ply);
	if (BT.success) {
		// 2ef7e
		if (BT.d2) {
			quirkysound(BT.d2 - 1);
		}
		_BlankaStartMoveAnim(ply, BT.d0);
		return TRUE;
	}
	d0 = _AnyButtons(ply);
	if (d0) {
		//2ee44(ply);
		_BlankaCheckMoves(ply, d0, 0 );		// XXX can't find where D2 was set!
		return TRUE;
	}
	return FALSE;
}
short PLCBCrouchBlanka(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	short d0;
	
	ply->StandSquat = PLY_CROUCH;
	if (_CheckBuzz1(ply)) { _StartBuzz1(ply); return TRUE; }
	if (_CheckBuzz2(ply)) { _StartBuzz2(ply); return TRUE; }
	if (_CheckBuzz3(ply)) { _StartBuzz3(ply); return TRUE; }
	ud->x0094 = 0;
	
	d0 = _AnyButtons(ply);
	if (d0) {
		_BlankaCheckMoves(ply, d0, 4);
		return TRUE;
	}
	return FALSE;
}
short PLCBJumpBlanka(Player *ply) {
	UD *ud=(UD *)&ply->UserData;

	short d0;
	ply->StandSquat = 4;
	ud->x0094 = 0;
	d0 = _AnyButtons(ply);
	if (d0) {
		//sub_2ee4e
		if (ply->VelX.full == 0) {
			_BlankaCheckMoves(ply, d0, 8);
		} else {
			_BlankaCheckMoves(ply, d0, 12);
		}
		return TRUE;
	}
	return FALSE;
}

#pragma mark Blanka Attacks
static void sub_2e57a(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	if (AF2) {
		if(_ButtonMask(ply, ud->x0082 & 0x40)) {
			_BlankaSetAnim(ply, 0, 0x18);
		} else {
			ply_exit_stand(ply);
		}
	} else {
		if (AF1) {
			ply_exit_stand(ply);
		} else {
			PLAYERTICK;
		}

	}
}
static void _CannonBounce(Player *ply) {		//2e6b2
	ply->mode2 = 8;
	ply->VelX.full = ply->Flip ? -0x200 : 0x200 ;
	ply->VelY.full   = 0x0a00;
	ply->AclY.full = 0x0080;
}
static void sub_2e670(Player *ply) {
	if (AF2 && ply->Timer == 1) {
		quirkysound(1);
	}
	PLAYERTICK;
	CAApplyVelocity((Object *)ply);
}
static void sub_2e6da(Player *ply) {		// 2e6da
	UD *ud=(UD *)&ply->UserData;

	soundsting(SOUND_IMPACT8);
	ply->VelX.full = 0;
	ud->x008a = 0;
	ply_exit_air(ply);
}
static void _BlankaSMCannonBall(Player *ply) {		// 2e5b2
	UD *ud=(UD *)&ply->UserData;

	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			BumpDiff_PowerMove();
			/* FALLTHRU */
		case 2:
			ud->x0090 = 0;
			ud->x0091 = 60;
			ply->Jumping = 1;
			if (ud->x008a) {
				_CannonBounce(ply);
				return;
			}
			NEXT(ply->mode2);
			ud->x0093 = 10;
			ply->VelY.full = 0;
			if (AF2 && ply->Timer == 1) {
				quirkysound(1);
			}
			PLAYERTICK;
			CAApplyVelocity((Object *)ply);
			break;
		case 4:
			if (ud->x008a) {
				_CannonBounce(ply);
				return;
			}
			if (--ud->x0093) {
				NEXT(ply->mode3);
				ud->x0093 = 40;
			}
			if (AF2 && ply->Timer == 1) {
				quirkysound(1);
			}
			PLAYERTICK;
			break;
		case 6:
			if (ud->x008a) {
				_CannonBounce(ply);
				return;
			}
			if ( ((ply->BoundCheck | ply->PlatformFallDir) & (ply->Flip + 1)) == 0) {
				if(g.GPCollDetect) {
					_CannonBounce(ply);
					return;
				}
				if(--ud->x0093) {
					sub_2e670(ply);
					return;
				}
			}
			sub_2e6da(ply);
			break;
		case 8:
			CATrajectory((Object *)ply);
			if (check_ground_collision((Object *)ply)) {
				sub_2e6da(ply);
			} else {
				if (AF2 && ply->Timer == 1) {
					quirkysound(1);
				}
				PLAYERTICK;
			}
			break;
		default:
			break;
	}
}
static void _CheckNextAction(Player *ply) {		//2e4d6
	UD *ud=(UD *)&ply->UserData;

	if (AF2) {
		if (PSSetNextAction(ply)) {
			if(_ButtonMask(ply, ud->x0084 & BUTTON_MASK)) {
				ud->x0086 = 1;
				plstat_do_nextaction(ply);
				return;
			}
		}
	} 
	PLAYERTICK;
}
static void _BzztSound(Player *ply) {		// 2e7ba
	UD *ud=(UD *)&ply->UserData;
	if (--ud->x00a1 == 0) {
		ud->x00a1 = 8;
		soundsting(SOUND_ZAP1_AGAIN);
	}
	PLAYERTICK;
}
static void _CheckFlip(Player *ply) {		//2ee32
	if (ply->Flip != ply->EnemyDirection) {
		ply->Flip ^= 1;
	}
}
static int _IDFromButton(Player *ply) {		//2efa6
	UD *ud=(UD *)&ply->UserData;

	u16 d1;
	d1 = ud->x0082 & BUTTON_MASK;
	if (d1 == 0) {
		return 0;
	}
	if (d1 & BUTTON_A) {
		return 1;
	}
	if (d1 & BUTTON_B) {
		return 2;
	}
	if (d1 & BUTTON_C) {
		return 3;
	}
	ply->PunchKick = PLY_KICKING;
	if (d1 & BUTTON_D) {
		return 4;
	}
	if (d1 & BUTTON_E) {
		return 5;
	}
	return 6;
}
static void _BlankaSMElectric(Player *ply) {		// 2e6f0
	UD *ud=(UD *)&ply->UserData;
	const static unsigned char data_2e79e[] = {
		255, 0, 5, 10, 255, 255, 255, 255, 255, 255
	};
	u16 d0;
	
	switch (ply->mode2) {
		case 0:
			if (AF1) {
				NEXT(ply->mode2);
				BumpDiff_PowerMove();
				ud->x009a = 15;
				ud->x00a1 = 8;
			}
			_BzztSound(ply);
			break;
		case 2:
			if(g.TimeOut | g.CarWasted) {
				_BlankaExit(ply);
			} else if (g.RoundResult) {
				NEXT(ply->mode2);
				ud->x009a = 20;
				_BzztSound(ply);
			} else if (check_platform_end(ply)) {
				soundsting(SOUND_ZAP1_AGAIN);
				exit_jumping2(ply);
			} else {
				_CheckFlip(ply);
				d0 = _IDFromButton(ply);
				if (data_2e79e[d0] >= ud->x009a) {
					if (--ud->x009a == 0) {
						_BlankaExit(ply);
					} else {
						_BzztSound(ply);
					}
				} else {
					ud->x009a = 15;
					if (ply->Move == ((d0 * 2)+0x3c)) {
						_BzztSound(ply);
					} else {
						ply->Move = (d0 * 2) + 0x3c;
						_BlankaSetAnim(ply, ply->Move, ply->AnimFlags & 0x1ff);			/* iffy, will probaby break */
					}
				}

			}
			break;
		case 4:
			if (--ud->x009a) {
				_BzztSound(ply);
			} else {
				soundsting(SOUND_ZAP1_AGAIN);
				ply_exit_stand(ply);
			}
			break;
		FATALDEFAULT;
	}
}
static void _BlankaSMCatch(Player *ply) {		// 2e7d2
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			random_damage_adjust_2(ply, 35);
			random_damage_adjust_1(ply, 0xc, 7);
			ply->LocalTimer = 0x82;
			_BlankaCatchLU(ply);
			_BlankaCatchSet(ply);
			break;
		case 2:
			if (--ply->LocalTimer == 0 || ply_opp_has_struggled_free(ply)) {
				ply_grip_release(ply, ply->Flip);		// d1 = 0, d2 = 0x20;
				NEXT(ply->mode2);
				sub_33ada(ply, 0x7a);
			} else {
				if (sub_3fee(ply)) {
					ply->Timer = 1;
				}
				if (AF2) {
					ActBlankaBiteBlood(ply);
					if(sub_3466(ply, 0, 0, 0, 0xff00, 0x27)) {
						NEXT(ply->mode2);
						sub_33ada(ply, 0x7a);
						return;
					} 
				}	
				PLAYERTICK;
				_BlankaCatchSet(ply);
			}
			break;
		case 4:
			if (AF2) {
				soundsting(SOUND_IMPACT8);
				ply->YPI = g.GroundPlaneY;
				ply_exit_stand(ply);
			} else {
				PLAYERTICK;
			}
			break;

		FATALDEFAULT;
	}
}

void PSCBAttackBlanka(Player *ply) {		/* 2e42a */
	UD *ud=(UD *)&ply->UserData;
	
	if (ply->Timer2) {
		ply->Timer2--;
		ud->x008a = 1;
		return;
	}
	switch (ply->Move) {
		case 0:
			sub_2e57a(ply);
			break;
		case 2:		case 6:		case 8:		case 0xc:
		case 0xe:	case 0x10:	case 0x12:	case 0x14:
		case 0x16:	case 0x18:	case 0x24:	case 0x26:
		case 0x2a:	case 0x2c:	case 0x3c:
			sub_2e4fe(ply);
			break;
		case 4:		case 0xa:	
			sub_2e496(ply);
			break;
		case 0x1a:	case 0x1c:	case 0x1e:	case 0x20:
		case 0x22:	case 0x2e:	case 0x30:	case 0x32:
		case 0x34:	case 0x36:	case 0x38:	case 0x3a:
			PLAYERTICK;		// (2e5ae)
			break;
		case 0x28:
			_CheckNextAction(ply);
			break;
		case 0x3e:	case 0x40:	case 0x42:
			_BlankaSMElectric(ply); 
			break;
		case 0x44:
			_BlankaSMCatch(ply);
			break;
		case 0x46:	case 0x48:	case 0x4a:
			_BlankaSMCannonBall(ply);
			break;
		FATALDEFAULT;	
	}	
}
void PSCBVictoryBlanka(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	short d0;
	Object *obj;
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			if (ply->RoundsWon != 2 && g.OnFinalStage==0 &&	g.NotUsed==0) {
				NEXT(ply->mode3);
			}
			d0 = (RAND16) + 0x20;
			if (g.OnBonusStage) {
				d0 -= 0x10;
				if (ply->RoundsWon == 1) {
					d0 -= 0x10;
				}
			}
			if ((char []){1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
				          0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
						  1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,}[d0]) 
			{
				sub_33ada(ply, 0x78);
			} else {
				sub_33ada(ply, 0x76);
			}
			g.FightOver = TRUE;
			if (ply->Opponent->XPI > ply->XPI) {
				ply->Flip = FACING_RIGHT;
			} else {
				ply->Flip = FACING_LEFT;
			}
			break;
		case 2:
			ply->PSFinishedParticipating = TRUE;
			if (AF2) {
				soundsting(AF2);
			}
			PLAYERTICK;
			break;
		case 4:
			ply->PSFinishedParticipating = TRUE;
			if (g.Pause_9e1 == 2) {
				if (obj=AllocActor()) {
					INITOBJ(obj, 0x25, 0xa);
					obj->Pool = 6;
					obj->Owner = ply;
					ud->x00a2 = obj;
					NEXT(ply->mode3);
				}				
			}
			if (AF2) {
				soundsting(AF2);
			}
			PLAYERTICK;
			break;	
		case 6:
			if (AF1) {
				if (ud->x00a2->XPI - ply->XPI + 0x50 < 0xa0 &&  ud->x00a2->XPI - ply->XPI + 0x50 > 0) {
					NEXT(ply->mode3);
					if (ply->Flip == ud->x00a2->Flip) {
						sub_33ada(ply, 0x4c);
					} else {
						ply->Flip ^= 1;
						sub_33ada(ply, 0x4c);
					}
				} else {
					PLAYERTICK;
				}
			} else {
				PLAYERTICK;
			}
			break;
		case 8:
			if (AF2) {
				soundsting(AF2);
			}
			PLAYERTICK;
		default:
			break;
	}
}
	
#pragma mark Computer CBs


static struct blankathrow sub_33956(Player *ply, short airthrow_d6) {
	struct blankathrow retval;
	
	static char data_339c8[2][3][8]={
		{
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{ 0x60, 0,    0,    0,    0, 2,0x3c, 0, },
			{    0, 0, 0x50, 0x40, 0x40, 0,0x44, 0, },
		}, {
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{    0, 0,    0,    0,    0, 0,   0, 0, },
		}
	};
	
	char *data;
	
	if (ply->CompDoThrow == FALSE) { 
		retval.success= FALSE;
		return retval;
	}
	data = data_339c8[ply->PunchKick][ply->ButtonStrength];
	if (data[6] == 0) {
		retval.success = FALSE;
		return retval; 
	}
	if (data[0]) {
		if (ply->OppXDist > data[0]) {
			retval.success = FALSE;
			return retval;
		} else {
			retval.d0 = data[4];		// XXX probable bug, should have been %d2
			retval.success = TRUE;
			retval.d0 = data[5];
			return retval;
		}
	} else {
		PLY_THROW_SET(data[1], data[2], data[3], data[4]);
		if (_check_throw(airthrow_d6, ply)) {
			retval.d2 = data[5];
			retval.success = TRUE;
			retval.d0 = data[6];
			return retval;
		}
		retval.success = FALSE;
		return retval;
	}
}


static void sub_33688(Player *ply) {	
	UD *ud=(UD *)&ply->UserData;
	
	if (AF1) {
		
		switch (ply->StandSquat) {
			case 0:				//336ac
				ply->AISigAttack = FALSE;
				ply->AIVolley = FALSE;
				ud->x008a  = 0;
				exit_comp_normal(ply);
				break;
			case 2:				//336be
				ply->AISigAttack = FALSE;
				ply->AIVolley = FALSE;
				ud->x008a  = 0;
				exit_to_compdisp1(ply);
				break;
			case 4:
			case 6:
				//336f4
				ply->AISigAttack = FALSE;
				ply->AIVolley = FALSE;
				ud->x008a = 0;
				PLAYERTICK;
				break;
			FATALDEFAULT;
		}
	} else {
		PLAYERTICK;
	}
}
static void sub_3366c (Player *ply) {
	
	if (AF2) {
		if (comp_diceroll(ply)) {
			if (comp_setnextaction(ply)) {
				comp_do_next_action(ply);
			}
		}
	}
	sub_33688(ply);
}
static void sub_33ada(Player *ply, short sel ) {
	// XXX setaction_direct((Object *)ply, actlist_46df4[sel]);
}
static void sub_33ac8(Player *ply) {
	if (ply->Flip ^= ply->EnemyDirection) {
		ply->Flip ^= 1;
	}
}
static void sub_3383c(Player *ply) {
	UD *ud=(UD *)&ply->UserData;

	if (--ud->pm1.a == 0) {
		ud->pm1.a = 8;
		queuesound(SOUND_ZAP1_AGAIN);
	}
	PLAYERTICK;
}
static void sub_337f2(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	
	switch (ply->mode3) {
		case 0:
			ud->pm1.a = 1;
			/* FALLTHRU */
		case 2:
			sub_33ac8(ply);
			if (--ud->x009a == 0) {
				// inlined sub_336ac(ply);
				ply->AISigAttack = FALSE;
				ply->AIVolley = FALSE;
				ud->x008a  = 0;
				exit_comp_normal(ply);
				return;
			}
			if (g.RoundResult) {
				NEXT(ply->mode3);
				ud->x009a = 0x14;
			} 
			sub_3383c(ply);
			break;
		case 4:
			sub_33ac8(ply);
			if (--ud->x009a == 0) {
				// inlined sub_336ac(ply);
				ply->AISigAttack = FALSE;
				ply->AIVolley = FALSE;
				ud->x008a  = 0;
				exit_comp_normal(ply);
				return;				
			}
			sub_3383c(ply);
			break;
		FATALDEFAULT;
	}
}
static void sub_337be(Player *ply) {
	ply->mode3 = 6;
	ply->VelX.full = ply->Flip ? -0x200 : 0x200;
	ply->AclX.full = 0;
	ply->VelY.full = 0x0a00;
	ply->AclY.full = 0x0080;	
}
static void sub_336d0(Player *ply) {
	ply->AISigAttack	= FALSE;
	ply->AIVolley			= FALSE;
	ply->UserData[0xa] = 0;
	comp_setjumping_main(ply);
}
static void sub_33704(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	
	switch (ply->mode3) {
		case 0:
			ply->Airborne = AIR_JUMPING;
			if (ud->x008a) {		//337be
				sub_337be(ply);
			} else {
				ud->x0092		= 0x28;
				ply->VelY.full	= 0;
				if (AF2 && (ply->Timer == 1)) {
					quirkysound(1);
				}
				PLAYERTICK;
			}
			break;
		case 2:
			if (g.GPCollDetect ) {
				NEXT(ply->mode3);
				ud->x0092 = 2;
			}
			NEXT(ply->mode3);
			ud->x0092 = 2;
			/* FALLTHRU */
		case 4:
			if (ud->x008a) {				
				sub_337be(ply);
			} else {
				if (ply->BoundCheck == ply->Flip + 1) {
					queuesound(SOUND_IMPACT8);
					sub_336d0(ply);			// blanka exit stand
				} else if (g.GPCollDetect) {
					sub_337be(ply);
				} else if (--ud->x0092 == 0){
					queuesound(SOUND_IMPACT8);
					sub_336d0(ply);
				} else {
					if (AF2 && ply->Timer == 1) {
						quirkysound(1);
					}
					PLAYERTICK;
					CAApplyVelocity((Object *)ply);
				}
			}
			break;
		case 6:
			CATrajectory((Object *)ply);
			if (check_ground_collision((Object *)ply)) {
				queuesound(SOUND_IMPACT8);
				sub_336d0(ply);
			} else {
				if (AF2 && ply->Timer == 1) {
					quirkysound(1);
				}
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_338d8(Player *ply) {
	ply_grip_release(ply, ply->Flip);		// %d0 = 0, %d1 = flip,  %d2 = 0x20
	NEXT(ply->mode3);
	sub_33ada(ply, 0x7a);
}
static void sub_33854(Player *ply) {
	UD *ud=(UD *)&ply->UserData;

	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			random_damage_adjust_2(ply, 35);
			random_damage_adjust_1(ply, 0xc, 7);
			ply->LocalTimer = 0x82;
			_BlankaCatchLU(ply);
			_BlankaCatchSet(ply);
			break;
		case 2:
			if (--ply->LocalTimer == 0) {
				sub_338d8(ply); 
				return;
			} else {
				if(ply_opp_has_struggled_free(ply)) { sub_338d8(ply); return; }
				if (sub_3fee(ply)) { ply->Timer = 1;}
				if (AF2 == 0) {
					PLAYERTICK;
					_BlankaCatchSet(ply);
					return;
				} else {
					ActBlankaBiteBlood(ply);
					if(sub_3466(ply, 0, 0, 0, 0xff00, 0x27)) {
						NEXT(ply->mode3);
						sub_33ada(ply, 0x7a);
						return;
					}
					PLAYERTICK;
					_BlankaCatchSet(ply);
				}
			}
			break;
		case 4:
			if (AF2) {
				queuesound(SOUND_IMPACT8);
				ply->YPI = g.GroundPlaneY;
				// inlined sub_336ac(ply);
				ply->AISigAttack = FALSE;
				ply->AIVolley = FALSE;
				ud->x008a  = 0;
				exit_comp_normal(ply);
			} else {
				PLAYERTICK;
			}
			break;
		default:
			break;
	}
}


static int sub_33b02(Player *ply, short sound_d2, short d0) {		 
	UD *ud=(UD *)&ply->UserData;
	
	if (sound_d2) {
		quirkysound((sound_d2 & 0xff) - 1);
	}
	ud->x008a = 0;
	ud->x0090 = d0;
	ply->mode3 = 0;
	sub_33ada(ply, d0);
	return 1;
}
static int sub_33a10(Player *ply, short d2) {
	const char *data;
	static const char data_33a58[4][2][3][4] = {
		{ 
			{
				{ 0x28, 0x01, 0x04, 0x0a,  },
				{ 0x28, 0x02, 0x02, 0x08,  },
				{ 0x28, 0x03, 0x00, 0x06,  },
			},{
				{ 0x2b, 0x01, 0x12, 0x0e,  },
				{ 0x2e, 0x02, 0x14, 0x10,  },
				{ 0x32, 0x03, 0x0c, 0x16,  },
			}
		},{ 
			{
				{ 0xff, 0x01, 0x28, 0x28,  },
				{ 0xff, 0x02, 0x2a, 0x2a,  },
				{ 0xff, 0x03, 0x2c, 0x2c,  },
			},{
				{ 0xff, 0x01, 0x24, 0x24,  },
				{ 0xff, 0x02, 0x26, 0x26,  },
				{ 0xff, 0x03, 0x18, 0x18,  },
			}
		},{ 
			{
				{ 0xff, 0x01, 0x1c, 0x1c,  },
				{ 0xff, 0x02, 0x1e, 0x1e,  },
				{ 0xff, 0x03, 0x2e, 0x2e,  },
			},{
				{ 0xff, 0x01, 0x30, 0x30,  },
				{ 0xff, 0x02, 0x32, 0x32,  },
				{ 0xff, 0x03, 0x38, 0x38,  },
			}
		},{ 
			{
				{ 0xff, 0x01, 0x20, 0x20,  },
				{ 0xff, 0x02, 0x22, 0x22,  },
				{ 0xff, 0x03, 0x1a, 0x1a,  },
			},{
				{ 0xff, 0x01, 0x34, 0x34,  },
				{ 0xff, 0x02, 0x36, 0x36,  },
				{ 0xff, 0x03, 0x3a, 0x3a,  },
			}
		}, 
	};
	struct blankathrow retval;
	data = data_33a58[d2/4][ply->PunchKick/2][ply->ButtonStrength/2]; 
	if (data[0] < ply->OppXDist ) {
		retval.d0 = data[3];
	} else {
		retval.d0 = data[2];
	}
	
	retval.d0 = data[2];
	retval.d2 = data[1];
	sub_33b02(ply, retval.d2, retval.d0);
	return 1;
}
static void sub_33ab8(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	short d0;
	
	switch (ply->PunchKick) {
		case PLY_PUNCHING:			// 33936
			queuesound(SOUND_SWOOSH1);
			ud->x009a = ply->AIMultiCount;
			d0 = 0x3e + ply->ButtonStrength;

			// 33b14 inlined
			ud->x008a = 0;
			ud->x0090 = d0;
			sub_33ada(ply, d0);
			break;
		case PLY_KICKING:			//3390c
			ply->VelX.full = (short []){0x600, 0x800, 0xa00}[ply->ButtonStrength/2];
			if (ply->Flip == FACING_LEFT) {
				ply->VelX.full = -ply->VelX.full;
			}
			d0 = 0x46 + ply->ButtonStrength;

			// 33b14 inlined
			ud->x008a = 0;
			ud->x0090 = d0;
			sub_33ada(ply, d0);
			break;
		FATALDEFAULT;
	}
	
}

void PLCBCompJumpBlanka(Player *ply) {	//335bc
	//33a02
	if (ply->VelX.full == 0) {
		sub_33a10(ply, 8);
	} else {
		sub_33a10(ply, 0xc);
	}
}	

void PLCBCompAttackBlanka(Player *ply) {		//335c0
	struct blankathrow BT;
	UD *ud=(UD *)&ply->UserData;
	
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->mode3 = 0;
			ud->x008a = 0;
			BT = sub_33956(ply, 0);	
			if (BT.success == TRUE) {
				sub_33b02(ply, BT.d2, BT.d0);
			} else if (ply->CompDoThrow != FALSE) {
				sub_33ab8(ply);
			} else {
				switch (ply->StandSquat) {
					case 0:
						//339f8 
						sub_33a10(ply, 0);
						break;
					case 2:
						//339fc
						sub_33a10(ply, 4);
						break;
					case 4:
						//33a02
						if (ply->VelX.full == 0) {
							sub_33a10(ply, 8);
						} else {
							sub_33a10(ply, 0xc);
						}
						break;
						FATALDEFAULT;
				}	
			}
			break;
		case 2:							//33600
			if (ply->Timer2) {
				--ply->Timer2;
				ud->x008a = 1;
			} else {
				switch (ud->x0090) {
					case 0:		case 4:		case 0xa:	case 0x24:
					case 0x28:
						sub_3366c(ply);
						break;
					case 2:		case 6:		case 8:		case 0xc:
					case 0xe:	case 0x10:	case 0x12:	case 0x14:
					case 0x16:	case 0x18:	case 0x1a:	case 0x1c:
					case 0x1e:	case 0x20:	case 0x22:	case 0x26:
					case 0x2a:	case 0x2c:	case 0x2e:	case 0x30:
					case 0x32:	case 0x34:	case 0x36:	case 0x38:
					case 0x3a:	case 0x3c:
						sub_33688(ply);
						break;
					case 0x3e:	case 0x40:	case 0x42:
						sub_337f2(ply);
						break;
					case 0x44:
						sub_33854(ply);
						break;
					case 0x46:	case 0x48:	case 0x4a:
						sub_33704(ply);
						break;
					FATALDEFAULT;
				}
			}
			break;
			FATALDEFAULT;
	}
}
