/*
 *  ehonda_comp.c
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


#include "ehonda.h"
struct UserDataComp_EHonda {
	char	x008a;
	char	x0090;
	char	x0092;
	char	x0093;		//multicount	
};
typedef struct UserDataComp_EHonda UDCOMP;
extern Game g;

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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
	ply->AISigAttack = FALSE;
	ply->AIVolley = FALSE;
	ud->x008a  = 0;
	exit_comp_normal(ply);
}	
static void sub_330d8(Player *ply) {	//330d8 
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
	switch (ply->mode3) {
		case 0:
			if (ud->x008a) {
				ply->mode3 = 8;
				ud->x008a = 0;
				return;
			} else if (AF2) {
				NEXT(ply->mode3);
				ud->x0092 = 40;		/* byte */
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
				if (ply_opp_has_struggled_2(ply)) { ply->Timer = 1;}
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
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
	UDCOMP *ud=(UDCOMP *)&ply->UserData;
	
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


