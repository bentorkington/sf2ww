/*
 *  vega.c
 *  GLUTBasics
 *
 *  Created by Ben on 13/02/11.
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

#include "lib.h"
#include "gfxlib.h"

extern Game g;

#include "vega.h"
#include "vegadata.h"
typedef struct UserData_Vega UD;


void pl_cb_setstatus2_vega(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_76f66[status / 2], argd0);
}
void pl_cb_setstatus3_vega(Player *ply, short status) {
	pl_cb_setstatus2_vega(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_vega(Player *ply, short status) {
	pl_cb_setstatus2_vega(ply, status, 0);
}


static void sub_37554(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	ply->AISigAttack = FALSE;
	ud->x0096 = FALSE;
	ply->AIVolley = FALSE;
	exit_comp_normal(ply);
}
static void sub_37566(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	ply->AISigAttack = FALSE;
	ud->x0096 = FALSE;
	ply->AIVolley = FALSE;
	exit_to_compdisp1(ply);
}
static void sub_3758a(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	ply->AISigAttack = FALSE;
	ud->x0096 = FALSE;
	ply->AIVolley = FALSE;
	comp_setjumping_main(ply);
}
static void sub_3759c(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	ply->AISigAttack = FALSE;
	ud->x0096 = FALSE;
	PLAYERTICK;
}
static void sub_37648(Player *ply) {
	switch (ply->mode3) {
		case 0:
			if (AF2==0) {
				NEXT(ply->mode3);
				ply->Airborne = AIR_JUMPING;
				queuesound(SOUND_PUNCH3);
			}
			PLAYERTICK;
			break;
		case 2:
			if (AF2) {
				NEXT(ply->mode3);
				ply->YPI = g.GroundPlaneY;
				ply->Airborne = AIR_ONGROUND;
			}
			CATrajectory((Object *)ply);
			if (PLAYERGROUND) {
				ply->YPI = g.GroundPlaneY;
			}
			PLAYERTICK;
			break;
		case 4:
			if (AF1) {
				sub_37554(ply);
			} else {
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_37d86(Player *ply, short arg_d0) {
	setaction_direct((Object *)ply, data_7905e[arg_d0]);
}
static void sub_37a02(Player *ply) {
	NEXT(ply->mode3);
	set_throw_trajectory(ply, 0, ply->Flip, 12);
}
static struct vegathrow sub_37c12(Player *ply) {		// comp_checkthrow
	const static char data_37c72[3][8] = {
		{ 0xda, 0x35, 0x22, 0x1b, 0x1, 0x10, 0x0, 0x0,  },
		{  0x0, 0x35, 0x3c, 0x10, 0x1, 0x12, 0x0, 0x0,  },
		{ 0xe1, 0x35, 0x1e, 0x10, 0x0, 0x14, 0x0, 0x0,  },
	};
	
	struct vegathrow VT;
	const char *data;
	short d0;
	
	VT.success = FALSE;
	
	if (ply->CompDoThrow == 0) { return VT; }
	if (ply->AISigSpecial) {
		d0 = 1;
	} else if (ply->Airborne != AIR_ONGROUND) {
		d0 = 2;
	} else {
		d0 = 0;
	}
	data = data_37c72[d0];
	if (data[5]) {
		return VT;
	}
	PLY_THROW_SET(data[0], data[1], data[2], data[3]);
	if (data[4]) {
		if (airthrowvalid(ply) == FALSE) {
			return VT;
		}
	} else {
		if (throwvalid(ply) == FALSE) {
			return VT;
		}
	}
	VT.move = data[5];
	VT.success = TRUE;
	return VT;
}

static void sub_37530(Player *ply) {
	if (AF1) {
		switch (ply->StandSquat) {
			case 0: sub_37554(ply);	break;
			case 2: sub_37566(ply); break;
			case 4:
			case 6: sub_3759c(ply); break;
			FATALDEFAULT;
		}
	} else {
		PLAYERTICK;
	}
}
static void sub_375a8(Player *ply) {
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->VelY.full = 0;
			ply->AclY.full = 0;
			ply->VelX.full = (short []){0xf600, 0x600}[ply->Flip];
			ply->AclX.full = (short []){0xff80, 0x80}[ply->Flip];
			PLAYERTICK;
			break;
		case 2:
			if (AF2==0 || ply->VelX.full == 0) {
				if (AF1) {
					sub_37554(ply);
				} else {
					PLAYERTICK;
				}
			} else {
				CATrajectory((Object *)ply);
			}
			break;
		FATALDEFAULT;
	}
	
}
static void sub_376ac(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	switch (ply->mode3) {
		case 0:
			if (AF2) {
				NEXT(ply->mode3);
				ply->Airborne = AIR_JUMPING;
			}
			PLAYERTICK;
			break;
		case 2:
			if (AF2 == 0) {
				NEXT(ply->mode3);
			}
			CATrajectory((Object *)ply);
			PLAYERTICK;
			break;
		case 4:
			if (AF1) {
				NEXT(ply->mode3);
				ply->Scroll = 2;
				ply->DSOffsetX = 0;				
			}
			PLAYERTICK;
			break;
		case 6:
			if (AF1) {
				ply->mode3 = 0;
				ud->x0090 = 0x1a;
				sub_37d86(ply, 0x1a);
			} else {
				ply->DSOffsetX = 0;
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_379ae(Player *ply) {
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->Flip = ply->EnemyDirection = ply->IsWithinBounds;
			/* FALLTHRU */
		case 2:
			if (AF2 == 0) {
				NEXT(ply->mode3);
			}
			PLAYERTICK;
			break;
		case 4:
			if (AF2) {
				sub_37a02(ply);	// set throw traj
			} else {
				CATrajectory((Object *)ply);
				if (PLAYERGROUND) {
					ply->CompDoJump = FALSE;
					sub_37d86(ply, 0x50);
					sub_37a02(ply);
				} else {
					PLAYERTICK;
				}
			}
			break;
		case 6:
			CATrajectory((Object *)ply);
			if (PLAYERGROUND) {
				ply->CompDoJump = FALSE;
				sub_3758a(ply);
			} else {
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_37b0c(Player *ply) {
	CATrajectory((Object *)ply);
	if (ply->VelY.full < 0) {
		NEXT(ply->mode3);
	}
	PLAYERTICK;
}	
static void sub_37a2a(Player *ply) {
	struct vegathrow VT;
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->CompDoThrow = TRUE;
			ply->ButtonStrength = 2;
			ply->PunchKick = PLY_PUNCHING;
			sub_37d86(ply, 0x4e);
			ply->Flip = (ply->VelX.full < 0) ? 0 : 1;
			queuesound(SOUND_VEGA_JUMP);
			/* FALLTHRU */
		case 2:
			ply->CompImmune = ply->VelY.full < 0 ? 0 : 2;
			CATrajectory((Object *)ply);
			if (PLAYERGROUND) {
				sub_3758a(ply);
			} else if (AF2) {
				PLAYERTICK;
			} else {
				VT = sub_37c12(ply);
				if (VT.success == FALSE) {
				PLAYERTICK;
				} else {
					ply->mode3 += 6;
					ply->Flip = ply->EnemyDirection = ply->IsWithinBounds ^ 1;
					if (ply->YPI < 0x40) {
						ply->YPI = g.GroundPlaneY;
						NEXT(ply->mode3);
						ply->VelX.full = ply->Flip < 0 ? 0x10: -0x10;
						ply->AclX.full = 0;
						ply->VelY.full = 0x600;
						ply->AclY.full = 0x80;
						ply->LocalTimer = 8;
					}
					sub_37d86(ply, 0x12);
				}
			}
			break;
		case 4:
			if (--ply->LocalTimer == 0) {
				NEXT(ply->mode3);
				sub_37b0c(ply);
			}
			break;
		case 6:
			sub_37b0c(ply);
			break;
		case 8:
			ply->VelX.full   = 0;
			ply->AclX.full   = 0;
			ply->VelY.full   = 0;
			if (AF2) {
				NEXT(ply->mode3);
			} else {
				PLAYERTICK;
			}
			break;
		case 10:
			if (PLAYERGROUND) {
				NEXT(ply->mode3);
				ply->YPI = g.GroundPlaneY;
				sub_36d6(ply, ply->Flip ? 0x40 : -0x40, 0, 2, ply->Flip, 13, 0x2e, 2);
				ActStartScreenWobble();
			} else {
				CATrajectory((Object *)ply);
			}
			break;
		case 12:
			if (AF1) {
				ply->EnemyDirection = ply->Flip;
				sub_37554(ply);
			} else {
				PLAYERTICK;
			}			
			break;
		default:
			break;
	}
}


