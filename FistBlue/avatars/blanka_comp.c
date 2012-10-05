/*
 *  blanka_comp.c
 *  MT2
 *
 *  Created by Ben on 22/05/12.
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
#include "actions.h"
#include "playerstate.h"
#include "computer.h"
#include "sound.h"
#include "sf2io.h"

#include "lib.h"
#include "gfxlib.h"


#include "blanka.h"

typedef struct UserDataComp_Blanka UDCOMP;
extern Game g;

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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
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
static void sub_33ac8(Player *ply) {
	if (ply->Flip ^= ply->EnemyDirection) {
		ply->Flip ^= 1;
	}
}
static void sub_3383c(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
	if (--ud->pm1.a == 0) {
		ud->pm1.a = 8;
		queuesound(SOUND_ZAP1_AGAIN);
	}
	PLAYERTICK;
}
static void sub_337f2(Player *ply) {
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
	switch (ply->mode3) {
		case 0:
			ply->Airborne = AIR_JUMPING;
			if (ud->x008a) {		//337be
				sub_337be(ply);
			} else {
				ud->x0092		= 40;
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
			if (PLAYERGROUND) {
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
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
				if (ply_opp_has_struggled_2(ply)) { ply->Timer = 1;}
				if (AF2 == 0) {
					PLAYERTICK;
					_BlankaCatchSet(ply);
					return;
				} else {
					ActBlankaBiteBlood(ply);
					if(ply_opp_apply_grip_damage(ply, 0, 0, 0, 0xff00, 0x27)) {
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
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
