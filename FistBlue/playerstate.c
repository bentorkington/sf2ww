/* playerstate.c */

#include "sf2const.h"

#include "sf2macros.h"
#include "sf2types.h"

#include "gstate.h"
#include "structs.h"
#include "player.h"

#include "structs.h"
#include "particle.h"
#include "sprite.h"
#include "sound.h"

#include "gfxlib.h"
#include "sf2io.h"

#include "playerstate.h"
#include "reactmode.h"
#include "rules.h"
#include "actions.h"
#include "collision.h"

#include "lib.h"

#include "computer.h"

#include "ryu.h"
#include "guile.h"
#include "ehonda.h"
#include "blanka.h"
#include "chunli.h"


extern Game g;
//extern struct geepee GP;


extern u16 data_2aa30[12][4][4];
extern struct smalladjust data_29126[];

static void PSJumpFromCrouch(Player *ply);
static int retreat_or_block(Player *ply);     /* 2a6b8 */
static void ply_set_standup(Player *ply);
static void _PSStandBlockToCrouchBlock(Player *ply);		// 28d7e
static void standblock_crouch(Player *ply);	/* 2a8ca */
static void sub_28d36(Player *ply);

void PSCBAttackDhalsim(Player *ply);
void PSCBPowerDhalsim(Player *ply);
void PSCBVictoryDhalsim(Player *ply);
void PSCBAttackZangeif(Player *ply);
void PSCBPowerZangeif(Player *ply);
void PSCBVictoryZangeif(Player *ply);