static void sub_37daa(Player *ply, short arg_d0) {
	ply->Move = arg_d0;
	sub_37d86(ply, arg_d0);
}
static void sub_37d98(Player *ply, short arg_d2, short arg_d0) {
	if (arg_d2) {
		quirkysound((arg_d2 & 0xff)-1);		
	}
	sub_37daa(ply, arg_d0);
}
static void sub_37cc6(Player *ply) {
	switch (ply->StandSquat) {
		case 0:
			sub_37554(ply);
			break;
		case 2:
			sub_37566(ply);
			break;
		case 4:
			sub_3759c(ply);
			break;
		FATALDEFAULT;
	}
}

static int sub_37ca2(Player *ply, short arg_d2) {
	const static char data_37cea[4][12] = {
		{ 0x1, 0x2, 0x2, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4,  },
		{ 0x1, 0x6, 0x1, 0x6, 0x1, 0x6, 0x1, 0x8, 0x2, 0xa, 0x1, 0x8,  },
		{ 0x1, 0xc, 0x1, 0xc, 0x1, 0xc, 0x1, 0xc, 0x1, 0xc, 0x1, 0xc,  },
		{ 0x1, 0xe, 0x1, 0xe, 0x1, 0xe, 0x1, 0xe, 0x1, 0xe, 0x1, 0xe,  },
	};
	
	const char *data;
	data = data_37cea[arg_d2 / 4];
	data += ply->ButtonStrength;
	data += ply->PunchKick * 6;
	if (data[1] == 0) {
		sub_37cc6(ply);
		return 0;
	} else {
		sub_37d98(ply, data[1], data[0]);
		return 1;
	}
}


