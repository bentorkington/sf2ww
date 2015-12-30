/*
 *  mbison.c
 *  GLUTBasics
 *
 *  Created by Ben on 27/01/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
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

#include "lib.h"
#include "gfxlib.h"

#include "mbison.h"

extern Game g;


typedef struct UserData_MBison UD;



void pl_cb_setstatus2_mbison(Player *ply, short status, int argd0) {
#ifdef REDHAMMER_EXTROM
    RHSetActionList((Object *)ply, RHOffsetLookup16(RHCODE(0x6aba0), status / 2), argd0);
#else
    setaction_list((Object *)ply, data_6aba0[status / 2], argd0);
#endif
}
void pl_cb_setstatus3_mbison(Player *ply, short status) {
	pl_cb_setstatus2_mbison(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_mbison(Player *ply, short status) {
	pl_cb_setstatus2_mbison(ply, status, 0);
}



static void sub_36616(Player *ply) {		// 34d9a
	CASetAnim2(
			   ply, 
			   (short []){0x44, 0x46, 0x48, 0x4a}[ply->StandSquat + ply->PunchKick/2],
			   ply->Move
			   );
}
static void sub_36636(Player *ply) {		// same as ChunLi etc.
	UD *ud=(UD *)&ply->UserData;
	if (ply->OppXDist < ud->x0082) {
		ply->Move = ud->x0081;
	} else {
		ply->Move = ud->x0080;
	}
	sub_36616(ply);
}
static void sub_36674(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley = FALSE;
	comp_setjumping_main(ply);
}
static void sub_36658(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley = FALSE;
	exit_comp_normal(ply);
}

static void sub_365da(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	if (AF1) {
		if (ply->ActionScript->Crouch) {
			ply->AISigAttack = FALSE;
			ply->AIVolley    = FALSE;
			exit_to_compdisp1(ply);
		} else {
			sub_36658(ply);
		}
	} else if (AF2 && ud->x0084 && comp_diceroll(ply)) {
		if(comp_setnextaction(ply)) {
			comp_do_next_action(ply);
		} else {
			quirkysound(ply->ButtonStrength/2);
			sub_36636(ply);
		}
	} else {
		PLAYERTICK;
	}
}
void sub_3623e(Player *ply) {		// 3623e cape processing
	Object *obj;
	switch (ply->UserData[5]) {
		case 0:
			NEXT(ply->UserData[5]);
			CASetAnim2(ply, STATUS_KICK, 0);
			break;
		case 2:
			if (g.x0305 == 0) {
				NEXT(ply->UserData[5]);
				CASetAnim2(ply, STATUS_KICK, 1);
				return;
			} 
			/* FALLTHRU */
		case 4:
			// 36272
			if (AF2) {
				g.x0306 = FALSE;
				if ((obj = AllocActor())) {
					obj->exists = TRUE;
					obj->Sel = SF2ACT_BISONCAPE;
					obj->XPI = ply->XPI - 8;
					obj->YPI = ply->YPI + 0x60;
					obj->Flip = ply->Flip;
					quirkysound(2);
				}
			}
			PLAYERTICK;
			break;
		default:
			break;
	}
	
}
static void sub_36782(Player *ply, short d0) {
	CASetAnim2(ply, (short []){0x50, 0x52, 0x54}[ply->ButtonStrength/2], d0);
}
static void sub_36318(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	switch (ply->ButtonStrength) {
		case 0:								// Little Punch Stand
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					MBM(0, 0, 0, 0);
					sub_36636(ply);
					quirkysound(0);
					break;
				case 2:			sub_365da(ply);		break;
				FATALDEFAULT;
			}
			break;
		case 2:							// Mid punch / throw stand
			switch (ply->mode2) {
				case 0:					
					if (ply->CompDoThrow) {
						PLY_THROW_SET(0xffe3, 0x35, 0x2c, 0x10);
						if (throwvalid(ply)) {
							ply->mode2 = 4;
							ply->Flip = ply->IsWithinBounds ^ 1;
							ply->Move = 3;
							CASetAnim2(ply, STATUS_CROUCH_PUNCH, ply->Move);
							return;
						}
					} 
					MBM(1, 1, 0, 0);
					sub_36636(ply);
					quirkysound(1);
					break;
				case 2:			sub_365da(ply);		break;
				case 4:			//363e0
					if (AF2) {
						NEXT(ply->mode2);
						set_throw_trajectory(ply, 0, ply->Flip ^ 1, 14);
					} else {
						PLAYERTICK;
					}
					break;
				case 6:		//363fe
					PLAYERTICK;
					if (AF2 == 2) {
						ply->Flip ^= 1;
						ply->EnemyDirection = ply->Flip;
					}
					break;
				case 8:
					if (AF1) {
						sub_36658(ply);
					} else {
						PLAYERTICK;
					}
				FATALDEFAULT;
			}
			break;
		case 4:							// Hi punch / throw stand
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					MBM(1, 1, 0, 0);
					sub_36636(ply);
					quirkysound(2);
					break;
				case 2:			sub_365da(ply);  break;
				FATALDEFAULT;
			}
		FATALDEFAULT;
	}
}
static int sub_369e6(Player *ply) {
	if (ply->XPI > get_scr2x() + 96) {
		return 0;
	} else if (ply->XPI < get_scr2x() + 288) {
		return 1;
	} else {
		return -1;
	}
}
static void sub_369a6(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	int temp = sub_369e6(ply);
	if (temp >= 0) {
		ply->Flip = temp ^ 1;
	}
	ud->x008c = 0;
	PLY_TRAJ0((ply->Flip ? 0x0380 : -0x380), 0x0500, 0x0005, 0x0048);
	CASetAnim2(ply, 0x56, 1);
}
static void sub_368fa(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	ud->x008c = 2;
	if (ply->YPI > 0x70) {
		PLY_TRAJ0(0x0700, 0x0800, 0x0050, 0x0080);
	} else {
		PLY_TRAJ0(0x0500, 0x0800, 0x0050, 0x0080);
	}
	if (ply->Flip) {
		ply->VelX.full = -ply->VelX.full;
		ply->VelY.full = -ply->VelY.full;
	}
	CASetAnim2(ply, 0x56, 2);
}
static void sub_36950(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	ud->x008c = 4;
	if (ply->YPI > 0x70) {
		PLY_TRAJ0(0x0700, 0x0800, 0x002c, 0x0080);
	} else {
		PLY_TRAJ0(0x0500, 0x0800, 0x002c, 0x0080);
	}
	if (ply->Flip) {
		ply->VelX.full = -ply->VelX.full;
		ply->VelY.full = -ply->VelY.full;
	}
	CASetAnim2(ply, 0x56, 3);
}
static void sub_36b12(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	ply->X.full += ply->VelX.full << 8;
	ply->Y.full += ply->VelY.full << 8;
	if (ud->x008b == 0) {
		ply->Y.full += ply->AclY.full << 8;
		ply->AclY.full -= 0x40;
		if (ply->AclY.full < 0) {
			ply->AclY.full = 0;
			++ud->x008b;
		}
	} else {
		ply->Y.full += ply->AclY.full << 8;
		ply->AclY.full += 0x40;
	}
}
static void sub_36796(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	const static short data_36818[3][8] = {
		{0x51, 0x4c, 0x47, 0x52, 0x51, 0x5a, 0x61, 0x55},
		{0x3c, 0x36, 0x30, 0x35, 0x3c, 0x39, 0x3a, 0x38},
		{0x35, 0x40, 0x41, 0x42, 0x35, 0x49, 0x57, 0x46},
	};
	int temp;
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->VelX.full = (ply->Opponent->XPI - ply->XPI) / 8;
			if (ply->Opponent->Airborne) {
				temp = 2;
			} else if (ply->Opponent->ActionScript->Crouch) {
				temp = 1;
			} else {
				temp = 0;
			}
			ply->VelY.full = (ply->Opponent->YPI + data_36818[temp][ply->Opponent->FighterID] - ply->YPI)/8;
			ply->AclY.full = 0x0400;
			ud->x0090 = 0;
			ud->x008b = 0;
			ud->x008c = 0;
			CASetAnim2(ply, 0x56, 0);
			break;
		case 2:
			if (AF2) {
				NEXT(ply->mode2);
				ply->Airborne = 1;
			}
			PLAYERTICK;
			break;
		case 4:
			sub_36b12(ply);
			if (PLAYERGROUND) {
				sub_36674(ply);
			} else {
				if (ply->BoundCheck) {
					ply->VelX.full = 0;
				}
				if (AF2) {
					NEXT(ply->mode2);
				} else {
					PLAYERTICK;
				}
			}
			break;
		case 6:
			sub_36b12(ply);
			if (PLAYERGROUND) {
				sub_36674(ply);
			} else {
				if (ply->BoundCheck) {
					ply->VelX.full = 0;
				}
				if (ud->x0090) {
					NEXT(ply->mode2);
					ply->Timer = 1;
					ply->Timer2 = 0;
					PLAYERTICK;
				}
			}
			break;
		case 8:
			if (AF1) {
				NEXT(ply->mode2);
				ply->mode3 = 0;
				if (ud->x0090 < 0 || ply->XPI < 64 || ply->Opponent->DizzyFall) {
					sub_369a6(ply);
				} else {
					temp = sub_369e6(ply);
					if (temp < 0) {
						if (0xaaaa & (1<<RAND16)) {
							sub_36950(ply);
						} else {
							sub_368fa(ply);
						}
					} else {
						if (ply->Flip == temp) {
							sub_368fa(ply);
						} else {
							sub_36950(ply);
						}
					}
				}
			} else {
				PLAYERTICK;
			}
			break;
		case 10:

			//todo
			break;
		default:
			break;
	}
	
}
static void sub_36682(Player *ply) {			// power move
	UD *ud=(UD *)&ply->UserData;
	if (ply->PunchKick) {
		sub_36796(ply);
	} else {
		//3668a
		if (ply->Timer2) {
			if (--ply->Timer2) {
				ud->x008a = ply->Timer2;
			}
		} else {
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					sub_36782(ply, 0);
					queuesound(SOUND_PLANE);
					break;
				case 2:
					if (AF1) {
						NEXT(ply->mode2);
						ply->VelY.full = 0x400;
						ply->AclY.full = 0xc0;
						ply->AclX.full = 0;
						ply->VelX.full = (short []){0x500, 0x700, 0x900}[ply->ButtonStrength/2];
						ud->x0088 = (short []){0xa0, 0xf0, 0x140}[ply->ButtonStrength/2];
						if (ply->Flip == 0) {
							ply->VelX.full = -ply->VelX.full;
						}
						ply->Airborne = AIR_JUMPING;
						ud->x0086 = ply->XPI;
						sub_36782(ply, 1);
					} else {
						PLAYERTICK;
					}
					break;
				case 4:
					if (ply->VelY.full < 0) {
						NEXT(ply->mode2);
						ply->VelY.full = 0;
						ply->AclY.full = 0;
						sub_36782(ply, 2);
					}
					CATrajectory((Object *)ply);
					break;
				case 6:
					if (ABS(ply->XPI - ud->x0086) < ud->x0088) {
						if (ply->BoundCheck != ply->Flip + 1 && ply->PlatformFallDir) {
							PLAYERTICK;
							CATrajectory((Object *)ply);
							return;
						}
					}
					NEXT(ply->mode2);
					ply->AclY.full = 0xc0;
					sub_36782(ply, 1);
					PLAYERTICK;
					CATrajectory((Object *)ply);
					break;
				case 8:
					if (PLAYERGROUND) {
						sub_36674(ply);
					} else {
						CATrajectory((Object *)ply);
					}
					break;
				FATALDEFAULT;
			}
		}
	}
}

