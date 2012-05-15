/*
 *  dhalsim.c
 *  GLUTBasics
 *
 *  Created by Ben on 7/11/10.
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
#include "playerstate.h"

#include "computer.h"

#include "lib.h"
#include "gfxlib.h"


#include "dhalsim.h"
#include "dhalsimdata.h"

extern Game g;

int g_dhalsim_d6;

struct dhalsim_powermove {
	signed char mode0;
	signed char mode1;
	signed char x0002;
	signed char x0003;
};
typedef struct dhalsim_powermove DM;

struct UserData_Dhalsim {
	signed char x0080;
	DM			x0090;
	DM			x0098;
	signed char x00c0;
	signed char x00c2;
	int			x00f0;
	int			x00f4;
};

typedef struct UserData_Dhalsim UD;

void pl_cb_setstatus2_dhalsim(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_5de06[status / 2], argd0);
}
void pl_cb_setstatus3_dhalsim(Player *ply, short status) {
	pl_cb_setstatus2_dhalsim(ply, status, ply->Step ^ ply->Flip);
}
void pl_cb_setstatus1_dhalsim(Player *ply, short status) {
	pl_cb_setstatus2_dhalsim(ply, status, 0);
}

int sub_32558(Player *ply, DM *dm) {
	UD *ud = (UD *)&ply->UserData;
	ply->mode1 = 12;
	ply->mode2 = 0;
	dm->x0002 = 0;
	ud->x00c2 = 5;
	BumpDiff_PowerMove();
	++g.HumanMoveCnt;
	return -1;
}
int sub_32414(Player *ply) {		// crouch
	ply->Move = ply->ButtonStrength;
	if (ply->PunchKick) {
		if (ply->OppXDist > (short []){0x32,0x3c,0x33}[ply->ButtonStrength]) {
			++ply->Move;
		}
	} else {
		if (ply->OppXDist > (short []){0x2f,0x2f,0x29}[ply->ButtonStrength]) {
			++ply->Move;
		}
	}
	ply->StandSquat = 2;
	return 1;
}
int sub_32328(Player *ply) {		// stand
	ply->Move = ply->ButtonStrength;
	if (ply->PunchKick) {
		ply->Move += ply->ButtonStrength;
		if (ply->OppXDist > (short []){0x33,0x38,0x42}[ply->ButtonStrength]) {
			ply->Move += 2;
		}
		if (ply->JoyDecode.full & 0xb) {
			++ply->Move;
		}
	} else {
		if (ply->OppXDist> (short []){0x33,0x30,0x3c}[ply->ButtonStrength]) {
			ply->Move += 2;
		}
	}
	ply->StandSquat = 0;
	return 1;
}	
int sub_32512(Player *ply) {
	UD *ud = (UD *)&ply->UserData;

	if (ply->Projectile == NULL && ply->PunchKick == PLY_PUNCHING) {
		if (0x5555 & (1<<RAND32)) {
			ud->x00c0 = 2;
			return sub_32558(ply, &ud->x0098);
		} else {
			ud->x00c0 = 0;
			return sub_32558(ply, &ud->x0090);
		}
	} else {
		if (ply->StandSquat) {
			return sub_32414(ply);
		} else {
			return sub_32328(ply);
		}
	}
}
int sub_32386(Player *ply) {
	g_dhalsim_d6 &= 3;
	if (g_dhalsim_d6 & ply->PunchKick==0 &ply->ButtonStrength != 0) {
		ply->Throw[0]=0xffe0;
		ply->Throw[1]=0x0035;
		ply->Throw[1]=0x0020;
		ply->Throw[3]=0x0010;
		if(throwvalid(ply)) {
			return 1;
		}
	}
	return 0;
}

static int sub_3257a(Player *ply) {
	g_dhalsim_d6 = ply->JoyDecode.full;
	return (~ply->JoyDecodeDash.full & ply->JoyDecode.full & BUTTON_MASK);
}
static void sub_32480(Player *ply, int move) {
	ply->Move = move;
	if (ply->PunchKick) {
		CASetAnim2(ply, 0x4a, move);
	} else {
		CASetAnim2(ply, 0x48, move);
	}
}
static void sub_32a58(Player *ply, DM *dm, int buttons) {
	UD *ud = (UD *)ply->UserData;
	if (ply->Projectile == NULL && check_special_ability(ply) && ud->x00c2) {
		decode_buttons(ply, buttons);
		if (dm->mode1 > ply->ButtonStrength) {
			dm->x0002 = 1;
			ply->Attacking = TRUE;
		}
	}
}
static void sub_329aa(Player *ply, const short *arg1, const char *arg2, DM *dm) {
	int buttons;
	switch (dm->mode0) {
		case 0:
			if ((ply->JoyCorrect2 & 0xf) == arg1[0]) {
				NEXT(dm->mode0);
				dm->x0003 += 2;
				dm->mode1 = arg2[RAND32];
			} else {
				dm->mode0 = 0;
				dm->x0003 = 0;
				dm->x0002 = 0;
			}
			break;
		case 2:
			if (--dm->mode1 == 0) {
				dm->mode0 = 0;
				dm->x0003 = 0;
				dm->x0002 = 0;
			} else {
				if ((ply->JoyCorrect2 & 0xf) == arg1[dm->x0003 / 2]) {
					if (arg1[dm->x0003] & BUTTON_MASK) {
						if(buttons = buttonspressed(ply, arg1[dm->x0003 / 2])){
							sub_32a58(ply, dm, buttons);
							return;
						}
						if (buttons = buttonsreleased(ply, arg1[dm->x0003 / 2])) {
							sub_32a58(ply, dm, buttons);
							return;
						}
						NEXT(dm->mode0);
					} 
					dm->x0003 += 2;
					dm->mode1 = arg2[RAND32];
				}
			}
			break;
		case 4:
			if (--dm->mode1 == 0) {
				dm->mode0 = 0;
				dm->x0003 = 0;
				dm->x0002 = 0;
			} else {
				if(buttons = buttonspressed(ply, arg1[dm->x0003 / 2])){
					sub_32a58(ply, dm, buttons);
					return;
				}
				if (buttons = buttonsreleased(ply, arg1[dm->x0003 / 2])) {
					sub_32a58(ply, dm, buttons);
					return;
				}
			}
			break;
		FATALDEFAULT;
	}
}

static void sub_32afa(Player *ply, DM *dm) {
	UD *ud = (UD *)ply->UserData;
	ply->mode1 = 0xc;
	ply->mode2 = 0;
	dm->x0002 = 0;
	ud->x00c2 = 5;
	BumpDiff_PowerMove();
	++g.HumanMoveCnt;
}
static void sub_3278c(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	ply->X.full += (ply->Path[ud->x0080].x.full << 8);
	ply->Y.full += (ply->Path[ud->x0080].y.full << 8);
}
static void sub_32664(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	if (AF1) {
		ud->x00c2 = 0;
		ply_exit_crouch(ply);
	}	
}
void PSCBAttackDhalsim(Player *ply) {			// 3258e
	UD *ud = (UD *)ply->UserData;
	// suicide code removed
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		if (ud->x00c2) {
			--ud->x00c2;
		}
		switch (ply->StandSquat) {
			case 0:
				if (ply->mode2) {
					if (AF1) {
						ud->x00c2 = 0;
						ply_exit_stand(ply);
					}
				} else {
					quirkysound(ply->ButtonStrength / 2);
					setstatus4(ply, (ply->PunchKick ? 0x42 : 0x40));
				}
				break;
			case 2:
				if (ply->PunchKick == PLY_PUNCHING) {
					switch (ply->mode2) {
						case 0:
							quirkysound(ply->ButtonStrength / 2);
							setstatus4(ply, 0x44);
							break;
						case 2:
							sub_32664(ply);
							break;
						FATALDEFAULT;
					}
					actiontick((Object *)ply);
				} else {
					switch (ply->Move) {
						case 0:
						case 2:
							switch (ply->mode2) {
								case 0:
									quirkysound(ply->ButtonStrength / 2);
									setstatus4(ply, 0x46);
									break;
								case 2:
									sub_32664(ply);
									break;
								FATALDEFAULT;
							} // ply->mode2
							break;
						case 1:
						case 3:
						case 4:
						case 5:
							switch (ply->mode2) {
								case 0:
									NEXT(ply->mode2);
									ud->x00f4 = 0xffff0000;
									ud->x00f0 = (int []){0xa0000, 0xc0000, 0xe0000}[ply->ButtonStrength / 2];
									quirkysound(ply->ButtonStrength / 2);
									CASetAnim2(ply, 0x46, ply->Move);
									break;
								case 2:
									if (AF2) {
										NEXT(ply->mode2);
									}
									actiontick((Object *)ply);
									break;
								case 4:
									if (ply->Flip) {
										ply->X.full += ud->x00f0;
									} else {
										ply->X.full -= ud->x00f0;
									}
									ud->x00f0 += ud->x00f4;
									if (ud->x00f0 < 0) {
										NEXT(ply->mode2);
									}
									if ((g.libsplatter + ply->Side) & 3 && 
										ply->OnPlatform2 && ply->OnPlatform == FALSE) {
										ply->OnPlatform2 = FALSE;
										exit_jumping2(ply);
									}
									break;
								case 6:
									if (AF1) {
										ud->x00c2 = 0;
										ply_exit_crouch(ply);
									} else {
										actiontick((Object *)ply);
									}
									break;
								FATALDEFAULT;
							}
							break;
						FATALDEFAULT;
					}
				}
				break;
			case 4:
				sub_3278c(ply);
				if (check_ground_collision((Object *)ply)) {
					ply->Jumping = FALSE;
					soundsting(0x2f);
					ply_exit_air(ply);
				} else {
					actiontick((Object *)ply);
				}
				break;
			case 6:
				
				break;
			FATALDEFAULT;
		}
	}
}

int PLCBStandDhalsim(Player *ply) {		// 32302
	UD *ud = (UD *)ply->UserData;
	int buttons;
	if (buttons = sub_3257a(ply)) {
		ply->StandSquat = 0;
		++g.HumanMoveCnt;
		decode_buttons(ply, buttons);
		if (LBRareChance()) {
			return sub_32512(ply);
		} else if (sub_32386(ply)) {
			ply->StandSquat = PLY_THROW;
			ud->x00c2 = 0;
			ply->Move = ply->ButtonStrength / 4;
			return 1;
		} else {
			return sub_32328(ply);
		}
	}
	return 0;
}
int PLCBCrouchDhalsim(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	int buttons;
	ud->x00c2 = 5;
	if (buttons = sub_3257a(ply)) {
		++g.HumanMoveCnt;
		decode_buttons(ply, buttons);
		ply->StandSquat = PLY_CROUCH;
		if (LBRareChance()) {
			return sub_32512(ply);
		} else {
			return sub_32414(ply);
		}
	}
	return 0;
}
int PLCBJumpDhalsim(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	int buttons;
	ud->x00c2 = 5;
	if (buttons = sub_3257a(ply)) {
		++g.HumanMoveCnt;
		decode_buttons(ply, buttons);
		if (ply->ButtonStrength >= 3) {
			if (ply->VelY.full < 256 && ply->VelY.full >= -256) {
				ply->StandSquat = 4;
				ply->Move = ply->ButtonStrength + 1;
				ply->Path = (VECT16 *)data_cfaf4;
				if (ply->PunchKick) {
					CASetAnim2(ply, 0x4a, ply->ButtonStrength + 1);
					if (ply->Flip) {
						ud->x0080 =  0xb;
					} else {
						ud->x0080 = 0x15;
					}
				} else {
					CASetAnim2(ply, 0x48, ply->ButtonStrength + 1);
					if (ply->Flip) {
						ud->x0080 =	 0xa;
					} else {
						ud->x0080 = 0x16;
					}					
				}
				ply->mode1 = 0xa;
				ply->mode2 = 0;
				ply->mode3 = 0;
				return -1;
			}
			sub_32480(ply, (ply->ButtonStrength + 1) & 0xfe);
			return 1;
		} else {
			sub_32480(ply, ply->ButtonStrength + 1);
			return 1;
		}
	}
	return 0;
}
int PLCBPowerDhalsim(Player *ply) {
	UD *ud = (UD *)ply->UserData;

	static const short data_32aa6[] = {0x0004, 0x0006, 0x0002, 0x0070 };
	static const short data_32aae[] = {0x1, 0x5, 0x4, 0x6, 0x2, 0x70};
	static const char data_32aba[] = {
		0x0A, 0x08, 0x08, 0x0B, 0x08, 0x08, 0x09, 0x08, 0x09, 0x08, 0x08, 0x0A, 0x08, 0x08, 0x0B, 0x0D, 
		0x08, 0x0F, 0x09, 0x08, 0x0A, 0x0C, 0x08, 0x08, 0x0B, 0x08, 0x09, 0x08, 0x0E, 0x0C, 0x08, 0x0A,
	};
	static const char data_32ada[] = {
		0x0A, 0x08, 0x08, 0x0B, 0x08, 0x08, 0x09, 0x08, 0x09, 0x08, 0x08, 0x0A, 0x08, 0x08, 0x0B, 0x0D, 
		0x08, 0x0F, 0x09, 0x08, 0x0A, 0x0C, 0x08, 0x08, 0x0B, 0x08, 0x09, 0x08, 0x0E, 0x0C, 0x08, 0x0A,
	};

	
	if (ply->PSFinishedParticipating == FALSE) {
		sub_329aa(ply, data_32aae, data_32ada, &ud->x0098);
		if (ud->x0098.x0002) {
			ud->x00c0 = 2;
			sub_32afa(ply, &ud->x0098);
		}
		sub_329aa(ply, data_32aa6, data_32aba, &ud->x0090);
		if (ud->x0090.x0002) {
			ud->x00c0 = 0;
			sub_32afa(ply, &ud->x0090);
		}
	}
	return 0;	// always returns 0
}
static short sub_35f7e(Player *ply, int d0) {
	UD *ud = (UD *)ply->UserData;
	
	ply->StandSquat = 4;	// SS_AIRMOVE
	ply->Move = d0;
	ply->Path = (VECT16 *)data_cfaf4;
	if (ply->PunchKick) {
		// 35fb6 
		CASetAnim2(ply, STATUS_JUMP_KICK, d0);
		if(ply->Flip != FACING_LEFT){
			ud->x0080 = 0xb;
		} else {
			ud->x0080 = 0x15;
		}
	}
	else {
		// 35f96
		CASetAnim2(ply, STATUS_JUMP_PUNCH, d0);
		if(ply->Flip != FACING_LEFT){
			ud->x0080 = 0xa;
		} else {
			ud->x0080 = 0x16;
		}
	}
	// 35fd4
	ply->mode1 = 0xa;
	ply->mode2 = 0;
	ply->mode3 = 0;
	return -1;
}

short sub_35fe6(Player *ply) {
	if(ply->VelY.full + 0x100 < 0x200) {
		return sub_35f7e(ply, 5);
	}
	return 0;
}