static void sub_378a4(Player *ply) {
	NEXT(ply->mode3);
	ply->VelY.full = 0;
	ply->VelX.full = 0;
	ply->AclX.full = 0;
	sub_37d86(ply, 0x52);
}
static void sub_3771e(Player *ply) {
	UD *ud=(UD *)&ply->UserData;

	const static char data_377b0[2][16] = {
		{ 0x20, 0x24, 0x28, 0x2c, 0x20, 0x24, 0x28, 0x2c, 0x20, 0x24, 0x28, 0x2c, 0x20, 0x24, 0x28, 0x2c,  },
		{ 0x0, 0x4, 0x8, 0xc, 0x10, 0x14, 0x18, 0x1c, 0x0, 0x4, 0x8, 0xc, 0x10, 0x14, 0x18, 0x1c,  },
	};
	const static short data_377d0[12][2] = {
		{   -80,   112,  },
		{   -80,    88,  },
		{   -80,    64,  },
		{   -80,    40,  },
		{    80,   112,  },
		{    80,    88,  },
		{    80,    64,  },
		{    80,    40,  },
		{     0,   112,  },
		{     0,    88,  },
		{     0,    64,  },
		{     0,    40,  },
	};
	const short *data;
	short d0, d3, d4;
	
	switch (ply->mode3) {
		case 0:
			ply->DSOffsetX = 0;
			NEXT(ply->mode3);
			if (ply->PunchKick == 2) {
				data = data_377d0[data_377b0[0][RAND16]/4];
			} else {
				data = data_377d0[data_377b0[1][RAND16]/4];
			}
			ud->x0092 = ply->Opponent->XPI + data[0];
			ud->x0094 = 0x28 + data[1];
			d4 = ABS(ud->x0092 - ply->XPI);
			if (d4 >= 0x80) {
				d0 = 0x80;
				d3 = 1;
			} else if (d4 >= 0x40) {
				d0 = 0x40;
				d3 = 2;
			} else {
				d0 = 0x20;
				d3 = 3;
			}
			ply->LocalTimer = d0;
			ply->VelX.full = (ud->x0092 - ply->XPI) << d3;
			ply->VelY.full = (ud->x0094 - ply->YPI) << d3;
			break;
		case 2:
			ply->DSOffsetX = 0;
			if (g.RoundResult) {
				sub_378a4(ply);
			} else {
				if (--ply->LocalTimer == 0) {
					NEXT(ply->mode3);
					sub_37d86(ply, 0x20);
				}
				CAApplyVelocity((Object *)ply);
				PLAYERTICK;
			} 
			break;
		case 4:
			ply->DSOffsetX = 0;
			if (g.RoundResult) {
				sub_378a4(ply);
			} else if (AF1) {
				ply->Attacking = TRUE;
				ply->VelX.full = (ply->Opponent->XPI - ply->XPI) << 3;
				ply->VelY.full = (ply->Opponent->YPI - ply->XPI) << 3;
				ply->AclX.full = 0;
				ply->AclY.full = 0x140;
				ply->VelY.full += 0x1400;
				ply->Scroll = 0;
				ply->mode3 = 0;
				ply->Move = ply->PunchKick == 2 ? 0x1e : 0x12;
			}
			ply->CompImmune = 2;
			PLAYERTICK;
			break;
		case 6:
			CATrajectory((Object *)ply);
			if (PLAYERGROUND) {
				sub_3758a(ply);
			}
			break;
		default:
			break;
	}
}
static void sub_37982(Player *ply) {
	ply->CompImmune = 2;
	if (ply->VelY.full < 0) {
		ply->CompImmune = 0;
		if (PLAYERGROUND) {
			sub_3758a(ply);
			return;
		}
	}
	
		
		if (AF2 == 0) {
			CATrajectory((Object *)ply);
		}
		PLAYERTICK;

}
static void sub_378c2(Player *ply) {
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->Attacking = TRUE;
			ply->VelX.full = (ply->Opponent->XPI - ply->XPI) << 3;
			ply->VelY.full = (ply->Opponent->YPI - ply->XPI) << 3;
			ply->AclX.full = 0;
			ply->AclY.full = 0x140;
			ply->VelY.full += 0x1400;
			ply->Flip = ply->VelX.full < 0 ? 0 : 1;
			queuesound(SOUND_VEGA_JUMP);
			ply->CompImmune = 2;
			sub_37d86(ply, 0x1e);
			break;
		case 2:
			ply->CompImmune = 2;
			if (AF1 == 0) {
				sub_37982(ply);
			} else if (ply->VelY.full >= 0) {
				sub_37982(ply);
			} else {
				ply->CompImmune = 0;
				if (ABS(ply->Opponent->XPI - ply->XPI) >= 0x40) {
					sub_37982(ply);
				} else if (ABS(ply->Opponent->XPI - ply->XPI) >= 0x80) {
					sub_37982(ply);	// ???
				} else {
					NEXT(ply->mode3);
					setaction_direct((Object *)ply, ply->ActionScript + sizeof(struct action));
				}
			}
			break;
		case 4:
			sub_37982(ply);
		default:
			break;
	}
}
static void sub_37b8e(Player *ply) {
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->Flip = ply->EnemyDirection = ply->IsWithinBounds ^ 1;
			ply->Flip ^= 1;
			queuesound(SOUND_GINK);
			/* FALLTHRU */
		case 2:
			if (AF2) {
				NEXT(ply->mode3);
				sub_36d6(ply, ply->Flip ? 0x40 : -0x40, 0, 2, ply->Flip, 12, 0x2e, 2);
				ActStartScreenWobble();
			} else {
				PLAYERTICK;
			}
			break;
		case 4:
			if (AF1) {
				ply->EnemyDirection = ply->Flip;
				sub_37554(ply);
			} else {
				PLAYERTICK;
			}
			break;
		FATALDEFAULT;
	}
	
}
static void sub_374da(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		//374e6
		switch (ud->x0090) {
			case 0:
				sub_3759c(ply);		break;
			case 2:		case 4:		case 6:		case 8:
			case 0xc:	case 0xe:
				sub_37530(ply);
				break;
			case 0xa:
				sub_375a8(ply);
				break;
			case 0x10:
				sub_379ae(ply);
				break;
			case 0x12:
				sub_37a2a(ply);
				break;
			case 0x14:
				sub_37b8e(ply);
				break;
			case 0x16:
				sub_37648(ply);
				break;
			case 0x18:
				sub_376ac(ply);
				break;
			case 0x1a:
				sub_3771e(ply);
				break;
			case 0x1c:
				sub_37554(ply);
				break;
			case 0x1e:
				sub_378c2(ply);
				break;
			FATALDEFAULT;
		}
	}
}
static void sub_37d1a(Player *ply) {
	switch (ply->PunchKick) {
		case 0:
			ply->VelX.full = ply->Flip ? 0x500 : -0x500;
			ply->AclX.full = 0;
			ply->VelY.full = 0x800;
			ply->AclY.full = 0x100;
			sub_37daa(ply, 0x16);
			break;
		case 2:
		case 4:
			ply->VelX.full = ply->Flip ? 0x380 : -0x380;
			ply->AclX.full = ply->Flip ? 5 : -5;
			ply->VelY.full = 0x800;
			ply->AclY.full = 0x58;
			ply->Attacking = FALSE;
			sub_37daa(ply, 0x18);
			break;
		FATALDEFAULT;
	}
}