// reco as 12 * 4 * struct Vect8
short data_2abb0[12][8] = {
	{ -0x300, 0x0000, 0x0200, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfda0, 0x0000, 0x01a0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfdc0, 0x0000, 0x01e0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfca0, 0x0000, 0x0280, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfd00, 0x0000, 0x0200, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfc00, 0x0000, 0x0300, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfd80, 0x0000, 0x0180, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfe00, 0x0000, 0x0160, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfc00, 0x0000, 0x0300, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfd80, 0x0000, 0x0200, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfc80, 0x0000, 0x0300, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
	{ 0xfb80, 0x0000, 0x0380, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  },
};



static short PSGetRoundResult(void) {		/* 2a768 */
	return g.RoundResult;
}

void PSEntry(Player *ply) {   /* 0x28396 was: player_per_frame */
    short offsetsel;
	
    if(ply->Human) {      		
        human_per_frame(ply);
    } else {             
        if(g.OnBonusStage) {
			/* shouldn't happen */
            ply->PSFinishedParticipating = TRUE;
        } else {
            set_comp_difficulty(ply);      /* 285ea */
            computer_per_frame(ply);
        }
    }
    check_ply_x_bounds(ply);
	
	// Update an 'extra' sprite associated with an avatar, such as Vega's claw
    ply->ExtraSpriteEna = FALSE;
    if(ply->exists && ply->VegaHasClaw) {
        if(offsetsel = ply->ActionScript->ExtraSprite) {
            ply->ExtraSpriteEna = TRUE;
            ply->Draw_OffsetX = ply->Sprite2[offsetsel].Offset.x;
            ply->Draw_OffsetY = ply->Sprite2[offsetsel].Offset.y;
            ply->Image2       = ply->Sprite2[offsetsel].Image;
        }
    }
}


void (*playerstate_LU[11])(Player *ply) = {
	proc_plstat_normal,
	proc_plstat_crouch,
	proc_plstat_jumping,
	proc_plstat_standingup,
	proc_plstat_blockstun,
	proc_plstat_attacking,
	proc_plstat_powermove,
	proc_plstat_taking_hit,
	proc_plstat_victory, 
	proc_plstat_victory,
	proc_plstat_thrown_recovery,
};

#pragma mark ---- Diziness ----

static void PSMakeDizzy(Player *ply) {			/* 2a652 make dizzy */
	ply->DizzyFall     = TRUE;
	ply->DizzyCount    = 0;
	ply->DizzyClearCnt = 0;
	
	switch (ply->ReactMode) {
		case RM_YOGAFIREHIT:
		case RM_ELECTROCUTED:
		case RM_TUMBLE1:
		case RM_TUMBLE2:
		case RM_FOOTSWEPT:
		case RM_FIREBALLHIT:
			break;
		default:
			ply->ReactMode = RM_FIREBALLDOWN;
	}
}
static void PSDizzyRandomise(Player *ply) {	        	/* 2a638 4 callers */
	static char data_2a698[32] = {
		1, -1, 2, 0, 1, 0, 0, -2, 0, 1, -1, -2, 0, 0, 2, 0, 3, -3,
		-1, 0, 0, 1, 0, 1, -1, 0, -2, 2, -2, 2, 0, -1,
	};
	
	ply->DizzyCount += data_2a698[RAND32];			/* chars */
	if(ply->DizzyCount > 0x1e) {
		PSMakeDizzy(ply);
	}
}
static void PSDizzyAccounting(Player *ply) {
	static u16 data_2a5ec[3][2] = {{0x6, 0x28},{0xb, 0x37},{0xe, 0x50}};
	
	if (ply->BlockStun) { return; }
	if(ply->DizzyFall) {
		PSMakeDizzy(ply);
		return;
	}
	if(ply->IsBlockingSuper) {
		if(ply->ReactMode == RM_MULTIHIT) {
			ply->DizzyCount    +=   8;
			ply->DizzyClearCnt += 120;
		} else {
			ply->DizzyCount    +=  15;
			ply->DizzyClearCnt += 120;
		}
		PSDizzyRandomise(ply);
		return; 
	}
	switch (ply->ReactMode) {
		case RM_0x06:
		case RM_0x08:
			break;
		case RM_HITINAIR:
			ply->DizzyCount    +=  15;
			ply->DizzyClearCnt +=  50;
			PSDizzyRandomise(ply);
			break;
		case RM_FOOTSWEPT:								/* 2a608 */
			ply->DizzyCount    +=   8;
			ply->DizzyClearCnt += 130;
			PSDizzyRandomise(ply);
			break;
		default:								/* 2a5d2 */
			ply->DizzyCount    += data_2a5ec[ply->ReactMode2][0];
			ply->DizzyClearCnt += data_2a5ec[ply->ReactMode2][1];
			PSDizzyRandomise(ply);
			break;
	}
}

#pragma mark ---- Damage Accounting ----

static void PSPlayerDamage(Player *ply, short energy){		//2a460
	short temp;
	ply->x01ac          = FALSE;			// set in react_to_attack, but never compared
	ply->DidCollideDash = ply->DidCollide;
	ply->DidCollide     = 0;
	ply->ProjHit		= FALSE;
	ply->Energy			= energy;
	ply->EnergyDash		= energy;
	ply->Attacking		= FALSE;
	ply->IsJumpThreat	= FALSE;
	ply->ReactMode		= ply->NextReactMode;
	ply->ReactMode2		= ply->NextReactMode2;
	temp = ply->DizzyStun;
	ply->DizzyStun = 0;
	if (temp==0) {
		PSDizzyAccounting(ply);
	}
	ply->mode1 = PLSTAT_REEL;
	ply->mode2 = 0;
	ply->mode3 = 0;
	ply->Invincible   = FALSE;
	ply->BlockStun    = FALSE;
	
	if(ply->DizzyFall == FALSE && ply->ReactMode == RM_HURRICANE 
						|| ply->ReactMode == RM_MULTIHIT) {
		ply->Invincible = TRUE;
	}
	if(ply->VegaHasClaw) {
		if(ply->ClawCnt <= 8) {
			ply->ClawCnt++;
		} else {
			if( 0xdfdf & (1 << (RAND16)) ) {
				ply->VegaHasClaw = FALSE;
				action_1cd3c(ply);
			}
		}
	}
}

static void PSPlayerKO(Player *ply) {		/* 2a508 */
	short x;
	
	char data_2a574[18] = {			// 2a574
		DIESTAT_0, DIESTAT_0, DIESTAT_0, DIESTAT_0, 
		DIESTAT_0, DIESTAT_0, DIESTAT_0, DIESTAT_0, 
		DIESTAT_0, DIESTAT_0, DIESTAT_0, DIESTAT_0, 
		DIESTAT_4, DIESTAT_ELECTROCUTED, DIESTAT_8, DIESTAT_10,
		DIESTAT_0, DIESTAT_12,
	};		
	
	if(ply->mode1 != PLSTAT_TUMBLE) {
		if(g.RoundResult < 0 || g.RoundResult != (1<<ply->Side)) {
			ply->mode0 += 2;		/* 2a544 */
			ply->mode1 = ply->mode2 = ply->mode3 = 0;
			ply->Attacking    = 0;
			ply->IsJumpThreat = FALSE;
			ply->ProjHit      = FALSE;
			ply->DizzyStun    = FALSE;
			ply->mode1        = data_2a574[ply->NextReactMode / 2];
		} else {
			x = ply->UndealtDamage;
			ply->UndealtDamage = 0;
			if(x) {
				QueueEffect(ply->Opponent->RewardID, ply->Opponent->Side);
			}
			PSPlayerDamage(ply, 0);
		}
	}
}

void player_postamble(Player *ply) {	// 2a410 called after comp_proc_stat and human_per_frame
	int temp;
	if(ply->BlockStun) {
		/* 2a500 */
		if (ply->Energy >= 0) {
			PSPlayerDamage(ply, ply->Energy);
		} else {
			PSPlayerKO(ply);
		}
	} else {
		if(ply->Energy < 0) {
			PSPlayerKO(ply);
		} else if(ply->Energy != ply->EnergyDash) {
			// 2a428 
			ply->MultiHitToggle ^= 1;
			if(ply->UndealtDamage == 0) {
				PSPlayerDamage(ply, ply->Energy);
			} else {
				QueueEffect(ply->Opponent->RewardID, ply->Opponent->Side);
				temp = ply->UndealtDamage;
				ply->UndealtDamage = 0;
				if((ply->Energy - temp) >= 0) {
					PSPlayerDamage(ply, ply->Energy - temp); 
				} else {
					ply->Energy = -1;
					PSPlayerKO(ply);
				}
			}
		}
	}
	
}
       
void human_per_frame(Player *ply) {		/* 285f4 */
    switch (ply->mode0) {
    case 0:
        NEXT(ply->mode0);
        LBInitHitBoxes(ply);
        ply->Step     = 
		ply->StepSave = STEP_STILL;
        CASetAnim1(ply, STATUS_NORMAL);
        break;
    case 2:
        if(g.OnBonusStage) {
            if(ply->TimerInvincible) {
                ply->TimerInvincible--;
            }
            if(ply->Energy != ply->EnergyDash) {
                ply->Energy = ply->EnergyDash ^ 0x1;
            }
        }
        if(ply->MultiHoldoff)	/* hit by chunli, honda or blanka multi */
			ply->MultiHoldoff--;  
			                  
        check_powermove_input(ply);
        
        if(g.PreRoundAnim) {
            PLAYERTICK;
        } else {
            if(ply->TCollDis)  {ply->TCollDis--;}
            if(ply->NoThrow_t) {ply->NoThrow_t--;}
            if(ply->ThrowStat < 0) {
                if(ply->DizzyClearCnt) {
                    if(ply->DizzyClearCnt-- == 0) {
                        ply->DizzyCount=0;
                    }
                }
                ply_thrown(ply);
            } else {
                /* Process PL_STAT */
                playerstate_LU[ ply->mode1 / 2 ](ply);		/* 286a2 */
                
                if (ply->mode0 == 2) { 
					if (ply->DizzyClearCnt) {
						if(--ply->DizzyClearCnt  == 0) {
							ply->DizzyCount = 0;
						}
					}
					player_postamble(ply);
				}
            }
        }
    }
}        
                

void PSPushBack(Player *ply) {		/* 29ed4 handle pushback/tumble */
	const char *cur = ply->PSPushBacks;
	char x;
	char a;
	
	if (ply->ProjectilePushBack)    { return; }
	if(ply->mode3 > 4) { return; }
	x = *cur++;
	if(x<=0) {
		cur--;
		x=0;
	}
	ply->PSPushBacks = cur;
	if(ply->BoundCheck == 0 || ply->PlatformFallDir == 0) { return;}
	if(ply->x012a < 0) {		/* copy of VelX.full */
		a=1;
	} else {
		a =  2;
		x = -x;
	}
	if(a == ply->PlatformFallDir && ply->DidCollideDash == FALSE) {
		ply->Opponent->XPI += x;
	}
}
void RMTumble(Player *ply) {	// 294f2
	SMTumble(ply);
	if (ply->Tumble) {
		PSPushBack(ply); /* handle pushback */
	} else {
		NEXT(ply->mode2)
		ply->LocalTimer = 10;
		check_dizzy(ply);
	}
}
static void _PSPlatformCheck(Player *ply) {	// 2979a
	if (ply->Human) {
		if (check_platform_end(ply)) {
			set_falling_from_platform(ply);
		}
	}
}
static void _PSDizzyStruggle(Player *ply) {		//29fe0
	const static u32 data_98cd2[32]={
		0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000,
		0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000,
		0xf0000000, 0xe0000000, 0xc0000000, 0x80000000,
		
		0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 
		0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000,
		0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000,
		0xff800000, 0xff800000, 0xff000000, 0xff000000,
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 
	};

	if (ply->Human) {
		/* 29fe4 */
		if ((ply->JoyDecodeDash.full & 0xf) && 
			(!ply->JoyDecodeDash.full & ply->JoyDecode.full & 0xf)
			) {
			ply->DizzyStruggle += 2;
		}
		if (ply->JoyDecodeDash.full & ply->JoyDecode.full & BUTTON_MASK) {
			ply->DizzyStruggle += 1;
		}
	} else {
		/* 2a01a computer */
		if(data_98cd2[ply->Difficulty] & (1 << RAND32)) {		/* u32 x 32 */
			ply->DizzyStruggle += (char []){
				2,2,1,2,2,1,3,1,3,2,1,1
			}[ply->FighterID];
		}
	}
}
static void _PSDizzyState(Player *ply) {	/* 29f34 diziness */
	switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->DizzyStun     = TRUE;
			ply->DizzyStruggle = 0;
			CASetAnim1(ply, STATUS_DIZZY);
			break;
		case 2:
			if (check_platform_end(ply)) {
				set_falling_from_platform(ply);
			} else if (--ply->DizzySpell) {
				/* 29fb4 */ 
				PLAYERTICK;
				_PSDizzyStruggle(ply);
				if (ply->DizzyStruggle >= 3) {
					ply->DizzyStruggle = 0;
					ply->DizzySpell -= ply->Timer;
					ply->Timer = 1;
					if (ply->DizzySpell <= 0) {
						ply->DizzyStun = FALSE;
						ply->mode3     = 0;
					}
					PLAYERTICK;
				}
			} else {
				/* 29fa8 */
				ply->DizzyStun = FALSE;
				ply->mode3     = 0;
				PLAYERTICK;			
			}
			break;
		FATALDEFAULT;
	}
}

static int is_projectile_near(Player *ply) {			/* 2a6fe */
	if(ply->Opponent->Projectile == 0) { return 0; }
	if(
	   ABS((ply->Opponent->Projectile->XPI- ply->XPI)) < 224) {
		return TRUE;
	} else {
		return FALSE;
	}
}

