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
#include "sound.h"
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
	signed char x0082;
	DM			x0090;
	DM			x0098;
	signed char x00c0;
	signed char x00c2;
	short		x00d0;
	int			x00f0;
	int			x00f4;
};

typedef struct UserData_Dhalsim UD;

void pl_cb_setstatus2_dhalsim(Player *ply, short status, int argd0) {
	setaction_list((Object *)ply, data_64fbe[status / 2], argd0);
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
		PLY_THROW_SET(0xffe0, 0x0035, 0x0020, 0x0010);
		if(throwvalid(ply)) {
			return 1;
		}
	}
	return 0;
}

static int sub_3257a(Player *ply) {
	g_dhalsim_d6 = ply->JoyDecode.full;
	return (PLY_NEWBUTTONS & BUTTON_MASK);
}
static void sub_32480(Player *ply, int move) {
	ply->Move = move;
	if (ply->PunchKick) {
		CASetAnim2(ply, STATUS_JUMP_KICK, move);
	} else {
		CASetAnim2(ply, STATUS_JUMP_PUNCH, move);
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
static void sub_3262c(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	ud->x00c2 = 0;
	ply_exit_stand(ply);
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
					setstatus4(ply, (ply->PunchKick ? STATUS_KICK : STATUS_PUNCH));
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
					PLAYERTICK;
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
									CASetAnim2(ply, STATUS_CROUCH_KICK, ply->Move);
									break;
								case 2:
									if (AF2) {
										NEXT(ply->mode2);
									}
									PLAYERTICK;
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
										PLAYERTICK;
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
				if (PLAYERGROUND) {
					ply->Jumping = FALSE;
					soundsting(SOUND_IMPACT8);
					ply_exit_air(ply);
				} else {
					PLAYERTICK;
				}
				break;
			case 6:
				if (ply->ButtonStrength == 4) {
					if (ply->mode2 == 0) {
						NEXT(ply->mode2);
						ply->mode3 = 0;
						if (ply->JoyDecode.full & 1) {
							ply->Flip = 0;
						} else {
							ply->Flip = 1;
						}
						ply->Opponent->Flip = ply->Flip ^ 1;
						CASetAnim2(ply, 0x50, ply->Move);
					} else {
						if (ply->mode3 == 0) {
							if (AF2) {
								NEXT(ply->mode3);
								set_throw_trajectory(ply, 0, ply->Flip ^ 1, 12);
							}
							PLAYERTICK;
						} else {
							if (AF1) {
								ply->Flip ^= 1;
								ply->EnemyDirection = ply->Flip;
								sub_3262c(ply);
							} else {
								PLAYERTICK;
							}
						}
					}
				} else {
					if (ply->mode2 == 0) {
						random_damage_adjust_2(ply, 0x25);
						random_damage_adjust_2(ply, 0xc);	
						ply->LocalTimer = 0xb4;
						setstatus4(ply, 0x50);
					} else {
						if (--ply->LocalTimer == 0) {
							ply_grip_release(ply, ply->Flip);
							sub_3262c(ply);
						} else {
							if(ply_opp_has_struggled_free(ply)) {
								ply_grip_release(ply, ply->Flip);
								sub_3262c(ply);
							} else {
								if (sub_3fee(ply)) {
									ply->Timer = 1;
								}
								if (AF2) {
									if (ply_opp_apply_grip_damage(ply, 0, 2, ply->Flip ? 32 : -32, 0x56, 0x29)) {
										sub_3262c(ply);
									} else {
										PLAYERTICK;
									}
								} else {
									PLAYERTICK;
								}
							}
						}
					}
				}
				break;
			FATALDEFAULT;
		}
	}
}

