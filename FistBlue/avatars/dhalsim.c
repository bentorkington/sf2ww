/*
 *  dhalsim.c
 *  GLUTBasics
 *
 *  Created by Ben on 7/11/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */


#include "sf2.h"
#include "structs.h"
#include "player.h"
#include "particle.h"
#include "rules.h"
#include "playerstate.h"
#include "sound.h"
#include "computer.h"
#include "projectiles.h"
#include "lib.h"
#include "gfxlib.h"
#include "sf2io.h"

#include "dhalsim.h"
#include "dhalsimdata.h"

extern Game g;

int g_dhalsim_d6;

struct dhalsim_powermove {
	signed char mode0;
	signed char mode1;
	signed char x0002;
	signed char sequence;
};
typedef struct dhalsim_powermove DM;

struct UserData_Dhalsim {
	signed char x0080;
	signed char x0082;
	DM			yogafire;		//0090
	DM			yogaflame;		//0098
	signed char move_is_flame;	//00c0 
	signed char timer;			// 00c2
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

static void dhalsim_begin_powermove(Player *ply, DM *dm) {		// 32558
	UD *ud = (UD *)&ply->UserData;
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = 0;
	dm->x0002  = 0;
	ud->timer  = 5;
	BumpDiff_PowerMove();
	++g.HumanMoveCnt;
}
static void sub_32414(Player *ply) {		// 32414 crouch
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
	ply->StandSquat = PLY_CROUCH;
}
static void sub_32328(Player *ply) {		// 32328 stand
	ply->Move = ply->ButtonStrength;
	if (ply->PunchKick) {
		ply->Move += ply->ButtonStrength;
		if (ply->OppXDist > (short []){0x33,0x38,0x42}[ply->ButtonStrength]) {
			ply->Move += 2;
		}
		if (ply->JoyDecode.full & 0xb) {		// any direction but down
			++ply->Move;
		}
	} else {
		if (ply->OppXDist> (short []){0x33,0x30,0x3c}[ply->ButtonStrength]) {
			ply->Move += 2;
		}
	}
	ply->StandSquat = PLY_STAND;
}	
static int dhalsim_luckymove(Player *ply) {		// 32512
	UD *ud = (UD *)&ply->UserData;

	if (ply->Projectile == NULL && ply->PunchKick == PLY_PUNCHING) {
		if (0x5555 & (1<<RAND32)) {
			ud->move_is_flame = 2;
			dhalsim_begin_powermove(ply, &ud->yogaflame);
		} else {
			ud->move_is_flame = 0;
			dhalsim_begin_powermove(ply, &ud->yogafire);
		}
		return -1;
	} else {
		if (ply->StandSquat) {
			sub_32414(ply);
		} else {
			sub_32328(ply);
		}
		return 1;
	}
}
int sub_32386(Player *ply) {
	g_dhalsim_d6 &= 3;
	if (g_dhalsim_d6 && ply->PunchKick==0 && ply->ButtonStrength != 0) {
		PLY_THROW_SET(0xffe0, 0x0035, 0x0020, 0x0010);
		if(throwvalid(ply)) {
			return 1;
		}
	}
	return 0;
}

static int dhalsim_newbuttons(Player *ply) {					// 3257a
	g_dhalsim_d6 = ply->JoyDecode.full;
	return (PLY_NEWBUTTONS & BUTTON_MASK);
}
static void dhalsim_set_anim_stand_attack(Player *ply, int move) {				// 32480
	ply->Move = move;
	if (ply->PunchKick) {
		CASetAnim2(ply, STATUS_JUMP_KICK, move);
	} else {
		CASetAnim2(ply, STATUS_JUMP_PUNCH, move);
	}
}
static void sub_32a58(Player *ply, DM *dm, int buttons) {
	UD *ud = (UD *)ply->UserData;
	if (ply->Projectile == NULL && check_special_ability(ply) && ud->timer) {
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
			if ((ply->JoyCorrect2 & JOY_MOVEMASK) == arg1[0]) {
				NEXT(dm->mode0);
				dm->sequence += 2;
				dm->mode1 = arg2[RAND32];
			} else {
				dm->mode0 = 0;
				dm->sequence = 0;
				dm->x0002 = 0;
			}
			break;
		case 2:
			if (--dm->mode1 == 0) {
				dm->mode0 = 0;
				dm->sequence = 0;
				dm->x0002 = 0;
			} else {
				if ((ply->JoyCorrect2 & JOY_MOVEMASK) == arg1[dm->sequence / 2]) {
					if (arg1[dm->sequence] & BUTTON_MASK) {
						if((buttons = buttonspressed(ply, arg1[dm->sequence / 2]))){
							sub_32a58(ply, dm, buttons);
							return;
						}
						if ((buttons = buttonsreleased(ply, arg1[dm->sequence / 2]))) {
							sub_32a58(ply, dm, buttons);
							return;
						}
						NEXT(dm->mode0);
					} 
					dm->sequence += 2;
					dm->mode1 = arg2[RAND32];
				}
			}
			break;
		case 4:
			if (--dm->mode1 == 0) {
				dm->mode0    = 0;
				dm->sequence = 0;
				dm->x0002    = 0;
			} else {
				if((buttons = buttonspressed(ply, arg1[dm->sequence / 2]))){
					sub_32a58(ply, dm, buttons);
					return;
				}
				if ((buttons = buttonsreleased(ply, arg1[dm->sequence / 2]))) {
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
	ply->mode1 = PLSTAT_IN_POWERMOVE;
	ply->mode2 = 0;
	dm->x0002  = 0;
	ud->timer  = 5;
	BumpDiff_PowerMove();
	++g.HumanMoveCnt;
}
static void dhalsim_trajectory(Player *ply) {			// 3287c
	UD *ud = (UD *)ply->UserData;
	ply->X.full += (ply->Path[ud->x0080].x.full << 8);
	ply->Y.full += (ply->Path[ud->x0080].y.full << 8);
}
static void dhalsim_check_exit_crouch(Player *ply) {		// 32664
	UD *ud = (UD *)ply->UserData;
	if (AF1) {
		ud->timer = 0;
		ply_exit_crouch(ply);
	}	
}
static void sub_3262c(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	ud->timer = 0;
	ply_exit_stand(ply);
}

void PSCBAttackDhalsim(Player *ply) {			// 3258e
	UD *ud = (UD *)ply->UserData;
	// suicide code removed
	if (ply->Timer2) {
		--ply->Timer2;
	} else {
		if (ud->timer) {
			--ud->timer;
		}
		switch (ply->StandSquat) {
			case 0:
				if (ply->mode2) {
					if (AF1) {
						ud->timer = 0;
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
							dhalsim_check_exit_crouch(ply);
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
									dhalsim_check_exit_crouch(ply);
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
										ud->timer = 0;
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
				dhalsim_trajectory(ply);
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
						ply->LocalTimer = 3 * TICKS_PER_SECOND;
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
								if (ply_opp_has_struggled_2(ply)) {
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

#pragma mark Dhalsim Comp Projectiles
static void dhalsim_comp_init_firebreath(Player *ply) {		// 32b3c
	UD *ud = (UD *)ply->UserData;
	NEXT(ply->mode2);
	soundsting(SOUND_YOGA);
	ud->x00d0 = 0xa;
	if (ud->move_is_flame) {
		CASetAnim2(ply, 0x4c, (ply->ButtonStrength / 2) + 3);
	} else {
		CASetAnim2(ply, 0x4c, ply->ButtonStrength / 2);
	}	
}
static void dhalsim_comp_start_proj_yogafire(Player *ply) {			// 32b64
	Object *obj;
	if (AF2 == 1) {
		NEXT(ply->mode2);
		if ((obj = AllocProjectile())) {
			obj->exists = TRUE;
			obj->Sel    = SF2_PROJ_YOGAFIRE;
			obj->XPI    = ply->XPI;
			obj->YPI    = ply->YPI;
			obj->Flip   = ply->Flip;
			obj->SubSel = ply->ButtonStrength;
			obj->Owner  = ply;
			ply->Projectile = obj;							
		}
		ply->LocalTimer = 40;
	}
	PLAYERTICK;
}
static int dhalsim_comp_check_finish_yogaflame(Player *ply) {			// 32c3a
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
static int dhalsim_comp_check_finish_yogafire(Player *ply) {		//32bba
	UD *ud = (UD *)ply->UserData;
	if (--ply->LocalTimer) {
		if (--ud->x00d0 == 0) {
			soundsting(SOUND_FIRE);
		}
		PLAYERTICK;
	}
	return AF1;	
}
static void dhalsim_comp_start_proj_yogaflame(Player *ply) {		// 32be4
	Object *obj;

	if (AF2 == 1) {
		NEXT(ply->mode2);
		if ((obj=AllocProjectile())) {
			obj->exists = TRUE;
			obj->Sel    = SF2_PROJ_YOGAFLAME;
			obj->XPI    = ply->XPI;
			obj->YPI    = ply->YPI;
			obj->Flip   = ply->Flip;
			obj->SubSel = ply->ButtonStrength;
			obj->Owner  = ply;
			ply->Projectile = obj;
			soundsting(SOUND_FLAME);
		}						
	}
	PLAYERTICK;	
}

#pragma mark Dhalsim Comp Callbacks 
void PSCBPowerDhalsim(Player *ply) {		// 32b1a
	UD *ud = (UD *)ply->UserData;
	switch (ud->move_is_flame) {
		case 0:
			switch (ply->mode2) {
				case 0:
					dhalsim_comp_init_firebreath(ply);
					break;
				case 2:
					dhalsim_comp_start_proj_yogafire(ply);
					break;
				case 4:
					if (dhalsim_comp_check_finish_yogafire(ply)) {
						ud->timer = 0;
						ply_exit_stand(ply);
					}
					break;
				FATALDEFAULT;
			}
			break;
		case 2:
			switch (ply->mode2) {
				case 0:
					dhalsim_comp_init_firebreath(ply);
					break;
				case 2:
					dhalsim_comp_start_proj_yogaflame(ply);
					break;
				case 4:
					if (dhalsim_comp_check_finish_yogaflame(ply)) {
						ud->timer = 0;
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
	if ((buttons = dhalsim_newbuttons(ply))) {
		ply->StandSquat = 0;
		++g.HumanMoveCnt;
		decode_buttons(ply, buttons);
		if (LBRareChance()) {
			return dhalsim_luckymove(ply);
		} else if (sub_32386(ply)) {
			ply->StandSquat = PLY_THROW;
			ud->timer = 0;
			ply->Move = ply->ButtonStrength / 4;
		} else {
			sub_32328(ply);
		}
		return 1;
	}
	return 0;
}
int PLCBCrouchDhalsim(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	int buttons;
	ud->timer = 5;
	if ((buttons = dhalsim_newbuttons(ply))) {
		++g.HumanMoveCnt;
		decode_buttons(ply, buttons);
		ply->StandSquat = PLY_CROUCH;
		if (LBRareChance()) {
			return dhalsim_luckymove(ply);
		} else {
			sub_32414(ply);
			return 1;
		}
	}
	return 0;
}
int PLCBJumpDhalsim(Player *ply) {
	UD *ud = (UD *)ply->UserData;
	int buttons;
	ud->timer = 5;
	if ((buttons = dhalsim_newbuttons(ply))) {
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
				ply->mode1 = PLSTAT_ATTACKING;
				ply->mode2 = 0;
				ply->mode3 = 0;
				return -1;
			}
			dhalsim_set_anim_stand_attack(ply, (ply->ButtonStrength + 1) & 0xfe);
			return 1;
		} else {
			dhalsim_set_anim_stand_attack(ply, ply->ButtonStrength + 1);
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
		0x0A, 0x08, 0x08, 0x0B, 0x08, 0x08, 0x09, 0x08, 
		0x09, 0x08, 0x08, 0x0A, 0x08, 0x08, 0x0B, 0x0D, 
		0x08, 0x0F, 0x09, 0x08, 0x0A, 0x0C, 0x08, 0x08, 
		0x0B, 0x08, 0x09, 0x08, 0x0E, 0x0C, 0x08, 0x0A,
	};
	static const char data_32ada[] = {
		0x0A, 0x08, 0x08, 0x0B, 0x08, 0x08, 0x09, 0x08, 
		0x09, 0x08, 0x08, 0x0A, 0x08, 0x08, 0x0B, 0x0D, 
		0x08, 0x0F, 0x09, 0x08, 0x0A, 0x0C, 0x08, 0x08, 
		0x0B, 0x08, 0x09, 0x08, 0x0E, 0x0C, 0x08, 0x0A,
	};

	
	if (ply->PSFinishedParticipating == FALSE) {
		sub_329aa(ply, data_32aae, data_32ada, &ud->yogaflame);
		if (ud->yogaflame.x0002) {
			ud->move_is_flame = 2;
			sub_32afa(ply, &ud->yogaflame);
		}
		sub_329aa(ply, data_32aa6, data_32aba, &ud->yogafire);
		if (ud->yogafire.x0002) {
			ud->move_is_flame = 0;
			sub_32afa(ply, &ud->yogafire);
		}
	}
	return 0;	// always returns 0
}

static void dhalsim_victory_anim(Player *ply, int arg) {		//32906
	UD *ud = (UD *)ply->UserData;
	ud->x0082 = arg;
	CASetAnim2(ply, DHALSIM_STATUS_VICTORY, arg);
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
				ply->LocalTimer = 1 * TICKS_PER_SECOND;
			} else {
				if (--ply->LocalTimer == 0) {
					NEXT(ply->mode3);
					ply->PSFinishedParticipating = TRUE;
					g.FightOver = TRUE;
					ply->LocalTimer = 0;		// unneeded

					if (g.OnBonusStage) {
						dhalsim_victory_anim(ply, 0);
					} else {
						if ((ply->RoundsWon ? 0x1088 : 0xef77) & (1 << RAND32)) {
							dhalsim_victory_anim(ply, 0);
						} else {
							dhalsim_victory_anim(ply, 1);
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

static int dhalsim_comp_check_special(Player *ply) {			// 361c0
	if (ply->AISigSpecial && ply->Projectile == NULL) {
		ply->StandSquat = 8;
		return TRUE;
	}
	return FALSE;
}
static void dhalsim_check_throw(Player *ply) {		// 36166
	if (ply->CompDoThrow && ply->PunchKick == PLY_PUNCHING && ply->ButtonStrength != 0) {
		PLY_THROW_SET(0xffe0, 0x0035, 0x0020, 0x0010);
		if (throwvalid(ply)) {
			ply->StandSquat = PLY_THROW;
			ply->Move = ply->ButtonStrength / 4;
		}
	}
}
static void dhalsim_comp_exit_stand(Player *ply) {		// 35dd2
	ply->AISigAttack = FALSE;
	ply->AIVolley    = FALSE;
	exit_comp_normal(ply);	
}
static void dhalsim_comp_attack_stand(Player *ply) {
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
				dhalsim_comp_exit_stand(ply);
			}
			break;
		FATALDEFAULT;
	}
}
static void dhalsim_comp_animate_exit_crouch(Player *ply) {			// 35e28
	if (AF1) {
		ply->AISigAttack = FALSE;
		ply->AIVolley    = FALSE;
		exit_to_compdisp1(ply);
	} else {
		PLAYERTICK;
	}

}
static void dhalsim_comp_attack_crouch(Player *ply) {			//35de0
	UD *ud = (UD *)ply->UserData;
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			if (ply->mode2 == 0) {
				ply->Move = ply->ButtonStrength;
				if (ply->OppXDist > (short []){47, 47, 41}[ply->ButtonStrength/2]) {
					++ply->Move;
				}
				quirkysound(ply->ButtonStrength/2);
				setstatus4(ply, STATUS_CROUCH_PUNCH);
			} else {
				dhalsim_comp_animate_exit_crouch(ply);
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
						setstatus4(ply, STATUS_CROUCH_KICK);
					} else {
						dhalsim_comp_animate_exit_crouch(ply);
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
							dhalsim_comp_animate_exit_crouch(ply);
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
static void dhalsim_comp_trajectory(Player *ply) {		//36020
	UD *ud = (UD *)ply->UserData;
	ply->X.full += (ply->Path[ud->x0080].x.full << 8);
	ply->Y.full += (ply->Path[ud->x0080].y.full << 8);
}
static void dhalsim_comp_attack_jump(Player *ply) {		// 35ffa
	ply->mode2 = 2;
	dhalsim_comp_trajectory(ply);
	if (PLAYERGROUND) {
		ply->Airborne	= AIR_ONGROUND;
		ply->CompDoJump = FALSE;
		
		soundsting(SOUND_IMPACT8);
		comp_setjumping_main(ply);
	} else {
		PLAYERTICK;
	}
}
static void dhalsim_comp_attack_throw(Player *ply) {			// 36078
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
					dhalsim_comp_exit_stand(ply);
				} else {
					PLAYERTICK;
				}
			}

		}
	} else {
		if (ply->mode2 == 0) {
			random_damage_adjust_2(ply, 0x25);
			random_damage_adjust_1(ply, 12, 9);
			ply->LocalTimer = 3 * TICKS_PER_SECOND;
			setstatus4(ply, 0x50);
		} else {
			if (--ply->LocalTimer == 0) {
				ply_grip_release(ply, ply->Flip);
				dhalsim_comp_exit_stand(ply);
			} else {
				if (ply_opp_has_struggled_free(ply)) {
					ply_grip_release(ply, ply->Flip);
					dhalsim_comp_exit_stand(ply);
				} else  {
					if (ply_opp_has_struggled_2(ply)) {
						ply->Timer = 1;
					}
					if (AF2) {
						if(ply_opp_apply_grip_damage(ply, 0, 3, (ply->Flip ? 16 : -16), 0x56, 0x29)){
							dhalsim_comp_exit_stand(ply);
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
static void dhalsim_comp_attack_special(Player *ply) {			// 361d8
	UD *ud = (UD *)ply->UserData;
	switch (ply->PunchKick) {
		case PLY_PUNCHING:
			switch (ply->mode2) {
				case 0:
					ud->move_is_flame = ply->PunchKick;
					dhalsim_comp_init_firebreath(ply);
					break;
				case 2:
					dhalsim_comp_start_proj_yogafire(ply);
					break;
				case 4:
					if (dhalsim_comp_check_finish_yogafire(ply) == 0) {
						dhalsim_comp_exit_stand(ply);
					}
					break;
				FATALDEFAULT;
			}
			break;
		case PLY_KICKING:
		case 4:
			switch (ply->mode2) {
				case 0:
					ud->move_is_flame = ply->PunchKick;
					dhalsim_comp_init_firebreath(ply);
					break;
				case 2:
					dhalsim_comp_start_proj_yogaflame(ply);
					break;
				case 4:
					if (dhalsim_comp_check_finish_yogaflame(ply)==0) {
						dhalsim_comp_exit_stand(ply);
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
			if(dhalsim_comp_check_special(ply) == 0) {
				dhalsim_check_throw(ply);
			}
		}
		switch (ply->StandSquat) {
			case PLY_STAND:		dhalsim_comp_attack_stand(ply);		break;
			case PLY_CROUCH:	dhalsim_comp_attack_crouch(ply);		break;
			case PLY_JUMP:		dhalsim_comp_attack_jump(ply);		break;
			case PLY_THROW:		dhalsim_comp_attack_throw(ply);		break;
			case 8:				dhalsim_comp_attack_special(ply);		break;
			FATALDEFAULT;
		}
	}
	
}
static short dhalsim_attack_spearhead(Player *ply, int move) {		// 35f7e
	UD *ud = (UD *)ply->UserData;
	
	ply->StandSquat = PLY_JUMP;
	ply->Move       = move;
	ply->Path       = (VECT16 *)data_cfaf4;
	if (ply->PunchKick) {
		CASetAnim2(ply, STATUS_JUMP_KICK, ply->Move);
		if(ply->Flip != FACING_LEFT){
			ud->x0080 = 0xb;
		} else {
			ud->x0080 = 0x15;
		}
	}
	else {
		CASetAnim2(ply, STATUS_JUMP_PUNCH, ply->Move);
		if(ply->Flip != FACING_LEFT){
			ud->x0080 = 0xa;
		} else {
			ud->x0080 = 0x16;
		}
	}
	// 35fd4
	ply->mode1 = PLSTAT_ATTACKING;
	ply->mode2 = 0;
	ply->mode3 = 0;
	return MINUS_ONE;
}
short sub_35fe6(Player *ply) {
	if(ply->VelY.full > -0x100 && ply->VelY.full <= 0x100) {
		return dhalsim_attack_spearhead(ply, 5);
	}
	return 0;
}

int PLCBCompJumpDhalsim(Player *ply) {
	if (ply->ButtonStrength > 3) {
		if (ply->VelY.full < -0x0100 && ply->VelY.full > 0x100) {
			ply->Move = ply->ButtonStrength;
			CASetAnim2(ply, (ply->PunchKick ? STATUS_JUMP_KICK : STATUS_JUMP_PUNCH), ply->Move);
			return TRUE;
		} else {
			return dhalsim_attack_spearhead(ply, ply->ButtonStrength + 1);
		}
	} else {
		ply->Move = ply->ButtonStrength + 1;
		CASetAnim2(ply, (ply->PunchKick ? STATUS_JUMP_KICK : STATUS_JUMP_PUNCH), ply->Move);
		return TRUE;
	}
}