static int retreat_or_block(Player *ply) {     /* 2a6b8 */
	if (is_projectile_near(ply) || (ply->Opponent->Attacking && ply->OppXDist <= 0xe0)) {	
		if(ply->Flip) {
			if (ply->JoyDecode.full & 2) {
				return TRUE;
			}
		} else {
			if (ply->JoyDecode.full & 1) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

static void standblock_crouch(Player *ply) {	/* 2a8ca */
	ply->mode1 = PLSTAT_STANDBLOCK;
	ply->mode2 = 4;
	ply->mode3 = 0;
	CASetAnim1(ply, STATUS_CROUCH_BLOCK);
}

void exit_jumping2(Player *ply) {	/* 0x288b8 */
	ply->DSOffsetX = 0;
	set_falling_from_platform(ply);
}

#pragma mark ---- NextAction ----

void plstat_do_nextaction(Player *ply) {    /* 28924 */
    void (*data_28932[])(Player *ply) = {
        ply_exit_stand,
        react_to_result,
        turn_around,
        crouch,
        set_jumping,
        ply_set_standup,	// set standup anim, then proc_plstat_crouch()
        exit_jumping2,		/* 288b8 */
    };
    data_28932[ ply->NextAction / 2 ] (ply);
}

void ply_set_standup(Player *ply) {         /* 0x287ec */
    ply->DSOffsetX    = 0;
    ply->mode1        = PLSTAT_CROUCH;    
    ply->mode2        = 2;
    ply->mode3        = 0;
    ply->IsJumpThreat = FALSE;
    CASetAnim1(ply, STATUS_STAND_UP);    /* status id 6 */
    proc_plstat_crouch(ply);
}

void sub_28814(Player *ply) {			// 28814
	ply->mode1 = PLSTAT_JUMPING;
	ply->mode2 = 2;
	ply->DSOffsetX = 0;
}

#pragma mark ---- proc_plstat_* ----

void proc_plstat_normal(Player *ply) {          /* 286cc */
    int temp;
	
    switch (ply->mode2){
		case 0:
			NEXT(ply->mode2)    
			ply->Airborne     = AIR_ONGROUND;
			ply->Attacking    = FALSE;
			ply->IsJumpThreat = FALSE;
			ply->Path         = data_2abb0[ply->FighterID]; 
			CASetAnimWithStep(ply, STATUS_STAND);
			/* FALLTHRU */
		case 2:
			set_towardsaway(ply);
			if(PSGetRoundResult()) { 
				react_to_result(ply);
			} else if (is_facing_enemy(ply)) { 
				turn_around(ply); 
			} else if (is_holding_down(ply)) { 
				crouch(ply);   
			} else {
				temp = ply_cb_standmove(ply);
				if(temp > 0) {
					set_attacking(ply);
					return;
				} else if (temp == 0) {
					if (is_pushing_up(ply))      { 
						set_jumping(ply);  
					} else if (retreat_or_block(ply)) { 
						set_standblock(ply);
					} else if (check_platform_end(ply)) { 
						set_falling_from_platform(ply);
					} else if (ply->Step ^ ply->StepSave ) {
						ply->StepSave = ply->Step;
						CASetAnimWithStep(ply, STATUS_WALKING);
						PLAYERTICK;
						update_obj_path((Object *)ply);					
					} else {
						PLAYERTICK;
						update_obj_path((Object *)ply);					
					}
					
				}
				
			}
			break;
		FATALDEFAULT
    }
}
void proc_plstat_crouch(Player *ply) {		// 28940
    int result;
	
    switch (ply->mode2){
		case 0:
			PLAYERTICK;
			if(check_platform_end(ply)) { set_falling_from_platform(ply); return; } 
			if(PSGetRoundResult())      { react_to_result(ply);           return; }
			if (is_facing_enemy(ply))   { turn_around(ply);               return; } 
			if (is_holding_down(ply)) {
				/* 289b2 */
				result = ply_cb_crouchmove(ply);
				if (result < 0) { return; }
				if (result != 0) {	set_attacking(ply); return;	}
				if (retreat_or_block(ply)) { standblock_crouch(ply); return;}
				return;
			} else {
				result = ply_cb_standmove(ply);
				if(result < 0) {return;}  /* special move */
				if(result > 0) {
					set_attacking(ply);
					return;
				}
				if (is_pushing_up(ply))             { set_jumping(ply);    return; }
				if (retreat_or_block(ply))          { set_standblock(ply); return; }
				if (AF2 == 2)    { return; }
				NEXT(ply->mode2);
				CASetAnim2(ply, STATUS_STAND_UP, AF2);   
			}
			break;
		case 2:
			// 289c8
			PLAYERTICK;
			if (check_platform_end(ply)) {
				set_falling_from_platform(ply);
			} else if (PSGetRoundResult()) {
				react_to_result(ply);
			} else if (is_facing_enemy(ply)) {
				turn_around(ply);
			} else if (is_holding_down(ply)) {
				result = ply_cb_crouchmove(ply);
				if (result<0) {
					return;
				} else if (result > 0) {
					set_attacking(ply);
				} else if (retreat_or_block(ply)) {
					standblock_crouch(ply);
				} else if (ply->Flip != 2) {
					ply->mode2 = 0;
					CASetAnim2(ply, STATUS_CROUCH, AF2);
				}
			} else {
				result = ply_cb_standmove(ply);
				if (result < 0) {
					return;
				} else if (result > 0) {
					set_attacking(ply);
				} else if (is_pushing_up(ply)) {
					set_jumping(ply);
				} else if (retreat_or_block(ply)) {
					set_standblock(ply);
				} else if (AF1) {
					ply_exit_stand(ply);
				}
			}
			break;
			FATALDEFAULT;
	}
}
void proc_plstat_attacking(Player *ply) {
    void (*PCB_ATTACK[8])(Player *ply) = {	
		PSCBAttackRyu,
		PSCBAttackEHonda,	
		NULL,					// todo
		PSCBAttackGuile,	
		PSCBAttackRyu,		//Ken is the same
		PSCBAttackChunLi,
		PSCBAttackZangeif,
		PSCBAttackDhalsim,
	};
	/* 
	 00028E44   0002 d168                    ;Ryu
	 00028E48   0002 da12                     
	 00028E4C   0002 e42a                       
	 00028E50   0002 f81e                    ;Guile
	 00028E54   0002 d168                    ;Ken
	 00028E58   0003 0854                 
	 00028E5C   0003 154e                 
	 00028E60   0003 258e                  
	 */	 
	
    ply->Attacking =  TRUE;
    PCB_ATTACK[ ply->FighterID ](ply);
}

static void PSCBPowerNULL(Player *ply) {	// 28ea2
	/* nothing, for E.Honda and Blanka */
}

void proc_plstat_powermove(Player *ply) {
    void (*PCB_POWERMOVE[8])(Player *ply) = {
		PSCBPowerRyu,
		PSCBPowerNULL,	 //EHonda doesn't need one	
		PSCBPowerNULL,	 //Blanka same as EHonda, nothing
		PSCBPowerGuile,	 
		PSCBPowerRyu,	 //Ken
		NULL,
		PSCBPowerZangeif,
		PSCBPowerDhalsim,
    };
    PCB_POWERMOVE[ply->FighterID](ply);
	/*
	 00028E82   0002 d714                        OR.B      #0xd714,D2    ;Ryu
	 00028E86   0002 8ea2                        OR.B      #0x8ea2,D2
	 00028E8A   0002 8ea2                        OR.B      #0x8ea2,D2
	 00028E8E   0002 fe26                        OR.B      #0xfe26,D2    ;Guile
	 00028E92   0002 d714                        OR.B      #0xd714,D2    ;Ken
	 00028E96   0003 0c6e                        OR.B      #0xc6e,D3
	 00028E9A   0003 1dec                        OR.B      #0x1dec,D3
	 00028E9E   0003 2b1a                        OR.B      #0x2b1a,D3
	 */
	
}
void proc_plstat_jumping(Player *ply) {         /* 28a46 */
    int temp;
	switch(ply->mode2) {
		case 0:			// Init Jump
			PLAYERTICK;
			if((ply->AnimFlags & 0xff) == 0) {
				NEXT(ply->mode2);
				ply->Airborne			= AIR_JUMPING;
				ply->IsJumpThreat       = TRUE;
				ply->OnPlatform2		= FALSE;
			}
			break;
		case 2:			// During jump, can attack
			temp = ply_cb_jumpmove(ply);
			if(temp) {
				if(temp<0) {
					/* 28b42 */
					ply->Attacking	  = TRUE;
					ply->IsJumpThreat = TRUE;
					ply->mode2 = 0;
					ply->mode3 = 0;
					if(ply->Timer2) {
						ply->Timer2--;
					} else {
						CATrajectory((Object *)ply);
						PLAYERTICK;
					}
				} else {
					ply->mode2 = 6;
					ply->Attacking    = TRUE;
					ply->IsJumpThreat = TRUE;
					/* %% */
					if(ply->Timer2) {
						ply->Timer2--;
					} else {
						jump_physics(ply);
					}
				}
			} else {
				jump_physics(ply);
			}
			break;
		case 4:  
			/* normal end to jump, never attacked in the jump */
			if(PSGetRoundResult())          { react_to_result(ply); return; }
			if(is_facing_enemy(ply))        { turn_around(ply);     return; }
			if(is_holding_down(ply)) {
				ply->LocalTimer = 0;                 /* 28ba6 */
				temp=ply_cb_crouchmove(ply);
				if(temp<1)                  { return; }
				if(temp)                    { set_attacking(ply);     return; }
				if(retreat_or_block(ply))   { standblock_crouch(ply); return; }
				crouch(ply);
				return;
			}
			temp=ply_cb_standmove(ply);
			if(temp<0) {return;}
			if(temp)                        { set_attacking(ply); return; }
			if(is_pushing_up(ply))          { set_jumping(ply);   return; }
			if(retreat_or_block(ply))       { set_standblock(ply);return; }
			ply->LocalTimer--;
			ply_exit_stand(ply);
			break;
		case 6:		/* 28a98 inlined, did an attack in the jump */
			if(ply->Timer2) {
				ply->Timer2--;
				return;
			}
			jump_physics(ply);
			break;
		case 8:		// bounced off a wall, only for ChunLi, (maybe Vega?)
			if(--ply->TmrWallBounce2) {return;}
			ply->mode2 = 2;
			PLY_TRAJ0(0x0400, 0x0600, 0x0005, 0x0048);
			ply->TmrWallBounce = 9;
			if(ply->Flip ^= 1 == 0) {
				ply->VelX.full = -ply->VelX.full;
				ply->AclX.full = -ply->AclX.full;
			}
			CASetAnim1(ply, 0x4e);
			break;
    }
}
void proc_plstat_standingup (Player *ply) {		/* 28c06 */
    int temp;
	
    switch(ply->mode2) {
		case 0:
			PLAYERTICK;
			if(PSGetRoundResult())      { react_to_result(ply); return; }
			if(is_holding_down(ply)) {        
				NEXT(ply->mode2);            /* 28c5a back down again */
				temp=ply_cb_crouchmove(ply);
				if(temp<0)                 { return; }
				if(temp)                   { set_attacking(ply); return; }
				if(retreat_or_block(ply))  { standblock_crouch(ply); return; }
				if(AF1){ ply_exit_crouch(ply); return; }		// ???
				temp = ply->Timer;
				CASetAnim2(ply, STATUS_CROUCH_TURN, ply->AnimFlags & 0xff);  
				ply->Timer = temp;
				return;
			}
			if(ply->JoyDecode.full & (JOY_LEFT | JOY_RIGHT))   { ply_exit_stand(ply); return; }
			temp=ply_cb_standmove(ply);
			if(temp<0) {return;}
			if(temp)                        { set_attacking(ply); return; }
			if(is_pushing_up(ply))          { set_jumping(ply); return; }
			if(retreat_or_block(ply))       { set_standblock(ply); return; }
			if(AF1)							{ ply_exit_stand(ply); return; }
			break;
		case 2:		/* 28c90 */
			PLAYERTICK;
			if(PSGetRoundResult()) {
				react_to_result(ply);
				return;
			}
			if(!is_holding_down(ply)) { 
				/* 28cc0 */
				temp=ply_cb_standmove(ply);
				if(temp<0)                 { return; }
				if(temp)                   { set_attacking(ply); return; }
				if(is_pushing_up(ply))     { set_jumping(ply); return; }  
				if(retreat_or_block(ply))  { standblock_crouch(ply); return; }                  
				if(AF1)					   { ply_exit_stand(ply); return; }
				
				ply->mode2 = 0;
				CASetAnim2(ply, STATUS_TURN_AROUND, ply->AnimFlags & 0xff);
				return;
			}
			temp=ply_cb_crouchmove(ply);
			if(temp<0)                 { return; }
			if(temp)                   { set_attacking(ply); return; }
			if(retreat_or_block(ply))  { standblock_crouch(ply); return; }  
			if(AF1) { ply_exit_crouch(ply); return; }
			break;
    }
}
void proc_plstat_blockstun(Player *ply) {       /* 28cfe */
    switch(ply->mode2) {
		case 0:
			if(PSGetRoundResult())              { react_to_result(ply); return; }
			if (is_facing_enemy(ply))           { turn_around(ply);     return; } 
			
			PLAYERTICK;
			
			if (retreat_or_block(ply))          { sub_28d78(ply); return; }
			if (is_holding_down(ply))           { PSJumpFromCrouch(ply); return; }
			standattack_or_jump(ply);
			break;
		case 2:            /* later after standattack_or_jump 28de0 */
			PLAYERTICK;
			if(ply->AnimFlags & 0x8000)          { ply_exit_stand(ply);  return; }
			if(retreat_or_block(ply))           { sub_28d36(ply);       return; }
			if(is_holding_down(ply))            { PSTurnAroundCrouch(ply);       return; }
			standattack_or_jump(ply);
			break;
		case 4:  
			PLAYERTICK;
			if(retreat_or_block(ply))           { sub_28d78(ply);       return; }
			if(is_holding_down(ply))            { _PSStandBlockToCrouchBlock(ply);       return; }
			crouchmove_or_jump(ply);
			break;                   
		case 6:
			PLAYERTICK;
			if(ply->AnimFlags & 0x8000)          { ply_exit_crouch(ply);     return; }
			if(retreat_or_block(ply))           { sub_28d36(ply);           return; }
			if(is_holding_down(ply))            { _PSStandBlockToCrouchBlock(ply);           return; }  
			crouchmove_or_jump(ply);
    }
}
void proc_plstat_taking_hit (Player *ply) {     /* 28ea4 */
    switch (ply->ReactMode) {
		case RM_VOMIT1:
		case RM_VOMIT2:
		case RM_0x04:
		case RM_0x06:
		case RM_0x08:
		case RM_0x0a:
		case RM_0x0c:
		case RM_0x14:
		case RM_MULTIHIT:
		case RM_HURRICANE:
		case RM_0x24:
		case RM_0x26:
			react_to_attack(ply);
			break;
		case RM_FOOTSWEPT:
			RMFootSwept(ply);
			break;
		case RM_HITINAIR:
			RMHitInAir(ply);
			break;
		case RM_ELECTROCUTED:
			RMElectrocuted(ply);
			break;
		case RM_FIREBALLDOWN:      /* knocked over backwards */
		case RM_YOGAFIREHIT:       /* down in flames */
			RMFireball(ply);
			break; 
		case RM_TUMBLE1:
			RMTumble32(ply);
			break;
		case RM_TUMBLE2:
			RMTumble34(ply);
			break;
		case RM_FIREBALLHIT:
			RMFireball(ply);
			break;
			FATALDEFAULT;
    }
}   
void proc_plstat_victory(Player *ply) {		//296f2
	void (*data_2976a[])(Player *ply)={		// 2976a
		PSCBVictoryRyu,
		NULL,		// 2e1fe
		PSCBVictoryBlanka,		// 2ecba
		sub_2ff6e,
		PSCBVictoryRyu,
		PSCBVictoryChunLi,
		PSCBVictoryZangeif,
		PSCBVictoryDhalsim,
		// Victory callbacks per player
	};
	
	switch (ply->PSRoundReactMode) {
		case ROUNDREACT_VICTORY:
			switch (ply->mode2) {
				case 0: NEXT(ply->mode2); ply->LocalTimer=5; _PSPlatformCheck(ply); break;
				case 2: 
					if(--ply->LocalTimer == 0) {
						NEXT(ply->mode2);
					}	
					_PSPlatformCheck(ply);
					break;
				case 4:
					if (g.FlagTimeWarp==0) {
						NEXT(ply->mode2);
						ply->LocalTimer = 0x28;
					}
					_PSPlatformCheck(ply);
					break;
				case 6:
					if(--ply->LocalTimer == 0) {NEXT(ply->mode2);}
					_PSPlatformCheck(ply);
					break;
				case 8:
					data_2976a[ply->FighterID](ply);
					break;
					FATALDEFAULT;
			}
			break;
		case ROUNDREACT_LOSS1:		// 297aa
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->LocalTimer = 5;
					_PSPlatformCheck(ply);
					break;
				case 2:
					if (--ply->LocalTimer == 0) {
						NEXT(ply->mode2);
					}
					_PSPlatformCheck(ply);
				case 4:
					if (g.FlagTimeWarp == 0) {
						NEXT(ply->mode2);
						ply->LocalTimer = 0x28;
					}
					_PSPlatformCheck(ply);
					break;
				case 6:
					if (--ply->LocalTimer == 0) {
						NEXT(ply->mode2);
						ply->PSFinishedParticipating = TRUE;
						CASetAnim1(ply, STATUS_PISSED_OFF);
					} else {
						_PSPlatformCheck(ply);
					}
					break;
				case 8:
					PLAYERTICK;
					break;
					FATALDEFAULT;
			}
			break;
		case ROUNDREACT_LOSS2:	// 29806
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->LocalTimer = 5;
					break;
				case 2:
					if(--ply->LocalTimer == 0) {
						NEXT(ply->mode2);
					}
					_PSPlatformCheck(ply);
					break;
				case 4:
					if (g.FlagTimeWarp==0) {
						NEXT(ply->mode2);
						ply->LocalTimer = 0x28;
					}
					_PSPlatformCheck(ply);
					break;
				case 6:
					if (--ply->LocalTimer == 0) {
						NEXT(ply->mode2);
						ply->PSFinishedParticipating = TRUE;
						CASetAnim1(ply, STATUS_PISSED_OFF);
					} else {
						_PSPlatformCheck(ply);
					}
				case 8:
					/* does nothing */
					break;
					FATALDEFAULT;
			}
			break;
			FATALDEFAULT;
	}
}

void jump_physics(Player *ply) {       /* 0x28aa4 do jump physics, player to ground collision, and react */
    
	CATrajectory((Object *)ply);
    if(ply->VelY.full < 0) {    /* on the way back down? */
        if(PLAYERGROUND) {
            /* we landed */
            ply->Attacking		= FALSE;
            ply->IsJumpThreat   = FALSE;
            ply->mode2			= 4;		// normal ending to jump
            ply->LocalTimer     = 7;
            ply->Airborne		= AIR_ONGROUND;
            queuesound(SOUND_IMPACT8);
            CASetAnim1(ply, STATUS_LANDING);
            return;
        }
    }
    if(ply->FighterID == FID_CHUN_LI) {     /* chun-li can bounce off walls */
        if(g.GPHitBoxCoords[1][0] == 0) {
            if(ply->TmrWallBounce) {
                ply->TmrWallBounce--;
                PLAYERTICK;
                return;
            }
            if(ply->YPI >= 0x68                             /* high enough */
               && ply->VelX.full != 0                       /* we were not jumping straight up */
               && (ply->PlatformFallDir || ply->BoundCheck)           /* we have hit a wall */
               && ply->JoyDecode.full & (ply->BoundCheck - 1)    /* joy is opposite to side we bounced off */
            ) {
                /* Chun-Li bounced off a wall */
                ply->Flip = ply->BoundCheck - 1;
                ply->mode2 = 8;
                ply->TmrWallBounce2 = 6;
                queuesound(SOUND_IMPACT8);
                CASetAnim1(ply, STATUS_BOUNCE_WALL);
                return;
            }
        }
    }
    PLAYERTICK;
}                 
    

/* in the following few stub routines, the CASetAnim1() call also had a timer passed and saved
   afterwards, sometimes in %d3, sometimes in %d5.  These registers are not used or changed by
   CASetAnim1(), so the timers are not passed here */

void sub_28d36(Player *ply) {		// 28d36
    if (is_holding_down(ply))  { 
		PSJumpFromCrouch(ply); 
	} else {
		ply->mode2=PLSTAT_NORMAL;
		CASetAnim2(ply, STATUS_STAND_BLOCK, AF2);
		standattack_or_jump(ply);
	}
}

static void PSJumpFromCrouch(Player *ply) {	// 28d5a
    ply->mode2 = PLSTAT_JUMPING;  
    CASetAnim2(ply, STATUS_CROUCH_BLOCK, AF2);
    crouchmove_or_jump(ply);
}

void sub_28d78(Player *ply) {
    if (is_holding_down(ply)) { 
		PSTurnAroundCrouch(ply);
	} else {
		_PSStandBlockToCrouchBlock(ply);
	}
}

static void _PSStandBlockToCrouchBlock(Player *ply) {		// 28d7e
    ply->mode2=PLSTAT_CROUCH;    
    CASetAnim2(ply, STATUS_STAND_BLOCK2, AF2);		// standing block
    standattack_or_jump(ply);
}
void PSTurnAroundCrouch(Player *ply) {		// 28d9a
    ply->mode2=PLSTAT_TURNAROUND;    
    CASetAnim2(ply, STATUS_CROUCH_BLOCK2, AF2);		
    standattack_or_jump(ply);
}
void crouchmove_or_jump(Player *ply) {
    int temp;
    temp=ply_cb_crouchmove(ply);
    if (temp < 0) { return; }
    if (temp)                   { set_attacking(ply);       return; }
    if (is_pushing_up(ply))     { set_jumping(ply);         return; }
}
void standattack_or_jump(Player *ply) {
    int temp;
    temp=ply_cb_standmove(ply);
    if (temp < 0) { return; }
    if (temp)                   { set_attacking(ply);       return; }
    if (is_pushing_up(ply))     { set_jumping(ply);         return; }
}

void sub_28e64(Player *ply) {
    ply->StandSquat == 0 ? ply_exit_stand(ply) : ply_exit_crouch(ply) ;
}

void sub_29280(Player *ply) {
    NEXT(ply->mode2);
    ply->LocalTimer = 12;
    ply->Airborne   = AIR_ONGROUND;
    queuesound(SOUND_GROUND_THUMP);
    CASetAnim2(ply, STATUS_FOOTSWEPT, 1);

    check_dizzy(ply);
}

void check_dizzy(Player *ply) {			/* 292a0 */
    if(ply->DizzyFall == 0) { return; }
    ply->DizzyStun = TRUE;
    random_dizzyspell(ply);
    ply->DizzyFall = 0;		
    StartDizzyAnim(ply);         /* dizzy stars action */
}

void random_dizzyspell(Player *ply) {				/* 0x29f20 */
	const static u16 data_29f5a[32]={
		0x003c, 0x005a, 0x0078, 0x00b4, 0x003c, 0x005a, 0x0096, 0x00b4, 0x0096, 0x00b4, 0x0096,
		0x00d2, 0x0096, 0x0078, 0x00d2, 0x0078, 0x005a, 0x003c, 0x00d2, 0x0078, 0x005a, 0x00b4,
		0x0096, 0x003c, 0x0096, 0x0078, 0x0096, 0x00d2, 0x0096, 0x005a, 0x00b4, 0x0096,  };

	ply->DizzySpell = data_29f5a[RAND32];	/* this is u16, so CPS actually does sf2rand() & 0x3e */
											/* how to emulate this? */
}

void PSDizzyState(Player *ply) {		/* 29324 */
    if(PSGetRoundResult())  {
        react_to_result(ply);    
    } else {
		_PSDizzyState(ply);
        if(ply->DizzyStun) { return; }
        ply->Invincible = 0;    /* 29336 */
        ply_exit_stand(ply);        
    }
}

int is_facing_enemy(Player *ply) {		//2a720 badly named
	if (ply->Flip == ply->EnemyDirection) {
		return FALSE;
	} else {
		return TRUE;
	}
}

short check_platform_end(Player *ply) {			/* 0x2a73e */
	if(ply->OnPlatform2) {
		if(ply->OnPlatform) {
			return FALSE;
		}
		ply->OnPlatform2 = 0;
	}
	if(ply->YPI <= 40) {
		ply->YPI = 40;
		return FALSE;
	}
	return TRUE;
}
int is_pushing_up(Player *ply) {		//2a72a
	if (ply->x01ad) {			// hmm, look at action04
		return 0;
	} else {
		return (ply->JoyDecode.full & JOY_UP);
	}
}
int is_holding_down(Player *ply) {			// 2a8aa
    return (ply->JoyDecode.full & JOY_DOWN);
}
/* Once upon a time there was light in my life,
   but now there's only love in the dark
   nothing I can can say,
   A total eclipse of the heart */

void turn_around(Player *ply) {	/* bright eyes 0x2a92a */
	short temp;
	ply->Flip = ply->EnemyDirection;
	if(is_holding_down(ply))	{ 
		/* 2a972 */ 
		ply->Flip = ply->EnemyDirection;
		temp = ply_cb_crouchmove(ply);
		if(temp<0) {return;}
		if(temp)		{set_attacking(ply);	return;}
		if(retreat_or_block(ply))		{standblock_crouch(ply);	return;}
		ply->mode1 = PLSTAT_TURNAROUND;
		ply->mode2 = 2;
		ply->mode3 = 0;
		CASetAnim1(ply, STATUS_TURN_AROUND);
	} else {
		temp = ply_cb_standmove(ply);
		if(temp<0) {return;}
		if(temp)					{set_attacking(ply);  return; }
		if(is_pushing_up(ply))		{set_jumping(ply);    return; }
		if(retreat_or_block(ply))	{set_standblock(ply); return; }
		if(ply->JoyDecode.full & 3) {ply_exit_stand(ply); return; }
		ply->mode1  = PLSTAT_TURNAROUND;
		ply->mode2 = ply->mode3  = 0;
		CASetAnim1(ply, STATUS_TURN_AROUND);
	}
}
void crouch(Player *ply) {          /* 0x2a9a4 */
    short temp;
    temp = ply_cb_crouchmove(ply);
	
    if(temp < 0)				{ return; }
    if(temp > 0)                { set_attacking(ply);     return; }
    if(retreat_or_block(ply))   { standblock_crouch(ply); return; }
    ply->mode1 = 2;             /* PLSTAT_CROUCHING */
    ply->mode2 = 0;
    ply->mode3 = 0;
    
    CASetAnim1(ply, STATUS_CROUCH);
}
void set_standblock(Player *ply) {
    ply->mode1 = PLSTAT_STANDBLOCK;
    ply->mode2 = ply->mode3 = 0;
    CASetAnim1(ply, STATUS_STAND_BLOCK);
}
void set_crouchblock(Player *ply) {
    ply->mode1 = PLSTAT_STANDBLOCK;
    ply->mode2 = ply->mode3 = 0;
    CASetAnim1(ply, STATUS_CROUCH_BLOCK);
}
void set_attacking(Player *ply) {
    ply->mode1 = PLSTAT_ATTACKING;
    ply->Attacking = TRUE;
    ply->mode2 = ply->mode3 = 0;
}

void set_falling_from_platform(Player *ply) {   /* was setjumping2 */
    ply->mode1 = PLSTAT_JUMPING;
    ply->mode2 = 2;			// we init the jump ourselves
    ply->mode3 = 0;
    ply->VelX.full = ply->VelY.full = ply->AclX.full = 0;
    ply->AclY.full = 0x48;          /* standard gravity */
    ply->Airborne = AIR_JUMPING;
    CASetAnim1(ply, STATUS_FALLING);
}
void set_jumping(Player *ply) {
    ply->mode1 = PLSTAT_JUMPING;
    ply->mode2 = ply->mode3 = 0;
    set_towardsaway(ply);
	
	
    ply->VelX.full  = data_2aa30[ply->FighterID][ply->Step ^ ply->Flip][0]; /* word */
    ply->AclX.full  = data_2aa30[ply->FighterID][ply->Step ^ ply->Flip][1];
    ply->VelY.full  = data_2aa30[ply->FighterID][ply->Step ^ ply->Flip][2];
    ply->AclY.full  = data_2aa30[ply->FighterID][ply->Step ^ ply->Flip][3];
    if(ply->Flip) {
        ply->VelX.full = -ply->VelX.full;
        ply->AclX.full = -ply->AclX.full;
    }
    CASetAnimWithStep(ply, STATUS_JUMP_START);
}

	
void SMTumble(Player *ply) {		//2a052 was downandout()
    switch (ply->mode3) {
		case 0:
			NEXT(ply->mode3);
			ply->Tumble          = TRUE;
			ply->ReactTimer      =  0;
			ply->Y.part.integer += 16;
			ply->Airborne        = AIR_REEL1;
			if(ply->ProjHit) {
				if(ply->ProjHit == PROJHIT_FIERY) {
					CASetAnim_BurningFall(ply);
				} else {
					CASetAnim_IcyFall(ply);
				}
			} else {
				CASetAnim2(ply, ply->TumbleStatus, 0);
			}
			break;
		case 2:		//2a0aa
			if(ply->Timer2) {
				if(--ply->Timer2) {
					ply->x012a = ply->VelX.full;
					return;
				}
			}
			NEXT(ply->mode3);
			ply->VelX.full    = ply->NextVelX.full;
			ply->AclX.full    = ply->NextAclX.full;
			ply->VelY.full    = ply->NextVelY.full;
			ply->AclY.full    = ply->NextAclY.full;
			if(ply->Direction == FACING_LEFT) {
				ply->VelX.full = -ply->VelX.full;
			}
			ply->x012a = ply->VelX.full;
			break;
		case 4:
			if(ply->ReactTimer) {
				ply->GroundSoundDisa = TRUE;
				if(--ply->ReactTimer) {
					PLAYERTICK;
					return;
				}
			}
			CATrajectory((Object *)ply);
			if((ply->PlatformFallDir != 0 || ply->BoundCheck != 0) && ply->GroundSoundDisa == 0 ) {
				queuesound(SOUND_GROUND_THUMP);
				ply->ReactTimer = 12;              
				ply->VelX.full  = 0;
				if(ply->VelY.full >  0) { ply->VelY.full = 0; }
			}
			if(ply->VelY.full >= 0) { 
				PLAYERTICK;
				return; 
			}
			if(!PLAYERGROUND) {
				PLAYERTICK;
				return;
			}
			NEXT(ply->mode3);
			ActStartGroundDust(ply);
			queuesound(SOUND_GROUND_THUMP);
			ply->LocalTimer = 4;
			ply->Airborne = AIR_ONGROUND;
			CASetAnim2(ply, ply->TumbleStatus, 1);
			break;
		case 6:
			if(--ply->LocalTimer) { 
				PLAYERTICK;
				return;
			}
			NEXT(ply->mode3);
			ply->VelX.full    = ply->Next2VelX.full;
			ply->AclX.full  = ply->Next2AclX.full;
			ply->VelY.full    = ply->Next2VelY.full;
			ply->AclY.full  = ply->Next2AclY.full;
			if(ply->Direction == FACING_LEFT) {
				ply->VelX.full = -ply->VelX.full;
			}
			ply->Airborne = 1;
			CASetAnim2(ply, ply->TumbleStatus, 2);
			break;
		case 8:
			CATrajectory((Object *)ply);
			if (ply->PlatformFallDir || ply->BoundCheck) {
				ply->VelX.full = 0;
			}
			if(PLAYERGROUND) {
				NEXT(ply->mode3);
				ply->VelX.full    = ply->Next3VelX.full;
				ply->AclX.full  = ply->Next3AclX.full;
				ply->VelY.full    = ply->Next3VelY.full;
				ply->AclY.full  = ply->Next3AclY.full;  
				ply->Airborne = 0;
				if(ply->Direction == FACING_LEFT) {
					ply->VelX.full = -ply->VelX.full;
					ply->AclX.full = -ply->AclX.full;
				}
				CASetAnim2(ply, ply->TumbleStatus, 3);
			} else {
				PLAYERTICK;
			}
			break;
		case 10:
			if(find_apex(ply)) {         /* returns true at vertex of trajectory */
				if(ply->PlatformFallDir == 0 && ply->BoundCheck == 0) {
					PLAYERTICK;
					return;
				}
			}
			ply->mode3 = 0;
			ply->Tumble = FALSE;
			PLAYERTICK;
			break;
		FATALDEFAULT;
    }
}


short PSSetNextAction(Player *ply) {		// 288c0
	ply->NextAction = NEXTACT_ROUNDOVER;
	if (PSGetRoundResult()) { 
		return TRUE;
	}
	if (check_platform_end(ply)) {
		ply->NextAction = NEXTACT_FALLING;
		return TRUE;
	}
	if (is_facing_enemy(ply)) {
		ply->NextAction = NEXTACT_TURNAROUND;
		return TRUE;
	}
	if (ply->StandSquat) {
		if(is_pushing_up(ply)) {
			ply->NextAction = NEXTACT_JUMP;
			return TRUE;
		}
		ply->NextAction = NEXTACT_STANDUP;
		if (is_holding_down(ply)) {
			return TRUE;
		}
	} else {
		if (is_holding_down(ply)) {
			ply->NextAction = NEXTACT_CROUCHDOWN;
			return TRUE;
		}
		ply->NextAction = NEXTACT_JUMP;
		if (is_pushing_up(ply)) {
			return TRUE;
		}
	}
	return FALSE;
}

#pragma mark ply_exit_*

void ply_exit_stand(Player *ply) {	// 2876a
    if(ply->Human) {
        ply->DSOffsetX = 0;
        set_towardsaway(ply);        
        ply->Step |= STEP_STILL;   
        ply->Direction = ply->Step;
        ply->Attacking = 0;
        ply->IsJumpThreat = 0;
        ply->mode1 = PLSTAT_NORMAL;
        ply->mode2 = 0;
        proc_plstat_normal(ply);
    } else {
        /* computer */
        ply->DSOffsetX = 0;
        comp_recovery_complete(ply);
        exit_comp_plstatfe_normal(ply);
    }
}


void ply_exit_crouch(Player *ply) {	// 287aa
    if(ply->Human) {
        ply->DSOffsetX = 0;
        ply->mode1     = PLSTAT_CROUCH;
        ply->mode2     = 0;
        ply->mode3     = 0;

        ply->Attacking    = FALSE;
        ply->IsJumpThreat = FALSE;
        
        CASetAnim2(ply, STATUS_CROUCH, 2);
        proc_plstat_crouch(ply);
    } else {
        /* computer */
        ply->DSOffsetX = 0;
        comp_recovery_complete(ply);
        comp_exit_plstat_crouch(ply);   /*probably misnamed */
    }
}
void ply_exit_air(Player *ply) {		// 28862
    if(ply->Human) {
        ply->DSOffsetX    = 0;
        ply->Attacking    = FALSE;
        ply->IsJumpThreat = FALSE;
        ply->mode1		  = PLSTAT_JUMPING;   
        ply->mode2        = 4;
        ply->mode3        = 0;
        ply->LocalTimer   = 7;
        ply->Airborne     = FALSE;
        queuesound(SOUND_IMPACT8);
        CASetAnim1(ply, STATUS_LANDING);
        proc_plstat_jumping(ply);
    } else {
        /* computer */
        ply->DSOffsetX = 0;
        comp_recovery_complete(ply);
        comp_exit_land(ply); 
    }
}

	
inline short check_round_result(void) {		/* 2cd06 */
	return g.RoundResult;
}

static void _react_to_result(Player *ply, u8 plstat, u8 extra) {		// 2a7a0 dupe with PSSetRoundReact
	ply->mode1 = plstat;
	ply->mode2 = 
	ply->mode3 = 0;

	ply->PSRoundReactMode = extra;
	CASetAnim1(ply, STATUS_NORMAL);
}

void react_to_result(Player *ply) {				/* 2a76e */
	ply->DizzyStun = 0;
	ply->PSRoundReactMode = 0;		//char
	if(g.OnBonusStage) {						/* 0x2a7b8 */
		if(g.ActiveHumans == 3) {
			if(g.RoundResult < 0) {
				if(g.BonusComplete) {
					_react_to_result(ply, PLSTAT_VICTORY, 0);
				} else {
					_react_to_result(ply, PLSTAT_LOSS, 4);
				}
			} else if(g.RoundResult & (1 << ply->Side)) {
				_react_to_result(ply, PLSTAT_VICTORY, 0);
			} else if(g.TimeOut) {
				_react_to_result(ply, PLSTAT_LOSS, 4);
			} else {
				_react_to_result(ply, PLSTAT_LOSS, 2);
			}
		} else if (g.BonusComplete) {
			_react_to_result(ply, PLSTAT_VICTORY, 0);
		} else {
			_react_to_result(ply, PLSTAT_LOSS, 4);
		}
	} else {
		if(g.RoundResult < 0) {
			/* 0x2a79c draw-game */
			_react_to_result(ply, PLSTAT_LOSS, 4);
		} else if (g.RoundResult & (1 << ply->Side)) {
			_react_to_result(ply, PLSTAT_VICTORY, 0);
		} else {
			_react_to_result(ply, PLSTAT_LOSS, 2);
		}
	}
}



static void _PSSetRoundReact(Player *ply, short round_react_mode, short new_ply_stat) { /* 2cd3e */
	ply->mode1              = new_ply_stat;
	ply->PSRoundReactMode   = round_react_mode;
	ply->mode2 = ply->mode3 = 0;
	CASetAnim1(ply, 2);		/* 2 is in %D1, %D0 is a */
}



void PSStateRoundOver(Player *ply) {				/* 2cd0c */
	ply->DizzyStun        = FALSE;
	ply->PSRoundReactMode = ROUNDREACT_VICTORY;
	if(g.OnBonusStage) {							/* 2cd56 */
		if(g.ActiveHumans == 3) {					/* 2cd68 */
			/* two humans on bonus stage, need a winner */
			if(g.RoundResult < 0) {
				if(g.BonusComplete) {
					_PSSetRoundReact(ply, ROUNDREACT_LOSS1, PLSTAT_VICTORY);		
				} else {
					_PSSetRoundReact(ply, ROUNDREACT_LOSS2, PLSTAT_LOSS);
				}
			} else if(g.RoundResult & (1 << ply->Side)) {
				_PSSetRoundReact(ply, ROUNDREACT_LOSS1, PLSTAT_VICTORY);   
			} else if(g.TimeOut) {
				_PSSetRoundReact(ply, ROUNDREACT_LOSS2, PLSTAT_LOSS);
			} else {
				_PSSetRoundReact(ply, ROUNDREACT_LOSS1, PLSTAT_LOSS);
			}
		} else if(g.BonusComplete) {
				_PSSetRoundReact(ply, ROUNDREACT_LOSS1, PLSTAT_VICTORY);
		} else {
				_PSSetRoundReact(ply, ROUNDREACT_LOSS2, PLSTAT_LOSS);
		}
	} else {
		if(g.RoundResult < 0) {
				_PSSetRoundReact(ply, ROUNDREACT_LOSS2, PLSTAT_LOSS);
		} else {
			if(g.RoundResult & (1<<ply->Side)) {
				_PSSetRoundReact(ply, ROUNDREACT_LOSS1, PLSTAT_VICTORY); 
			} else {
				_PSSetRoundReact(ply, ROUNDREACT_LOSS1, PLSTAT_LOSS);
			}
		}
	}
}

#pragma mark ---- callback 28340 ----

static void ply_calc_draw_order(void) {		/* 28414 */
	if (g.x0a5c) {		/* char, bison beating Mbison at end? */
		if (g.Player1.FighterID == FID_GUILE) {
			g.PlyDrawOrder = 0;
		} else {
			g.PlyDrawOrder = 1;
		}
	} else {
		g.PlyDrawOrder = 0;
		if (g.Player1.exists && g.Player2.exists) {
			if (g.Player2.ActionScript->Priority > g.Player1.ActionScript->Priority) {	/* priority */
				g.PlyDrawOrder = 1;	
			} else if (g.Player2.ActionScript->Priority == g.Player1.ActionScript->Priority) {
				g.PlyDrawOrder = g.LastDrawOrder;
				return;
			}
		}
		g.LastDrawOrder = g.PlyDrawOrder;
	}
}

static void ply_set_direction_bonus(Player *ply) {		/* 284f6 */
	if (ply->Airborne==0) {
		if (ply->JoyDecode.full & 0x2) {
			ply->EnemyDirection=FACING_LEFT;
		} else if(ply->JoyDecode.full & 0x1) {
			ply->EnemyDirection=FACING_RIGHT;
		}
	}
}

static void ply_set_enemydirection(void) {		/* 2846e */
	int temp, d2;
	if (g.OnBonusStage) {
		if (g.PreRoundAnim == 0) {
			ply_set_direction_bonus(PLAYER1);
			ply_set_direction_bonus(PLAYER2);
		}
	} else {		
		temp = g.Player2.XPI - g.Player1.XPI;
		if (temp >= 0) { d2 = 1; } else { d2 = 0; }

		if (temp > 24 || temp < -24) {
			if (d2) {
				g.Player1.EnemyDirection = FACING_RIGHT;
				g.Player2.EnemyDirection = FACING_LEFT;
			} else {
				g.Player1.EnemyDirection = FACING_LEFT;
				g.Player2.EnemyDirection = FACING_RIGHT;
			}
		}
	}
}



inline static void _PSPlayerDelta(void) {		/* 2851c */
	/* 28528 inlined x 2 */
	g.Player1.DeltaX.full = g.Player1.X.full - g.Player1.OldX.full;
	g.Player1.DeltaY.full = g.Player1.Y.full - g.Player1.OldY.full;
	g.Player2.DeltaX.full = g.Player2.X.full - g.Player2.OldX.full;
	g.Player2.DeltaY.full = g.Player2.Y.full - g.Player2.OldY.full;
}

static void _PSBonusCarOppDistCalc(Player *ply) {			/* 285c2 */
	int x=0x90;
	
	if (ply->XPI < 0xb0) {
		x = 0xf0;
	}
	x = ABS(x - ply->XPI)-ply->Size;
	if (x<0) { x = 0;}
	ply->OppXDist = x;
}

static void _PSOppDistCalc(Player *a0, Player *a1) {		/* 2858c */
	int temp=ABS(a1->XPI - a0->XPI)-a1->Size;
	if (temp<0) {
		temp = 0;
	}
	a0->OppXDist = temp;
}

static short sub_208c8(short d2) {		// 208c8 probably belongs in actions.c
	// barrel distance calc
	struct UserData_Barrels {
		Object *barrel[8];		/* 0xb0 */
	};
	struct UserData_Barrel {
		short playerdists[2];		//x0098
	};
	
	struct UserData_Barrels *ud = (struct UserData_Barrels *)&g.x8ab6->UserData;
	typedef struct UserData_Barrel Bar;
	Bar *bar;
	
	short d0 = 0x100;
	Object *a2;
	short i;

	for (i=0;i<8;i++) {
		a2 = ud->barrel[i];
		bar = (Bar *)&a2->UserData;
		if (bar->playerdists[d2]<d0) {
			d0 = bar->playerdists[d2];
		}
	}
	return d0;
}

static void sub_208a6(void) {		// 208a6 Calc Oppdist when on Barrels
	if (g.Player1.exists) {
		g.Player1.OppXDist = sub_208c8(0);
	}
	if (g.Player2.exists) {
		g.Player2.OppXDist = sub_208c8(1);
	}
}

static void _PSCalcOppDistances(void) {
	int temp;
	temp = ABS(g.Player2.YPI - g.Player1.YPI);
	g.Player1.OppYDist = temp;
	g.Player2.OppYDist = temp;
	switch (g.CurrentStage) {
		case STAGE_BONUS_DRUMS:
			g.Player1.OppXDist = 0x200;
			g.Player2.OppYDist = 0x200;
			break;
		case STAGE_BONUS_CAR:		/* 285b6 */
			_PSBonusCarOppDistCalc(PLAYER1);
			_PSBonusCarOppDistCalc(PLAYER2);
			break;
		case STAGE_BONUS_BARRELS:
			sub_208a6();		// far away hmm... 
		default:
			/* 28578 */
			_PSOppDistCalc(PLAYER1, PLAYER2);
			_PSOppDistCalc(PLAYER2, PLAYER1);
			break;
	}
}

void ApplyPhysicsRules(void) {		/* 28340 */
	ply_calc_draw_order();				/* calc player draw order */
	ply_set_enemydirection();			/* set directions */
	_PSPlayerDelta();					/* inertia? */
	 CDCheckPushBoxes();				/* check pushboxes */
	_PSCalcOppDistances();				/* calc x/y differences */
}
	

void _RMRecoverTumble(Player *ply) {      
	/* 292f4 recovering from a knockover, if dizzy go to next state,
	 else ply_exit_stand */
	
	static const char data_29306[32]={
		0x1d, 0x7b, 0x00, 0xcc, 0x01, 0x43, 0x4a, 0x2e, 
		0x01, 0x24, 0x67, 0x24, 0x54, 0x2e, 0x00, 0x04, 
		0x42, 0x2e, 0x00, 0x05, 0x42, 0x2e, 0x01, 0x43,
		0x42, 0x2e, 0x01, 0xbf, 0x4e, 0x75, 0x61, 0x00, 
	};
	
	
    PLAYERTICK;
    if((ply->AnimFlags & 0xff) == 0) { 
		return; 
	}
    ply->NoThrow_t = data_29306[RAND32];    /* set up a randomized holdoff timer */
    if(ply->DizzyStun) {
        ply->mode2 += 2;
        ply->mode3 = 0;
        ply->NoThrow_t = 0;
        ply->Invincible = 0;
    } else {
        ply->Invincible = 0;    /* 29336 */
        ply_exit_stand(ply);   
    }
}