static void sub_3645e(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	switch (ply->ButtonStrength) {
		case 0:
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->Move = 0;
					CASetAnim2(ply, STATUS_CROUCH_KICK, ply->Move);
					quirkysound(0);
					break;
				case 2:
					if (AF2 == 0) {
						NEXT(ply->mode2);
						ply->VelX.full = ply->Flip ? 0xc00 : -0xc00;
						ply->AclX.full = ply->Flip ? 0x80 : -0x80;
					}
					PLAYERTICK;
					break;
				case 4:
					find_apex(ply);
					if (ply->VelX.full == 0) {
						sub_36674(ply);
					} else {
						PLAYERTICK;
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 2:		// 364d2
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					MBM(1,1,0,0);
					sub_36636(ply);
					quirkysound(1);
					break;
				case 2:
					sub_365da(ply);
					break;
				FATALDEFAULT;
			}
		case 4:		// 36508
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->Move = 2;
					CASetAnim2(ply, STATUS_CROUCH_KICK, ply->Move);
					break;
				case 2:
					if (AF2 == 0) {
						NEXT(ply->mode2);
						ply->VelY.full = 0x200;
						ply->AclY.full = 0x30;
						ply->VelX.full = ply->Flip ? 0x800 : -0x800;
						ply->AclX.full = ply->Flip ? -0x10 : 0x10;
					}
					ply->Airborne = AIR_ONGROUND;
					PLAYERTICK;
					quirkysound(2);
					break;
				case 4:
					if (ply->VelY.full < 0) {
						if (PLAYERGROUND) {
							sub_36674(ply);
							return;
						}
					}
					PLAYERTICK;
					CATrajectory((Object *)ply);
					break;
				FATALDEFAULT;
			}
		FATALDEFAULT;
	}
}

int PLCBCompJumpMBison(Player *ply) {		//362ba
	CASetAnim2(ply, 0x4e, ply->VelX.full == 0 ? 0 : 1);
	quirkysound(ply->ButtonStrength / 2);
	return 0;
}

void PLCBCompAttackMBison(Player *ply) {
	UD *ud=(UD *)&ply->UserData;
	if (ply->AISigSpecial) {
		sub_36682(ply);
	} else {
		if (ply->Timer2) {
			if (--ply->Timer2) {
				ud->x008a = ply->Timer2;		//char
			}
		} else {
			switch (ply->StandSquat) {
				case PLY_STAND:
					switch (ply->PunchKick) {
						case 0:
							sub_36318(ply);
							break;
						case 2:
							sub_3645e(ply);
							break;
						default:
							break;
					}
					break;
				case PLY_CROUCH:
					switch (ply->mode2) {
						case 0:
							NEXT(ply->mode2);
							MBM(0,0,0,0);
							sub_36636(ply);
							quirkysound(0);
							break;
						case 2:		sub_365da(ply);	break;
						FATALDEFAULT;
					}
				FATALDEFAULT;
			}
		}
	}
}