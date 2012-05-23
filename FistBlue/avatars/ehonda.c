/*
 *  ehonda.c
 *  GLUTBasics
 *
 *  Created by Ben on 18/12/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
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


#include "ehonda.h"
#include "ehondadata.h"

extern Game g;


typedef struct UserData_EHonda UD;


void pl_cb_setstatus2_ehonda(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_3d2fc[status / 2], argd0);
}


void pl_cb_setstatus3_ehonda(Player *ply, short status) {
	pl_cb_setstatus2_ehonda(ply, status, ply->Step ^ ply->Flip);
}


void pl_cb_setstatus1_ehonda(Player *ply, short status) {
	pl_cb_setstatus2_ehonda(ply, status, 0);
}

static short _EHondaCheckFreeOink(Player *ply) {	/* 2e062 check free oink */
	UD *ud=(UD *)&ply->UserData;

	short temp = (ply->JoyDecode.full ^  ply->JoyDecodeDash.full) & 0x70;
	if (temp == 0) {
		return 0;
	}
	decode_buttons(ply, temp);
	if ((ply->ButtonStrength + 2 <= ud->x0093 || ud->x0088 == 0x00ff)
										&& check_special_ability(ply)) {
		return 1;	
	} else {
		return 0;
	}
}

static struct ehondares sub_2e0c2(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	struct ehondares EH;
	
	if (ud->x0082 & BUTTON_A) {
		if (ud->x0098 >= 8)     {EH.d0 = 2; EH.d1=1; return EH;}
		if (ud->x0088 == 0x1ff) {EH.d0 = 2; EH.d1= 1; return EH;}
	}
	if (ud->x0082 & BUTTON_B) {
		if (ud->x009a >= 8)     {EH.d0 = 4; EH.d1=1; return EH; }
		if (ud->x0088 == 0x1ff) {EH.d0 = 4; EH.d1 = 1; return EH;}
	}
	if (ud->x0082 & BUTTON_C) {
		if (ud->x009c >= 8)     {EH.d0 = 6; EH.d1 = 1; return EH; }
		if (ud->x0088 == 0x1ff) {EH.d0 = 6; EH.d1 = 1; return EH;}
	}
	EH.d0 = 0;
	EH.d1 = ud->x0082;
	return EH;
}
static void _CheckCharge(Player *ply, EHondaMove *bm, short buttons_d0, short d1) { //2e040
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
static void _EHondaSMCheckOink(Player *ply) {		// 2df74
	static const u8 data_2dfa6[]={0x3c, 0x23, 0x0c, 0x1};
	static const u8 data_2dfd4[]={ 8, 9, 8,10, 8,11, 8, 12, 8, 9, 8,10, 8,11, 8,13,
		8, 9, 8,10, 8,11, 8,14, 8, 9, 8,10, 8,12, 8,15,
	};
	UD *ud=(UD *)&ply->UserData;

	
	switch (ud->x0091) {
		case 0:
			ud->x0093 = 0;
			if(ply->JoyCorrect & 1) {
				if(--ud->x0092 == 0){
					NEXT(ud->x0091);
				}
			} else {
				ud->x0092 = data_2dfa6[g.x0320];
			}
			break;
		case 2:
			if((ply->JoyCorrect & 1)==0) {
				NEXT(ud->x0091);
				ud->x0092 = data_2dfd4[RAND32];
			}
			break;
		case 4:
			if (ply->JoyCorrect & 2) {
				NEXT(ud->x0091);
				ud->x0092 = 8;
				ud->x0093 = 6;
			} else {
				if (--ud->x0092 == 0) {
					ud->x0091 = 0;
					ud->x0092 = 60;
				}
			}
			break;
		case 6:
			if (--ud->x0092 == 0) {
				ud->x0092 = 4;
				if (--ud->x0093 == 0) {
					ud->x0091 = 0;
					ud->x0092 = 60;
				}
			}
			break;
			FATALDEFAULT;
	}
	
}
static short _EHondaStartMoveAnim(Player *ply, short d0) {		// 2e3e6
	UD *ud=(UD *)&ply->UserData;
	ud->x008a = 0;
	ud->x0095 = 0;
	ply->Move = d0;
	g.HumanMoveCnt++;
	return 1;
}
static void _EHondaStartOink(Player *ply) {			// 2e098
	static short data_2e0b6[][2]={{0x3a, 0x600}, {0x3c, 0x800}, {0x3e, 0xa00}};
	
	if (ply->Flip) {
		ply->VelX.full = 	data_2e0b6[ply->ButtonStrength][1];
	} else {
		ply->VelX.full =   -data_2e0b6[ply->ButtonStrength][1];
	}
	_EHondaStartMoveAnim(ply, data_2e0b6[ply->ButtonStrength][0]);
}
static void sub_2daf6(Player *ply) {
	UD *ud=(UD *)&ply->UserData;

	ud->x008a = 0;
	ud->x0086 = 0;
	switch (ply->StandSquat) {
		case 0:	ply_exit_stand(ply);	break;
		case 2: ply_exit_crouch(ply);	break;
		case 4:							break;
	}
}	
static void sub_2dad8(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	
	if(ud->x0095 != 5) {
		ud->x0095++;
		if (_EHondaCheckFreeOink(ply)) {	/* check for a free oink */
			_EHondaStartOink(ply);		/* oink ? */
			return;
		}
	}
	if (AF1) {
		sub_2daf6(ply);
	} else {
		PLAYERTICK;
	}
}	
static short _ButtonMask(Player *ply, short d1) {		// 2e392
	UD *ud=(UD *)&ply->UserData;
	if (ud->x0086) {
		ud->x0086 = 0;
		return d1 & ud->x0084;
	} else {
		return d1 & ud->x0082;
	}
}
static int sub_2e116(Player *ply, short d0) {
	switch (d0) {
		case 0:
			/* nothing */
			return FALSE;
			break;
		case 2:
			ply->mode2 = 0;
			_EHondaStartMoveAnim(ply, 0x34);
			break;
		case 4:
			ply->mode2 = 0;
			_EHondaStartMoveAnim(ply, 0x36);
			break;
		case 6:
			ply->mode2 = 0;
			_EHondaStartMoveAnim(ply, 0x38);
			break;
		FATALDEFAULT;
	}
	return TRUE;
}

static void _OinkBounce(Player *ply) {		//2dbfe
	ply->mode2 = 4;		
	ply->VelX.full = ply->Flip ? 0x400 : -0x400;
	ply->AclX.full = 0xffe0;
	ply->VelY.full = 0;
	ply->AclY.full = 0;
}	

void _EHondaSMOink(Player *ply) {	//2db28
	UD *ud=(UD *)&ply->UserData;

	switch (ply->mode2) {
		case 0:
			if (ud->x008a) {
				ply->mode2 = 4;		//2dbfe
				ply->VelX.full = ply->Flip ? 0x400 : -0x400;
				ply->AclX.full = 0xffe0;
				ply->VelY.full = 0;
				ply->AclY.full = 0;
			} else if (AF2 == 1) {
				//2db7c
				PLAYERTICK;
			} else {
				NEXT(ply->mode2);
				BumpDiff_PowerMove();
				ud->x0091=0;
				ud->x0092=0x2d;
				ud->x0094=0xa;
				ply->VelY.full = 0;
				ply->UserData[0xa] = 0;
				ply->Airborne = AIR_JUMPING;
				soundsting(SOUND_OINK);
				PLAYERTICK;
			}
			break;
		case 2:
			if (ply->UserData[0xa]) {
				NEXT(ply->mode2);
				return;
			} else if (--ply->UserData[0x14] == 0) {
				NEXT(ply->mode2);
				ply->UserData[0x14]=0x28;
				ply->UserData[0xa] = 0;
			}
			CAApplyVelocity((Object *)ply);
			break;
		case 4:
			if (ud->x008a) {
			    _OinkBounce(ply);
			} else {
				if ( ((ply->BoundCheck | ply->PlatformFallDir) & (ply->Flip + 1)) == 0) {
					if(g.GPCollDetect) {
						_OinkBounce(ply);
						return;
					}
					if(--ud->x0094) {
						CAApplyVelocity((Object *)ply);
						return;
					}
				}
				ply->mode2 += 4;
				CAApplyVelocity((Object *)ply);
			}
			break;
		case 6:					// Oink hits
			CATrajectory((Object *)ply);
			if (AF2 != 2) {
				NEXT(ply->mode2);
				soundsting(SOUND_IMPACT8);
			}
			PLAYERTICK;
			break;
		case 8:
			ply->Airborne = AIR_ONGROUND;
			if (AF1) {
				ply->VelX.full = 0;
				ply->Airborne = AIR_ONGROUND;
				ud->x008a = 0;
				ply_exit_stand(ply);
			} else {
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}
static u16 _AnyButtons(Player *ply) {		// 2e38e
	return _ButtonMask(ply, BUTTON_MASK);
}

static u16 _IDFromButton(Player *ply) {		//2e3fc
	UD *ud=(UD *)&ply->UserData;
	static const char data_2e422[] = {
		0,1,2,1,3,1,2,1
	};
	if (ud->x0082 & 0x70) {
		return data_2e422[(ud->x0082 >> 4) & 7];
	} else {
		return 3 + data_2e422[(ud->x0082 >> 8) & 7];
	}
}

static struct ehondathrow sub_2e14c(Player *ply) {		//2e14c
		UD *ud=(UD *)&ply->UserData;
		short airthrow_d6 = FALSE;		// xxx
		static const char data_2e1ce[6][8]={		// redo
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{ 0, 0xe0, 0x35, 0x20, 0x10, 0,0x32, 0, },
			{ 0, 0xe0, 0x35, 0x20, 0x10, 0,0x40, 0, },
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{ 0x60, 0,    0,    0,    0, 2,0x0c, 0, },
			{ 0, 0xe0, 0x35, 0x20, 0x10, 0,0x0e, 0, },
		};
		const char *data;
		short d0;
		struct ehondathrow retval;
		
		retval.success = FALSE;
		
		if ((ply->JoyDecode.full & 4)==0) { return retval; }
		if ((ud->x0082 & 0x660) == 0)	  { return retval; }
		d0 = _IDFromButton(ply);
		if (d0 == 0)					  { return retval; }
		data = data_2e1ce[d0-1];
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
				ply->StandSquat = 0;
				return retval;
			}
			return retval;
		}
	
}
static void _EHondaCheckMoves(Player *ply, u16 d0, short initial_d2) {		//2e2be
	UD *ud=(UD *)&ply->UserData;
	const char *data, *data2;
	short d2;
	struct ehondathrow ET;
	char data_2e32e[4][6][4] = {
		{ 
			{ 0xff, 0x01, 0x00, 0x00,  },
			{ 0xff, 0x02, 0x02, 0x02,  },
			{ 0xff, 0x03, 0x04, 0x04,  },
			{ 0x39, 0x01, 0x08, 0x10,  },
			{ 0x46, 0x02, 0x0a, 0x12,  },
			{ 0x46, 0x03, 0x06, 0x14,  },
		},{ 
			{ 0xff, 0x01, 0x1c, 0x1c,  },
			{ 0xff, 0x02, 0x1e, 0x1e,  },
			{ 0xff, 0x03, 0x20, 0x20,  },
			{ 0xff, 0x01, 0x18, 0x18,  },
			{ 0xff, 0x02, 0x1a, 0x1a,  },
			{ 0xff, 0x03, 0x16, 0x16,  },
		},{ 
			{ 0xff, 0x01, 0x22, 0x22,  },
			{ 0xff, 0x02, 0x24, 0x24,  },
			{ 0xff, 0x03, 0x26, 0x26,  },
			{ 0xff, 0x01, 0x2c, 0x2c,  },
			{ 0xff, 0x02, 0x2e, 0x2e,  },
			{ 0xff, 0x03, 0x30, 0x30,  },
		},{ 
			{ 0xff, 0x01, 0x44, 0x44,  },
			{ 0xff, 0x02, 0x46, 0x46,  },
			{ 0xff, 0x03, 0x48, 0x48,  },
			{ 0xff, 0x01, 0x28, 0x28,  },
			{ 0xff, 0x02, 0x2a, 0x2a,  },
			{ 0xff, 0x03, 0x42, 0x42,  },
		},
	};
	/* next address 0002e38e */
	
	ud->x0084 = d0;
	if (d0 & 0x10) {
		d2 = 0;
	} else if (d0 & 0x20) {
		d2 = 4;
	} else if (d0 & 0x40) {
		d2 = 8;
	} else if (d0 & 0x100) {
		d2 = 0xc;
	} else if (d0 & 0x200) {
		d2 = 0x10;
	} else {
		d2 = 0x14;
	}
	data = data_2e32e[initial_d2/4][d2/4];
	data2 = data;
	if (data[0] < ply->OppXDist) {
		data2++;
	}
	ET.d0 = data2[2];
	ET.d2 = data[1];
	// 2ef7e
	if (ET.d2) {
		quirkysound(ET.d2 - 1);
	}
	_EHondaStartMoveAnim(ply, ET.d0);
}


#pragma mark Callbacks

short PLCBStandEHonda(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	
	short d0;
	struct ehondathrow ET;
	struct ehondares ER;		// d0 and d1
	
	ply->StandSquat = 0;
	ud->x0095 = 0;
	decode_buttons(ply, ud->x0082);
	if (_EHondaCheckFreeOink(ply)) {
		_EHondaStartOink(ply); 
		return TRUE; 
	}
	ER=sub_2e0c2(ply);
	if (ER.d0) { sub_2e116(ply, ER.d0); return TRUE; }
	ET = sub_2e14c(ply);
	if (ET.success) {  
		//2e3d4
		if (ET.d2) {
			quirkysound(ET.d2 - 1);
		}
		_EHondaStartMoveAnim(ply, ET.d0);
		return TRUE; }
	d0 = _AnyButtons(ply);
	if (d0) {
		//2e2a6;
		_EHondaCheckMoves(ply, d0, 0);	// Initial D2 assumed zero
		return TRUE;
	}
	return FALSE;
}
short PLCBCrouchEHonda(Player *ply) {		// 2de60
	UD *ud=(UD *)&ply->UserData;
	struct ehondares EH;
	ply->StandSquat = 2;
	short d0;
	
	EH = sub_2e0c2(ply);
	if (EH.d0) {
		return sub_2e116(ply, EH.d0);
	}
	ud->x0095 = 0;
	d0 = _AnyButtons(ply);
	if (d0) {
		_EHondaCheckMoves(ply, d0, 4);
		return TRUE;		// maybe return value from sub? prob always true.
	}
	return FALSE;
}
short PLCBJumpEHonda(Player *ply) {		//2de7c
	UD *ud=(UD *)&ply->UserData;
	short d0;
	
	ply->StandSquat = 4;
	ud->x0095 = 0;
	d0 = _AnyButtons(ply);
	if (d0) {
		if (ply->VelX.full == 0) {
			_EHondaCheckMoves(ply, d0, 8);
		} else {
			_EHondaCheckMoves(ply, d0, 12);
		}
		return TRUE;
	}
	return 0;
}
static void sub_2dda2(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	ud->x008a = 0;
	ply_exit_stand(ply);
}
static void sub_2dd08(Player *ply) {	//2dd08
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->LocalTimer = 0x82;
			random_damage_adjust_2(ply, 35);
			random_damage_adjust_1(ply, 0x23, 0x8);
			/* FALLTHRU */
		case 2:
			if (--ply->LocalTimer == 0) {
				//2dd9a
				ply_grip_release(ply, ply->Flip);
				sub_2dda2(ply);
			} else {
				if (sub_3fee(ply)) {
					ply->Timer = 1;
				}
				if (AF2==2) {
					if(ply_opp_apply_grip_damage(ply, 0, 2, ply->Flip ? 0x28 : -0x28, 0x4d, 0x47)) {
						sub_2dda2(ply);
						return;
					} 
				} else {
					if (ply_opp_apply_grip_damage(ply, 0, 2, ply->Flip ? 0x39 : -0x39, 0x40, 0x47)) {
						sub_2dda2(ply);
						return;
					}
				}
					   
					
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}

static void sub_2daae(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	if (AF2) {
		if (PSSetNextAction(ply)) {
			if (_ButtonMask(ply, ud->x0084 & BUTTON_MASK)) {
				ud->x0086 = 1;
				plstat_do_nextaction(ply);
				return;
			}
		}
	} 
	sub_2dad8(ply);
}
static void sub_2dcea(Player *ply) {
	if (AF1) {
		if ((ply->JoyDecode.full & 4) == 0) {
			ply->StandSquat = 0;
		}
		sub_2daf6(ply);
	} else {
		PLAYERTICK;
	}
}

static void _EHondaSetAnim(Player *ply, u16 d0, u16 d1) {		// 2e3be
	// XXX setaction_list((Object *)ply, actlist_3ffbc[d0], d1 / 18);		// XXX
}		

static void sub_2e3ac(Player *ply, u16 d0) {		
	// XXX setaction_direct((Object *)ply, actlist_3ffbc[d0]);		// XXX
}		
						   

static void _EHondaSMHandslap(Player *ply) {		// 2dc3a
	UD *ud=(UD *)&ply->UserData;
	const static unsigned char data_2dce2[] = {
		255, 0, 5, 10, 255, 255, 255, 255, 255, 255
	};
	int d0;
	
	switch (ply->mode2) {
		case 0:
			if (AF1) {
				NEXT(ply->mode2);
				BumpDiff_PowerMove();
				ud->x0096 = 15;
			}
			PLAYERTICK;
			break;
		case 2:
			if(g.TimeOut | g.CarWasted) {		//2dcea
				sub_2dcea(ply);
				return;
			} else if (AF2 == 0x60 && ply->Timer == 1) {
				soundsting(SOUND_HUA);		/* Hua */
			}
			if (g.RoundResult) {
				NEXT(ply->mode2);
				sub_2e3ac(ply, 0x6e);
				return;
			}
			if (check_platform_end(ply)) {
				exit_jumping2(ply);
				return;
			}
			d0 = _IDFromButton(ply);
			if (data_2dce2[d0] >= ud->x0096) {
				if (--ud->x0096 == 0) {
					NEXT(ply->mode2);
					sub_2e3ac(ply, 0x6e);
				} else {
					PLAYERTICK;
				}
			} else {
				ud->x0096 = 15;
				if (ply->Move == ((d0 * 2)+0x32)) {
					PLAYERTICK;
				} else {
					ply->Move = (d0 * 2) + 0x32;
					_EHondaSetAnim(ply, ply->Move, AF2);	// 2e3be
				}
			}
			break;
		case 4:
			sub_2dcea(ply);
			break;
			FATALDEFAULT;
	}
}
static void sub_2ddac(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			if ((ply->JoyCorrect & 1) == 0) {
				ply->EnemyDirection ^= 1;
				ply->Flip ^= 1;
			}
			break;
		case 2:
			if (AF2) {
				NEXT(ply->mode2);
				set_throw_trajectory(ply, 0, ply->Flip ^ 1, 14);
				soundsting(SOUND_HUA);
			}
			PLAYERTICK;
			break;
		case 4:
			if (AF1) {
				ply->Flip ^= 1;
				ply->XPI += ply->Flip ? 0x28 : -0x28;
				ud->x008a = 0;
				ply_exit_stand(ply);
			} else {
				PLAYERTICK;
			}
			break;
		default:
			break;
	}
}


void PLCBPowerEHonda(Player *ply) {	    //2de90
/*	if (ud->x0080 == 0) {
		Machine suicides if interrupt handler changed
	}
 */
	UD *ud=(UD *)&ply->UserData;
	
	ud->x0082 = (~ply->JoyCorrectDash & ply->JoyCorrect);
	
	_EHondaSMCheckOink(ply);
	_CheckCharge(ply, &ud->pm1, BUTTON_A, 0xf);
	_CheckCharge(ply, &ud->pm2, BUTTON_B, 0xa);
	_CheckCharge(ply, &ud->pm3, BUTTON_C, 0x5);
	if (ud->x0082 & 0x70) {
		ud->x0088 = (sf2rand() & 1) << 16 + sf2rand();
	} else {
		return;
	}
}
void PSCBAttackEHonda(Player *ply) {		/* 2da12 attack callback */
	UD *ud=(UD *)&ply->UserData;
	struct ehondares EH;
	
	if (ply->Timer2) {
		ply->Timer2--;
		ud->x008a = 1;
		return;
	}
	switch (ply->Move) {
		case 0:
		case 2:		/* 2da7c */
			EH = sub_2e0c2(ply);
			if (EH.d1) {
				sub_2e116(ply, EH.d0);
			} else if (AF2 == 0) {
				sub_2dad8(ply);
			} else if (PSSetNextAction(ply)==0) {
				sub_2dad8(ply);
			} else if (_ButtonMask(ply, ud->x0084 & BUTTON_MASK)) {
				sub_2dad8(ply);
			} else {
				ud->x0086 = 1;
				plstat_do_nextaction(ply);
			}
			break;
		case 4:	case 6: case 8: case 0xa: case 0xc:
		case 0x10: case 0x12: case 0x14: case 0x16:
		case 0x18: case 0x1a:
			sub_2dad8(ply);
			break;
		case 0xe:	case 0x40:
			sub_2dd08(ply);
			break;
		case 0x1c:	case 0x1e:
			sub_2daae(ply);
			break;
		case 0x20:
			sub_2dad8(ply);
			break;
		case 0x22: case 0x24: case 0x26: case 0x28:
		case 0x2a: case 0x2c: case 0x2e: case 0x30:
			break;
		case 0x32:
			sub_2ddac(ply);
			break;
		case 0x34:	case 0x36:	case 0x38:
			_EHondaSMHandslap(ply);
			break;
		case 0x42:	case 0x44: case 0x46: case 0x48:
			/* this is 2db24 */
			PLAYERTICK;
			break;
		case 0x3a:	case 0x3c:	case 0x3e:		//Oinks
			_EHondaSMOink(ply);
			break;
		FATALDEFAULT;
	}
}
	
#pragma mark Computer CBs

static struct ehondathrow sub_333cc(Player *ply, short airthrow_d6) {
	struct ehondathrow retval;
	
	static char data_3343e[2][3][8]={
		{
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{    0, 0, 0x50, 0x40, 0x40, 0,0x32, 0, },
			{    0, 0, 0x50, 0x40, 0x40, 0,0x40, 0, },
		}, {
			{    0, 0,    0,    0,    0, 0,   0, 0, },
			{ 0x60, 0,    0,    0,    0, 2,0x0c, 0, },
			{    0, 0, 0x50, 0x40, 0x40, 0,0x0e, 0, },
		}
	};
			
	char *data;
	
	if (ply->CompDoThrow == FALSE) { 
		retval.success= FALSE;
		return retval;
	}
	data = data_3343e[ply->PunchKick][ply->ButtonStrength];
	if (data[6] == 0) {
		retval.success = FALSE;
		return retval; 
	}
	if (data[0]) {
		if (ply->OppXDist > data[0]) {
			retval.success = FALSE;
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
	}
}
static void sub_330fc(Player *ply) {			// 330fc
	UD *ud=(UD *)&ply->UserData;

	ply->AISigAttack = FALSE;
	ply->AIVolley = FALSE;
	ud->x008a  = 0;
	exit_comp_normal(ply);
}	
static void sub_330d8(Player *ply) {	//330d8 
	UD *ud=(UD *)&ply->UserData;

	if (AF1) {//330e2 XXX refactor me
		switch (ply->StandSquat) {
			case 0:				//330fc
				ply->AISigAttack = FALSE;
				ply->AIVolley = FALSE;
				ud->x008a  = 0;
				exit_comp_normal(ply);
				break;
			case 2:				//3310e
				ply->AISigAttack = FALSE;
				ply->AIVolley = FALSE;
				ud->x008a  = 0;
				exit_to_compdisp1(ply);
				break;
			case 4:
			case 6:
				//33120
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
static void sub_330ba (Player *ply) {
	if (AF2) {
		if (comp_diceroll(ply)) {
			if (comp_setnextaction(ply)) {
				comp_do_next_action(ply);
			}
		}
	}
	sub_330d8(ply);
}
static void sub_33570(Player *ply, short sel ) {
	// XXX setaction_direct((Object *)ply, actlist_3ffbc[sel]);
}
static void sub_3314e(Player *ply) {
	NEXT(ply->mode3);
	sub_33570(ply, 0x6e);
}
static void sub_33130(Player *ply) {
	UD *ud=(UD *)&ply->UserData;

	switch (ply->mode3) {
		case 0:
			if (g.RoundResult) {
				sub_3314e(ply);
			} else {
				if (--ud->x0093) {
					// 3315a
					if (ply->AnimFlags == 0x60 &&
						ply->Timer == 1) {	/* whole word compare */
						queuesound(SOUND_HUA);
					}
					PLAYERTICK;
				} else {
					sub_3314e(ply);
				}
			}
			break;
		case 2:
			if (AF1) {
				sub_330fc(ply);	/* ehonda exit stand */
			} else {
				PLAYERTICK;
			}

		default:
			break;
	}
}
static void sub_33238(Player *ply) {
	ply->mode3 = 6;
	ply->VelX.full = ply->Flip ? -0x400 : 0x400;
	ply->AclX.full = 0xffe0;
	ply->VelY.full = 0;
	ply->AclY.full = 0;
}
static void sub_33186(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	
	switch (ply->mode3) {
		case 0:
			if (ud->x008a) {
				ply->mode3 = 8;
				ud->x008a = 0;
				return;
			} else if (AF2) {
				NEXT(ply->mode3);
				ud->x0092 = 0x28;		/* byte */
				ply->VelY.full = 0;
				ud->x008a = 0;
				ply->Airborne = AIR_JUMPING;
				queuesound(SOUND_OINK);
			}
			PLAYERTICK;
			break;
		case 2:
			if (g.GPCollDetect ) {
				sub_33238(ply);
				return;
			}
			/* FALLTHRU */
		case 4:
			if (ud->x008a) {				
				sub_33238(ply);
			} else {
				if (ply->BoundCheck == ply->Flip + 1) {
					ud->x008a = 0;
					ply->mode3 = 8;
					ply->VelX.full = 0;
					CAApplyVelocity((Object *)ply);
				} else {
					if (--ud->x0092) {
						CAApplyVelocity((Object *)ply);
					} else {
						ud->x008a = 0;
						ply->mode3 = 8;
						ply->VelX.full = 0;
						CAApplyVelocity((Object *)ply);
					}
				}
			}
			break;
		case 6:
			CATrajectory((Object *)ply);
			if (AF2 == 0x2) {
				NEXT(ply->mode2);
				queuesound(SOUND_IMPACT8);
			}
			PLAYERTICK;
			break;
		case 8:
			ply->Airborne = AIR_ONGROUND;
			if (AF1) {
				ply->VelX.full = 0;
				ud->x008a = 0;
				sub_330fc(ply);
			} else {
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_332f6(Player *ply, 	short xoff /* %d4 */, short yoff /* %d5 */) {
	if (ply->Flip != FACING_LEFT) {
		xoff = -xoff;
	}
	if(ply_opp_apply_grip_damage(ply, 0, 2, xoff, yoff, 0x47)) {
		sub_330fc(ply);
	} else {
		PLAYERTICK;
	}	
}
static void sub_3330a(Player *ply) {
	ply_grip_release(ply, ply->Flip);
	sub_330fc(ply);
}
static void sub_33272(Player *ply) {
	short xoff, yoff;
	
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->LocalTimer = 0x82;
			random_damage_adjust_2(ply, 35);
			if (AF2 == 2) {
				sub_332f6(ply, 0, 0);  // XXX assuming %d4 and %d5 are zero
				return;
			}
			random_damage_adjust_1(ply, 0xc, 8);
			/* FALLTHRU */
		case 2:
			if (--ply->LocalTimer == 0) {
				sub_3330a(ply); 
				return;
			} else {
				if(ply_opp_has_struggled_free(ply)) { sub_3330a(ply); return; }
				if (sub_3fee(ply)) { ply->Timer = 1;}
				if (AF2 == 0) {
					PLAYERTICK;
					return;
				} else if (AF2 == 2) {
					xoff = 0xffd8;
					yoff = 0x4d;
				} else {
					xoff = 0xffc7;
					yoff = 0x40;
				}
				sub_332f6(ply, xoff, yoff);
			}
			
			break;
		default:
			break;
	}
}
static void sub_33316(Player *ply) {
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->EnemyDirection = ply->IsWithinBounds ^ 1;
			ply->Flip = ply->EnemyDirection;
			break;
		case 2:
			if (AF2) {
				NEXT(ply->mode3);
				set_throw_trajectory(ply, 0, ply->Flip ^ 1, 14);
			}
			PLAYERTICK;
			break;
		case 4:
			if (AF1) {
				ply->EnemyDirection ^= 1;
				ply->Flip ^= 1;
				ply->XPI += ply->Flip ? 0x28 : -0x28 ;
				sub_330fc(ply);
			} else {
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}

static int sub_335aa(Player *ply, int d0) {
	UD *ud=(UD *)&ply->UserData;
	ud->x008a = 0;
	ud->x0090 = d0;
	ply->mode3 = 0;
	sub_33570(ply, d0);
	return 1;	
}

static int sub_33598(Player *ply, short d2, short d0) {	
	if (d2) {
		quirkysound((d2 & 0xff) - 1);
	}
	return sub_335aa(ply, d0);
}

static int sub_334ba(Player *ply, short d2) {
	const char *data;
	static const char data_33500[4][2][3][4] = {
		{ 
			{
				{ 0xff, 0x01, 0x00, 0x00,  },
				{ 0xff, 0x02, 0x02, 0x02,  },
				{ 0xff, 0x03, 0x04, 0x04,  },
			},{
				{ 0x39, 0x01, 0x08, 0x10,  },
				{ 0x46, 0x02, 0x0a, 0x12,  },
				{ 0x46, 0x03, 0x06, 0x14,  },
			}
		},{ 
			{
				{ 0xff, 0x01, 0x1c, 0x1c,  },
				{ 0xff, 0x02, 0x1e, 0x1e,  },
				{ 0xff, 0x03, 0x20, 0x20,  },
			},{
				{ 0xff, 0x01, 0x18, 0x18,  },
				{ 0xff, 0x02, 0x1a, 0x1a,  },
				{ 0xff, 0x03, 0x16, 0x16,  },
			}
		},{ 
			{
				{ 0xff, 0x01, 0x22, 0x22,  },
				{ 0xff, 0x02, 0x24, 0x24,  },
				{ 0xff, 0x03, 0x26, 0x26,  },
			},{
				{ 0xff, 0x01, 0x2c, 0x2c,  },
				{ 0xff, 0x02, 0x2e, 0x2e,  },
				{ 0xff, 0x03, 0x30, 0x30,  },
			}
		},{ 
			{
				{ 0xff, 0x01, 0x44, 0x44,  },
				{ 0xff, 0x02, 0x46, 0x46,  },
				{ 0xff, 0x03, 0x48, 0x48,  },
			},{
				{ 0xff, 0x01, 0x28, 0x28,  },
				{ 0xff, 0x02, 0x2a, 0x2a,  },
				{ 0xff, 0x03, 0x42, 0x42,  },
			}
		}, 
	};
	struct ehondathrow retval;
	data = data_33500[d2/4][ply->PunchKick/2][ply->ButtonStrength/2]; 
	if (data[0] < ply->OppXDist ) {
		retval.d0 = data[3];
	} else {
		retval.d0 = data[2];
	}

	retval.d0 = data[2];
	retval.d2 = data[1];
	return sub_33598(ply, retval.d2, retval.d0);
}

int PLCBCompJumpEHonda(Player *ply) {	//33012
	//334ac
	if (ply->VelX.full == 0) {
		return sub_334ba(ply, 8);
	} else {
		return sub_334ba(ply, 12);
	}
}	
static void sub_333b4(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	ud->x0093 = ply->AIMultiCount;
	sub_335aa(ply, (short[]){0x34, 0x36, 0x38}[ply->ButtonStrength/2]);
}
static void sub_3338a(Player *ply) {
	static const short data_333a8[3][2]={
		{0x3a, 0x0600},
		{0x3c, 0x0800},
		{0x3e, 0x0a00},
	};
	if (ply->Flip) {
		ply->VelX.full =  data_333a8[ply->ButtonStrength/2][1];
	} else {
		ply->VelX.full = -data_333a8[ply->ButtonStrength/2][1];
	}
	sub_335aa(ply, data_333a8[ply->ButtonStrength/2][0]);
}

void PLCBCompAttackEHonda(Player *ply) {		//33016
	struct ehondathrow ET;
	UD *ud=(UD *)&ply->UserData;

	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->mode3 = 0;
			ET = sub_333cc(ply, 0);
			if (ET.success == TRUE) {
				sub_33598(ply, ET.d2, ET.d0);
			} else if (ply->CompDoThrow != FALSE) {
				if (ply->PunchKick) {
					sub_3338a(ply);
				} else {
					sub_333b4(ply);
				}
			} else {
				switch (ply->StandSquat) {
					case 0:
						sub_334ba(ply, 0);
						break;
					case 2:
						sub_334ba(ply, 4);
						break;
					case 4:
						if (ply->VelX.full == 0) {
							sub_334ba(ply, 8);
						} else {
							sub_334ba(ply, 0xc);
						}
						break;
					FATALDEFAULT;
				}	
			}
			break;
		case 2:			
			if (ply->Timer2) {
				--ply->Timer2;
				ud->x008a = 1;
			} else {
				switch (ud->x0090) {
					case 0:		case 2:		case 0x1c:	case 0x1e:
						sub_330ba(ply);
						break;
					case 4:		case 6:		case 8:		case 0xa:
					case 0xc:	case 0x10:	case 0x12:	case 0x14:
					case 0x16:	case 0x18:	case 0x1a:	case 0x20:
					case 0x22:	case 0x24:	case 0x26:	case 0x28:
					case 0x2a:	case 0x2c:	case 0x2e:	case 0x30:
					case 0x42:	case 0x44:	case 0x46:	case 0x48:
						sub_330d8(ply);		 
						break;
					case 0xe:	case 0x40:
						sub_33272(ply);
						break;
					case 0x32:
						sub_33316(ply);
						break;
					case 0x34:	case 0x36:	case 0x38:
						sub_33130(ply);
						break;
					case 0x3a:	case 0x3c:	case 0x3e:
						sub_33186(ply);
						break;
					FATALDEFAULT;
				}
			}
			break;
		FATALDEFAULT;
	}
}

void PSCBVictoryEHonda(Player *ply) {		// 2e1fe
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			// still todo, this will do for now
			sub_2e3ac(ply, ((sf2rand() & 1) ? 0x6c : 0x88));
			break;
		case 2:
			ply->PSFinishedParticipating = TRUE;
			if (AF2) {
				soundsting(AF2);
			}
			PLAYERTICK;
			break;
		FATALDEFAULT;
	}
}