static void sub_32b3c(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	NEXT(ply->mode2);
	soundsting(SOUND_YOGA);
	ud->x00d0 = 0xa;
	if (ud->x00c0) {
		CASetAnim2(ply, 0x4c, (ply->ButtonStrength / 2) + 3);
	} else {
		CASetAnim2(ply, 0x4c, ply->ButtonStrength / 2);
	}	
}
static void sub_32b64(Player *ply) {
	Object *obj;
	if (AF2 == 1) {
		NEXT(ply->mode2);
		if (obj = AllocProjectile()) {
			obj->exists = TRUE;
			obj->Sel = 1;
			obj->XPI = ply->XPI;
			obj->YPI = ply->YPI;
			obj->Flip = ply->Flip;
			obj->SubSel = ply->ButtonStrength;
			obj->Owner = ply;
			ply->Projectile = obj;							
		}
		ply->LocalTimer = 40;
	}
	PLAYERTICK;
}
static int sub_32c3a(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	if (ply->Projectile) {
		if(--ud->x00d0 == 0) {
			soundsting(SOUND_FLAME);
		}
		PLAYERTICK;
		return 1;
	} else {
		return 0;
	}
}
static int sub_32bba(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	if (--ply->LocalTimer) {
		if (--ud->x00d0 == 0) {
			soundsting(SOUND_FIRE);
		}
		PLAYERTICK;
	}
	return AF1;	
}
static void sub_32be4(Player *ply) {
	Object *obj;

	if (AF1 == 1) {
		NEXT(ply->mode2);
		if (obj=AllocProjectile()) {
			obj->exists = TRUE;
			obj->Sel = 2;
			obj->XPI = ply->XPI;
			obj->YPI = ply->YPI;
			obj->Flip = ply->Flip;
			obj->SubSel = ply->ButtonStrength;
			obj->Owner = ply;
			ply->Projectile = obj;
			soundsting(SOUND_FLAME);
		}						
	}
	PLAYERTICK;	
}