static int sub_37c94(Player *ply) {
	if (ply->VelX.full == 0) {
		return sub_37ca2(ply, 8);
	} else {
		return sub_37ca2(ply, 12);
	}	
}
		//37498
void PLCBCompAttackVega(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	struct vegathrow VT;
	if (ud->x0096) {
		sub_374da(ply);
	} else {
		switch (ply->mode2) {
			case 0:
				NEXT(ply->mode2);
				ply->mode3 = 0;
				VT = sub_37c12(ply);
				if(VT.success) {
					sub_37daa(ply, VT.move);
				} else  if (ply->AISigSpecial) {
					sub_37d1a(ply);
				} else {
					switch (ply->StandSquat) {
						case 0:				//sub_37c8a(ply);
							sub_37ca2(ply, 0);
							break;
						case 2:
							sub_37ca2(ply, 4);
							break;
						case 4:
							sub_37c94(ply);
							break;
						FATALDEFAULT;
					}
				}

				break;
			case 2:
				sub_374da(ply);
				break;
			FATALDEFAULT;
		}
	}
}

static int sub_37624(Player *ply) {
	short y = ply->YPI - 0x28;
	
	if (y < 0 || y >= 0x40) {
		return TRUE;
	}
	return FALSE;
}

static int sub_37602(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	struct vegathrow vt;
	if (sub_37624(ply) == 0) {
		return FALSE;
	} else {
		vt = sub_37c12(ply);
		if (vt.success) {
			sub_37daa(ply, vt.move);
			ply->mode1 = 0xa;
			ud->x0096 = 1;
			return -1;
		}
	}
	return 0;
}
int PLCBCompJumpVega (Player *ply) {		// 37486
	if (ply->CompDoThrow == 0) {
		return sub_37c94(ply);
	} else {
		if (sub_37602(ply) == 0) {
			return sub_37c94(ply);
		}
	}
	return 0;
}