void PSCBPowerDhalsim(Player *ply) {		// 32b1a
	UD *ud = (UD *)ply->UserData;
	switch (ud->x00c0) {
		case 0:
			switch (ply->mode2) {
				case 0:
					sub_32b3c(ply);
					break;
				case 2:
					sub_32b64(ply);
					break;
				case 4:
					if (sub_32bba(ply)) {
						ud->x00c2 = 0;
						ply_exit_stand(ply);
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 2:
			switch (ply->mode2) {
				case 0:
					sub_32b3c(ply);
					break;
				case 2:
					sub_32be4(ply);
					break;
				case 4:
					if (sub_32c3a(ply)) {
						ud->x00c2 = 0;
						ply_exit_stand(ply);
					}
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
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
					CASetAnim2(ply, STATUS_JUMP_KICK, ply->ButtonStrength + 1);
					if (ply->Flip) {
						ud->x0080 =  0xb;
					} else {
						ud->x0080 = 0x15;
					}
				} else {
					CASetAnim2(ply, STATUS_JUMP_PUNCH, ply->ButtonStrength + 1);
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
static void sub_32906(Player *ply, int arg) {
	UD *ud = (UD *)ply->UserData;
	ud->x0082 = arg;
	CASetAnim2(ply, 0x4e, arg);
	ply->x0163 = TRUE;
	PLAYERTICK;
}
void PSCBVictoryDhalsim(Player *ply) {		//328a8
	static const int data_cf6f4[] = {
		0x0000, 0x00E0, 0x0031, 0x00DB, 0x0061, 0x00CE, 0x008E, 0x00BA, 
		0x00B4, 0x009E, 0x00D4, 0x007C, 0x00EC, 0x0055, 0x00FA, 0x002B, 
		0x0100, 0x0000, 0x00FA, 0xFFD5, 0x00EC, 0xFFAB, 0x00D4, 0xFF84, 
		0x00B4, 0xFF62, 0x008E, 0xFF46, 0x0061, 0xFF32, 0x0031, 0xFF25, 
		0x0000, 0xFF20, 0xFFCF, 0xFF25, 0xFF9F, 0xFF32, 0xFF72, 0xFF46, 
		0xFF4C, 0xFF62, 0xFF2C, 0xFF84, 0xFF14, 0xFFAB, 0xFF06, 0xFFD5, 
		0xFF00, 0x0000, 0xFF06, 0x002B, 0xFF14, 0x0055, 0xFF2C, 0x007C, 
		0xFF4C, 0x009E, 0xFF72, 0x00BA, 0xFF9F, 0x00CE, 0xFFCF, 0x00DB, 
	};
	UD *ud = (UD *)ply->UserData;
	if (ply->mode3 == 0) {
		if (ply->x02e6 == 0) {
			if (ud->x0082 == 0) {
				ud->x0082 += 2;
				ply->LocalTimer = 60;
			} else {
				if (--ply->LocalTimer == 0) {
					NEXT(ply->mode3);
					ply->PSFinishedParticipating = TRUE;
					g.FightOver = TRUE;
					ply->LocalTimer = 0;		// unneeded

					if (g.OnBonusStage) {
						sub_32906(ply, 0);
					} else {
						if ((ply->RoundsWon ? 0x1088 : 0xef77) & (1 << RAND32)) {
							sub_32906(ply, 0);
						} else {
							sub_32906(ply, 1);
						}
					}
				}
			}
		} else {
			return;
		}
	} else {
		if (ud->x0082) {
			if (ply->AnimFlags) {
				ply->Y.full += (data_cf6f4[ply->LocalTimer * 2] << 8);
				++ply->LocalTimer;
				ply->LocalTimer &= 0x1f;
			} else {
				PLAYERTICK;
			}
		} else {
			if (AF2) {
				ply->Flip ^= 1;
			}
			PLAYERTICK;
		}
	}	
}

static int sub_361c0(Player *ply) {
	if (ply->AISigSpecial && ply->Projectile == NULL) {
		ply->StandSquat = 8;
		return TRUE;
	}
	return FALSE;
}
static void sub_36166(Player *ply) {
	if (ply->CompDoThrow && ply->PunchKick == PLY_PUNCHING && ply->ButtonStrength != 0) {
		PLY_THROW_SET(0xffe0, 0x0035, 0x0020, 0x0010);
		if (throwvalid(ply)) {
			ply->StandSquat = 6;
			ply->Move = ply->ButtonStrength / 4;
		}
	}
}
static void sub_35dd2(Player *ply) {
	ply->AISigAttack = FALSE;
	ply->AIVolley    = FALSE;
	exit_comp_normal(ply);	
}
static void sub_35d46(Player *ply) {
	switch (ply->mode2) {
		case 0:
			if (ply->PunchKick == PLY_PUNCHING) {
				ply->Move = ply->ButtonStrength;
				if (ply->OppXDist > (short []){51, 48, 60}[ply->ButtonStrength/2]) {
					++ply->Move;
				}
				quirkysound(ply->ButtonStrength/2);
				setstatus4(ply, STATUS_PUNCH);
			} else {
				ply->Move = ply->ButtonStrength * 2;
				if (ply->OppXDist > (short []){51, 56, 66}[ply->ButtonStrength/2]) {
					ply->Move += 2;
				}
				quirkysound(ply->ButtonStrength - 1);
				setstatus4(ply, STATUS_KICK);
			}
			break;
		case 2:
			if (AF1) {
				sub_35dd2(ply);
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_35e28(Player *ply) {
	if (AF1) {
		ply->AISigAttack = FALSE;
		ply->AIVolley    = FALSE;
		exit_to_compdisp1(ply);
	}	
}
static void sub_35de0(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			if (ply->mode2 == 0) {
				ply->Move = ply->ButtonStrength;
				if (ply->OppXDist > (short []){47, 47, 41}[ply->ButtonStrength/2]) {
					++ply->Move;
				}
				quirkysound(ply->ButtonStrength/2);
				setstatus4(ply, 0x44);
			} else {
				sub_35e28(ply);
			}
			break;
		case PLY_KICKING:
			if (ply->mode2 == 0) {
				ply->Move = ply->ButtonStrength;
				if (ply->OppXDist > (short []){50, 60, 51}[ply->ButtonStrength/2]) {
					++ply->Move;
				}
			}
			switch (ply->Move) {
				case 0:
				case 2:
					if (ply->mode2 == 0) {
						quirkysound(ply->ButtonStrength/2);
						setstatus4(ply, 0x46);
					} else {
						sub_35e28(ply);
					}
					break;
				case 1:
				case 3:
				case 4:
				case 5:
					switch (ply->mode2) {
						case 0:
							NEXT(ply->mode2);
							ud->x00f4 = 0xffff0000;
							ud->x00f0 = (int[]){0xa0000,0xc0000,0xe0000}[ply->ButtonStrength/2];
							CASetAnim2(ply, STATUS_CROUCH_KICK, ply->Move);
							break;
						case 2:
							if (AF2) {
								NEXT(ply->mode2);
							}
							PLAYERTICK;
							break;
						case 4:
							if (ply->Flip) {
								ply->XPI += ud->x00f0;
							} else {
								ply->XPI -= ud->x00f0;
							}
							ud->x00f0 += ud->x00f4;
							if (ud->x00f0 < 0) {
								NEXT(ply->mode2);
							}
							if (((g.libsplatter + ply->Side) & 3) == 0) {
								if (ply->OnPlatform2 && ply->OnPlatform == 0) {
									ply->OnPlatform2 = FALSE;
									comp_setjumping_main(ply);	
								}
							}
							break;
						case 6:
							if (AF1) {
								ply->AISigAttack = FALSE;
								ply->AIVolley    = FALSE;
								exit_to_compdisp1(ply);
							}
							break;
						FATALDEFAULT;
					}
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}
static void sub_36020(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	ply->X.full += ply->Path[ud->x0080].x.full;
	ply->Y.full += ply->Path[ud->x0080].y.full;
}
static void sub_35ffa(Player *ply) {
	ply->mode2 = 2;
	sub_36020(ply);
	if (PLAYERGROUND) {
		ply->Airborne = 0;
		ply->CompDoJump = FALSE;
		soundsting(SOUND_IMPACT8);
		comp_setjumping_main(ply);
	} else {
		PLAYERTICK;
	}
}
static void sub_36078(Player *ply) {
	if (ply->ButtonStrength == 4) {
		if (ply->mode2 == 0) {
			NEXT(ply->mode2);
			ply->Step = ply->IsWithinBounds ^ 1;
			ply->Flip = ply->Step;
			ply->Opponent->Flip = ply->Flip ^ 1;
			CASetAnim2(ply, 0x50, ply->Move);
		} else {
			if (ply->mode3 == 0) {
				if (AF2) {
					NEXT(ply->mode3);
					set_throw_trajectory(ply, 0, ply->Flip, 12);
				}
				PLAYERTICK;
			} else {
				if (AF1) {
					ply->Flip ^= 1;
					ply->EnemyDirection = ply->Flip;
					sub_35dd2(ply);
				} else {
					PLAYERTICK;
				}
			}

		}
	} else {
		if (ply->mode2 == 0) {
			random_damage_adjust_2(ply, 0x25);
			random_damage_adjust_1(ply, 12, 9);
			ply->LocalTimer = 0xb4;
			setstatus4(ply, 0x50);
		} else {
			if (--ply->LocalTimer == 0) {
				ply_grip_release(ply, ply->Flip);
				sub_35dd2(ply);
			} else {
				if (ply_opp_has_struggled_free(ply)) {
					ply_grip_release(ply, ply->Flip);
					sub_35dd2(ply);
				} else  {
					if (sub_3fee(ply)) {
						ply->Timer = 1;
					}
					if (AF2) {
						if(ply_opp_apply_grip_damage(ply, 0, 3, (ply->Flip ? 16 : -16), 0x56, 0x29)){
							sub_35dd2(ply);
						} else {
							PLAYERTICK;
						}
					} else {
						PLAYERTICK;
					}
				}
			}

		}

	}
}
static void sub_361d8(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->mode2) {
				case 0:
					ud->x00c0 = ply->PunchKick;
					sub_32b3c(ply);
					break;
				case 2:
					sub_32b64(ply);
					break;
				case 4:
					if (sub_32bba(ply) == 0) {
						sub_35dd2(ply);
					}
					break;
				FATALDEFAULT;
			}
			break;
		case PLY_KICKING:
		case 4:
			switch (ply->mode2) {
				case 0:
					ud->x00c0 = ply->PunchKick;
					sub_32b3c(ply);
					break;
				case 2:
					sub_32be4(ply);
					break;
				case 4:
					if (sub_32c3a(ply)==0) {
						sub_35dd2(ply);
					}
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}

void PLCBCompAttackDhalsim(Player *ply) {
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		if (ply->mode2 == 0) {
			if(sub_361c0(ply) == 0) {
				sub_36166(ply);
			}
		}
		switch (ply->StandSquat) {
			case PLY_STAND:		sub_35d46(ply);		break;
			case PLY_CROUCH:	sub_35de0(ply);		break;
			case 4:				sub_35ffa(ply);		break;
			case 6:				sub_36078(ply);		break;
			case 8:				sub_361d8(ply);		break;
			FATALDEFAULT;
		}
	}
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
static void sub_35f54(Player *ply, int move) {
	ply->Move = move;
	CASetAnim2(ply, (ply->PunchKick ? 0x4a : 0x48), ply->Move);
}

int PLCBCompJumpDhalsim(Player *ply) {
	if (ply->ButtonStrength > 3) {
		if (ply->VelY.full < -0x0100 && ply->VelY.full > 0x100) {
			sub_35f54(ply, ply->ButtonStrength);
			return TRUE;
		} else {
			return sub_35f7e(ply, ply->ButtonStrength + 1);
		}
	} else {
		sub_35f54(ply, ply->ButtonStrength + 1);
		return TRUE;
	}
}



