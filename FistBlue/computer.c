/* computer.c */

#include <stdio.h>

#include "sf2const.h"
#include "sf2macros.h"
#include "sf2types.h"

#include "structs.h"
#include "player.h"

#include "particle.h"

#include "playerstate.h"
#include "reactmode.h"
#include "rules.h"
#include "actions.h"
#include "collision.h"
#include "sound.h"
#include "lib.h"
#include "sf2io.h"

#include "computer.h"
#include "compdata.h"

#include "ai.h"


#include "ryu.h"		/* this is also Ken for the most part */
#include "ehonda.h"
#include "blanka.h"
#include "guile.h"
#include "chunli.h"
#include "dhalsim.h"
#include "mbison.h"
#include "vega.h"


extern Game g;

void PLCBCompAttackDhalsim(Player *ply);
int	PLCBCompJumpDhalsim(Player *ply);
int PLCBCompJumpChunLi(Player *ply);
void PLCBCompAttackZangeif(Player *ply);
int PLCBCompJumpZangeif(Player *ply);
int PLCBCompJumpVega(Player *ply);
int PLCBCompJumpBalrog(Player *ply);
int PLCBCompJumpSagat(Player *ply);
int PLCBCompAttackBalrog(Player *ply);
int PLCBCompAttackSagat(Player *ply);


extern short data_2aa30[12][4][4];

static void comp_changetactics(Player *ply, short d0);	/* 2c1b4 */

static void comp_standblock(Player *ply);		/* 2cd8e */
static int check_comp_should_jump(Player *ply);	/* 2cce8 */
static void comp_set_standattack(Player *ply);
static int comp_check_block (Player *ply);	/* 2cd88 */
static int comp_check_standattack(Player *ply); /* 2cde2 */
static void comp_turn_crouch(Player *ply);	/* 0x2ce84 */
static void comp_check_set_crouch(Player *ply);	/* 0x2ceb2 */
static void comp_set_falling(Player *ply); /* 2ce0e */
static void comp_plstat_crouch (Player *ply);
static void turn_me_around(Player *ply);	/* 2ce40 */
static void process_plstat_frontend(Player *ply);

static void comp_crouchblock(Player *ply); 
static void comp_set_dunnokick(Player *ply);
static void comp_standblock(Player *ply);
static void comp_set_falling(Player *ply); /* 2ce0e */

static short comp_check_newtactics(Player *ply);
static short comp_check_platform(Player *ply);
static void comp_proc_stat(Player *ply);

static void comp_attack_plycallback (Player *ply); /* 2cc58 */

void comp_plstat_normal(Player *ply);  /* 2c2a4 */


static void CompStateGroundAttack(Player *ply, short ss);

static short check_ply_27b(Player *ply);	/* 2ccd6 */
static void comp_standattack_or_jump(Player *ply);		/* 2cbd6 */
static void comp_set_block4 (Player *ply);
static void comp_set_block5(Player *ply);
static void comp_set_block3(Player *ply);
static void comp_set_block2(Player *ply);
static void comp_set_block1(Player *ply);
static void comp_set_block6(Player *ply);
static void comp_exit_stand_from_crouch(Player *ply);	/* 2c4c8 */


void  set_comp_difficulty(Player *ply) {		/* 285ea */
	ply->Difficulty = g.CurrentDifficulty;
	LBUpdateAITimers(ply);	
}



#pragma mark ---- Checks ----
inline static int check_comp_should_jump(Player *ply) {	/* 2cce8 */
	return ply->CompDoJump;		/* x0213 char */
}
static int comp_check_block (Player *ply) {	/* 2cd88 */
	return ply->CompDoBlock;		/* x0212 char */
}
static int comp_check_standattack(Player *ply) { /* 2cde2 */
	return ply->AISigAttack;	/* x0147 */
}
static int check_my_direction(Player *ply) {		/* 2ccde */
	if(ply->EnemyDirection == ply->Flip) {
		return FALSE;
	} else {
		return TRUE;
	}
}
static int check_compDoBlockStun(Player *ply) {
	return (ply->CompDoBlockStun != 0);
}


void comp_recovery_complete(Player *ply) { /* 2b700 */
	ply->AIAllowAggressive	= FALSE;		
	ply->AIAggTimer0			= 0;
	ply->AIAggTimer1			= 0;
	ply->AISigAttack		= FALSE;	/* x0147 */
	ply->CompDoJump			= FALSE;
	ply->AICanJumpAttack	= FALSE;
	ply->CompDoBlockStun	= FALSE;
	ply->x0236				= 0;
	ply->InGroundAttack		= FALSE;
	
	ply->AIWalkDirection	|= STEP_STILL;
	
	if(ply->AIForceDefensive) {					/* x0279 */
		AIPrepareNewState(ply, ply->AITypeDef);		// 2b6f8
	} else if (ply->AIAgressive) {				/* x0200 */
		AIPrepareNewState(ply, ply->AITypeAgg1);
	} else {
		AIPrepareNewState(ply, ply->AITypeAgg0);		
	}
}
static short comp_check_newtactics(Player *ply) {				// 2c18c
	if(ply->InGroundAttack) { return 0; }
	if(ply->AIForceDefensive) { 
		if(ply->YokeAttackingMe == ply->YokeSaved) {
			return 0;		// no new attack
		} else {
			return 2;		// new attack, ForceDefensive
		}
	}
	if(ply->AIThreatFound) {return 1;}
	return 0;
}
static short comp_check_platform(Player *ply) {		// 2ccee
	if (ply->OnPlatform2 == 0) {
		return FALSE;
	}
	if (ply->OnPlatform != 0) {
		return FALSE;
	}
	ply->OnPlatform2 = 0;
	return FALSE;
}

#pragma mark ---- AI BEGIN ----

void computer_per_frame(Player *ply) {		/* 2ac70 */
	switch (ply->mode0) {
		case 0:
			NEXT(ply->mode0);
			LBInitHitBoxes(ply);
			
			ply->AIThreatCheckMode = 2;
			AIInitPlayer(ply);

			ply->Step     = STEP_STILL;
			ply->StepSave = STEP_STILL;
			ply->x023a    = data_995ae[ply->FighterID][0];	// unused?
			ply->x023c    = data_995ae[ply->FighterID][1];
			CASetAnim1(ply, STATUS_NORMAL);
			break;
		case 2:
			if (g.RoundCnt==0  & ply->FighterID == FID_M_BISON && g.x0306) {
				/* Special cape stuff */
				sub_3623e(ply);
			} else {
				/* 2acf2 */
				if (g.PreRoundAnim) {
					PLAYERTICK;
					return;
				}
				if (ply->MultiHoldoff) { ply->MultiHoldoff--; }
				if (ply->NoThrow_t)    { ply->NoThrow_t--;    }
				if(ply->ThrowStat < 0) {			/* being thrown 2acc4 */
					if(ply->DizzyClearCnt) {
						if(ply->DizzyClearCnt-- == 0){ply->DizzyCount=0;}
					}
					ply_thrown(ply);
					return;
				}
				AICheckThreats(ply);
				
				if(ply->mode1 == PLSTAT_REEL || ply->mode1 == PLSTAT_TUMBLE) {
					comp_proc_stat(ply);
					return;
				}
				/* fall through to 2ad2a */
				process_plstat_frontend(ply);
			} 
			break;
		case 4:
			RM_SM_knockedout(ply);		/* die */
			break;
		FATALDEFAULT;
	}
}

static void _comp_bounce_off_wall(Player *ply, short d0) {	//2cb86
	ply->Flip = d0;
	ply->mode2 = 6;
	ply->TmrWallBounce2 = 6;
	queuesound(SOUND_IMPACT8);
	CASetAnim1(ply, STATUS_BOUNCE_WALL);
}
static void comp_jump_physics(Player *ply) {		/* 2c854 */
	if(ply->x014e) {
		if(--ply->x014e == 0){ply->AICanJumpAttack = TRUE;}	
	}
	CATrajectory((Object *)ply);
	if(ply->VelY.full >= 0) {
		PLAYERTICK;
		return;
	}
	if(PLAYERGROUND){
		/* 2c8da */
		ply->Attacking		= FALSE;
		ply->IsJumpThreat	= FALSE;
		ply->mode2			= 4;
		ply->LocalTimer		= 7;
		ply->Airborne		= AIR_ONGROUND;
		ply->CompDoJump		= FALSE;
		queuesound(SOUND_IMPACT8);
		CASetAnim1(ply, STATUS_LANDING);
		return;
	}
	
	if(ply->AIWallBounce==TRUE) {
		if(ply->FighterID == FID_CHUN_LI || ply->FighterID == FID_VEGA) {
			if(ply->TmrWallBounce) {
				ply->TmrWallBounce--;
			} else {
				if(ply->YPI >= 104 && ply->VelX.full != 0) {
					if(ply->PlatformFallDir) {
						_comp_bounce_off_wall(ply, ply->PlatformFallDir-1);
						return;
					}
					if(ply->BoundCheck) {
						_comp_bounce_off_wall(ply, ply->BoundCheck-1);
						return;
					}
					PLAYERTICK;
				}
			}
		}
	}
	PLAYERTICK;
}

static void _AICalcTrajTick(Player *ply) {   /* 2c820 computer jump tracjectory */
	CATrajectory((Object *)ply);
	PLAYERTICK;
}

static void comp_jump_dhalsimcheck(Player *ply) {		/* 2c828 */
	if (ply->FighterID == FID_DHALSIM && ply->CompDoThrow != 0) {
		if(sub_35fe6(ply) < 0) {		/*Dhalsim something */
			/* 2c804 */
			ply->Attacking		= TRUE;
			ply->IsJumpThreat	= TRUE;
			ply->mode2			= 0;
			ply->mode3			= 0;
			if(ply->Timer2 == 0){
				_AICalcTrajTick(ply);
			}
			return;
		}
	}
	comp_jump_physics(ply);
}

#pragma mark ---- Per-Avatar Callbacks ----

static short comp_jump_plycallback(Player *ply) { /* 2c9be */
	int (*data_2c9cc[])(Player *) = {
		PLCBCompJumpRyuKen,
		PLCBCompJumpEHonda,
		PLCBCompJumpBlanka,
		PLCBCompJumpGuile,
		PLCBCompJumpRyuKen,
		PLCBCompJumpChunLi,			// chunli
		PLCBCompJumpZangeif,
		PLCBCompJumpDhalsim,
		PLCBCompJumpMBison,
		PLCBCompJumpSagat,
		PLCBCompJumpBalrog,
		PLCBCompJumpVega,
	};
	return data_2c9cc[ply->FighterID](ply);
}

static void comp_footsweep_or_jump(Player *ply) {		/* 2cbc4 */
	if(comp_check_standattack(ply)){
		CompStateGroundAttack(ply, PLY_CROUCH); 
	} else if(check_comp_should_jump(ply)){
		comp_set_jump(ply);
	}
}


#pragma mark ---- comp_plstat_* ----

void comp_plstat_normal(Player *ply) {  /* 2c2a4 */
	short temp;
	extern short data_2abb0[12][8];
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->Airborne			= AIR_ONGROUND;
			ply->Attacking			= FALSE;
			ply->IsJumpThreat		= FALSE;
			ply->Path = data_2abb0[ply->FighterID];	
			CASetAnimWithStep(ply, STATUS_STAND);
			break;
		case 2:
			/* 2c2e2 */
			ply->Step = ply->AIWalkDirection;
			if(check_round_result()) {
				PSStateRoundOver(ply); 
			} else if(check_my_direction(ply)) {
				turn_around(ply);	/* bright eyes */
			} else if(comp_check_block(ply)) {
				comp_check_set_crouch(ply);
			} else if(temp = comp_check_newtactics(ply)) {
				comp_changetactics(ply,temp);
			} else if(comp_check_standattack(ply)) {
				CompStateGroundAttack(ply, PLY_STAND);
			} else if(check_comp_should_jump(ply)) {
				comp_set_jump(ply); 
			} else if(check_compDoBlockStun(ply)) { 
				comp_standblock(ply);
			} else if(comp_check_platform(ply)) { 
				comp_set_falling(ply);
			} else if(ply->Step ^ ply->StepSave) {
				ply->StepSave = ply->Step;
				CASetAnimWithStep(ply, STATUS_STAND);
			}
			PLAYERTICK;
			update_obj_path((Object *)ply);
			
			break;
		FATALDEFAULT;
	}
}
static void comp_plstat_crouch (Player *ply) {		/* 2c682 was comp_disposition_1*/
	short temp;
	switch (ply->mode2) {
		case 0:
			PLAYERTICK;
			if (check_round_result()) {
				PSStateRoundOver(ply);
			} else if (check_my_direction(ply)) {
				turn_me_around(ply);
			} else if(comp_check_block(ply)) {
				/* 2c6f0 */
				if(temp=comp_check_newtactics(ply)) {
					comp_changetactics(ply,temp);
				} else if(comp_check_standattack(ply)){
					CompStateGroundAttack(ply, PLY_CROUCH);
				} else if(check_compDoBlockStun(ply)){
					comp_standblock(ply);
				}
			} else if(temp=comp_check_newtactics(ply)) {
				comp_changetactics(ply,temp);
			} else if(comp_check_standattack(ply)){
				CompStateGroundAttack(ply, PLY_STAND);
			} else if(check_comp_should_jump(ply)){
				comp_set_jump(ply);
			} else if(check_compDoBlockStun(ply)){
				comp_standblock(ply);
			} else if((ply->AnimFlags & 0xff) != 2) {
				NEXT(ply->mode2) 
				CASetAnim2(ply, STATUS_STAND_UP, ply->AnimFlags & 0xff);
			}
			break;
		case 2:
			PLAYERTICK;
			if (check_round_result()) {
				PSStateRoundOver(ply);
			} else if (check_my_direction(ply)) {
				turn_me_around(ply);
			} else if (comp_check_block(ply)) {
				if(temp=comp_check_newtactics(ply)){
					comp_changetactics(ply,temp);
				} else if(comp_check_standattack(ply)) {
					CompStateGroundAttack(ply, PLY_CROUCH);
				} else if(check_compDoBlockStun(ply)){
					comp_standblock(ply);
				} else if(ply->Flip != 2) {
					ply->mode2=0;
					CASetAnim2(ply, STATUS_CROUCH, AF2);
				}
			} else {
				/* 2c75e */
				if(temp=comp_check_newtactics(ply)){
					comp_changetactics(ply,temp);
				} else if(comp_check_standattack(ply)){
					CompStateGroundAttack(ply, PLY_CROUCH);
				} else if(check_comp_should_jump(ply)){
					comp_set_jump(ply);
				} else if(check_compDoBlockStun(ply)){
					comp_standblock(ply);
				} else if(AF1) {
					exit_comp_normal(ply);
				}
			}
			break;
		FATALDEFAULT;
			
	}
}
static void comp_plstat_jump(Player *ply) { /* 2c788 was comp_desire_jump */
	int d0;
	switch (ply->mode2) {
		case 0:
			PLAYERTICK;		/* tick through the initial jump anim */
			if ((AF2) == 0) {
				NEXT(ply->mode2);			/* have 'lifted off' */
				ply->Airborne    = AIR_JUMPING;
				ply->OnPlatform2 = FALSE;
			}
			break;
		case 2:								/* in the air */
			if (ply->AICanJumpAttack) {
				if (ply->Opponent->YPI >= 56) {
					/*2cf74*/
					if(ply->OppXDist > ply->x0228) {comp_jump_dhalsimcheck(ply);return;}
				} else {
					if(ply->VelY.full >= 0 && ply->YPI > ply->x023c) {
						comp_jump_dhalsimcheck(ply);       /* 2c828 */
						return;
					}
					if(ply->OppXDist > ply->x0238) {
						comp_jump_dhalsimcheck(ply);
						return;
					}
				}
				/* 2c7fe */
				if(comp_jump_plycallback(ply) >= 0){
					/* comp attacks mid-air    2c842		*/ 
					ply->Attacking		= TRUE;
					ply->IsJumpThreat	= TRUE;
					ply->mode2			= 8;
					
					comp_jump_physics(ply);
					return;
				}
				ply->Attacking = TRUE;
				ply->IsJumpThreat     = TRUE;
				ply->mode2 = ply->mode3 = 0;
				if (ply->Timer2 == 0) {
					_AICalcTrajTick(ply);
				}
			} else {
				/* 2c854 */
				comp_jump_physics(ply);
				return;
			}
			break;
		case 4:
			if (check_round_result()) {
				PSStateRoundOver(ply);
			} else if (check_my_direction(ply)) {
				turn_me_around(ply);
			} else if (comp_check_block(ply)) {
				/* 2c94a ... */
				ply->LocalTimer = 0;
				if (d0=comp_check_newtactics(ply)) {
					comp_changetactics(ply, d0);
				} else if (comp_check_standattack(ply)) {
					CompStateGroundAttack(ply, PLY_CROUCH);
				} else if (check_compDoBlockStun(ply)) {
					comp_standblock(ply);
				} else {
					comp_check_set_crouch(ply);
				}
			} else {
				if (d0=comp_check_newtactics(ply)) {
					comp_changetactics(ply, d0);
				} else if (comp_check_standattack(ply)) {
					CompStateGroundAttack(ply, PLY_STAND);
				} else if (check_comp_should_jump(ply)) {
					comp_set_jump(ply);
				} else if (check_compDoBlockStun(ply)) {
					comp_standblock(ply);
				} else {
					--ply->LocalTimer;
					exit_comp_normal(ply);
				}
			}
			break;
		case 6:
			if (--ply->TmrWallBounce2 == 0) {
				ply->mode2 = 2;
				ply->VelX.full = 0x0400;
				ply->AclX.full = 0x0005;
				ply->VelY.full = 0x0600;
				ply->AclY.full = 0x0048;
				ply->TmrWallBounce = 9;
				ply->Flip ^= 1;
				if (ply->Flip == FACING_LEFT) {
					ply->VelX.full = -ply->VelX.full;
					ply->AclX.full = -ply->AclX.full;
				}
				CASetAnim1(ply, 0x4e);
			}
			break;
		case 8:		/* Attacking - don't do anything until timer runs out */
			if (ply->Timer2) {
				ply->Timer2--;
			} else {
				comp_jump_physics(ply);
			}
			break;
		FATALDEFAULT;
	}
}
static void comp_plstat_turnaround(Player *ply) {	/* 2c9fe */
	short temp;
	switch (ply->mode2) {
		case 0:
			PLAYERTICK;
			if(check_round_result()) {PSStateRoundOver(ply); return;}
			if(comp_check_block(ply)) {
				/* 2ca54(ply); */
				NEXT(ply->mode2);
				if(temp=comp_check_newtactics(ply)) {comp_changetactics(ply,temp);return;}
				if(comp_check_standattack(ply)){CompStateGroundAttack(ply, PLY_CROUCH); return;}
				if(check_compDoBlockStun(ply)){comp_standblock(ply); return;}
				if(AF1) {
					exit_to_compdisp1(ply);
					return;
				}
				CASetAnim2(ply, STATUS_CROUCH_TURN, AF2);
				return;
			}
			if(ply->AIWalkDirection & 0x1 == 0) {
				exit_comp_normal(ply);
				return;
			}
			COMP_CHK_TACTICS
			if(comp_check_standattack(ply)){CompStateGroundAttack(ply, PLY_STAND); return;}
			if(check_comp_should_jump(ply)){comp_set_jump(ply); return;}
			if(check_compDoBlockStun(ply)){comp_standblock(ply); return;}
			if(AF1) {
				exit_comp_normal(ply);
			}
			break;
		case 2:
			/* 2ca8e */
			PLAYERTICK;
			if(check_round_result()) {PSStateRoundOver(ply); return;}
			if(comp_check_block(ply)) {				
				/* 2cac2 */
				COMP_CHK_TACTICS
				if(comp_check_standattack(ply)){CompStateGroundAttack(ply, PLY_STAND); return;}
				if(check_comp_should_jump(ply)){comp_set_jump(ply); return;}
				if(check_compDoBlockStun(ply)){comp_standblock(ply); return;}
				if(AF1) {
					exit_comp_normal(ply);
					return;
				}
				CASetAnim2(ply, STATUS_TURN_AROUND, AF2);
				return;
			}
			if(temp=comp_check_newtactics(ply)) {comp_changetactics(ply,temp);return;}
			if(comp_check_standattack(ply)){CompStateGroundAttack(ply, PLY_CROUCH); return;}
			if(check_compDoBlockStun(ply)){comp_standblock(ply); return;}
			if(AF1) {
				exit_to_compdisp1(ply);
				return;
			}
			break;
		FATALDEFAULT;
	}
	
}
static void sub_2cc96(Player *ply) { /* really does nothing */ }
static void comp_plstat_block(Player *ply) {		// 2cb04
	short temp;
	switch (ply->mode2) {
		case 0:
			if(check_round_result())		{PSStateRoundOver(ply); return;}
			if(check_my_direction(ply))		{turn_me_around(ply); return;}
			PLAYERTICK;
			if(temp=comp_check_newtactics(ply))			{comp_changetactics(ply, temp); return;}
			if(check_compDoBlockStun(ply))	{comp_set_block2(ply);return;}
			if(check_ply_27b(ply))			{comp_set_block1(ply);return;}
			comp_standattack_or_jump(ply);
			break;
		case 2:										// 2cbe8 
			PLAYERTICK;
			if(AF1) {
				exit_comp_normal(ply);
				return;
			}
			COMP_CHK_TACTICS
			if(check_compDoBlockStun(ply))	{comp_set_block6(ply); return;}
			if(check_ply_27b(ply))			{comp_set_block4(ply);return;}
			comp_footsweep_or_jump(ply);
			break;
		case 4:
			/* 2cc0c */
			PLAYERTICK;
			if(AF1){exit_comp_normal(ply);return;}
			COMP_CHK_TACTICS
			COMP_CHK_BLOCKSTUN {comp_set_block2(ply); return;}
			COMP_CHK_27BZ      {comp_set_block4(ply); return;}
			comp_standattack_or_jump(ply);
			break;
		case 6:
			/* 2cc2a */
			PLAYERTICK;
			if(AF1){exit_to_compdisp1(ply);return;}
			COMP_CHK_TACTICS
			COMP_CHK_BLOCKSTUN {comp_set_block6(ply); return;}
			COMP_CHK_27BZ      {comp_set_block5(ply); return;}
			comp_footsweep_or_jump(ply);
			break;
		case 8:
			/* 2cc52 */
			// Vega climbing wall?
			// XXX sub_37e2e(ply);
			break;
		FATALDEFAULT;
	}
}
static void comp_proc_stat(Player *ply) {		/* 2c242 process plstat */
	static void (*sub_select_2c28e[])(Player *ply) = {
		comp_plstat_normal,
		comp_plstat_crouch,
		comp_plstat_jump,
		comp_plstat_turnaround,
		comp_plstat_block,			/* blocking ? */
		comp_attack_plycallback,
		sub_2cc96,					/* powermove, does nothing */
		proc_plstat_taking_hit,		/* same as for human from here */
		proc_plstat_victory,		/* victory */		
		proc_plstat_victory,
		proc_plstat_thrown_recovery,
	};
	
	if(g.FastEndingFight || g.OnBonusStage) {
		if (ply->EnergyDash != ply->Energy) {
			ply->Energy ^= 1;
		}
	}
	/* 2c260 */
	sub_select_2c28e[ply->mode1/2](ply);
	
	if (ply->mode0 == 2) {
		if(ply->DizzyClearCnt){
			ply->DizzyClearCnt--;
			if (ply->DizzyClearCnt == 0) {
				ply->DizzyCount=0;
			}
		} else {
			ply->DizzyCount = 0;
		}
		player_postamble(ply);		// damage / reward stuff in playerstate.c
	}
}	


static void turn_me_around(Player *ply) {	/* 2ce40 */
	short temp;
	ply->Flip =ply->EnemyDirection;
	if(comp_check_block(ply)) { comp_turn_crouch(ply); return; }
	COMP_CHK_TACTICS
	if(check_comp_should_jump(ply)) { comp_set_jump(ply); return;}
	if(check_compDoBlockStun(ply)) { comp_standblock(ply); return;}
	if(ply->AIWalkDirection & 0x1) {
		ply->mode1 = PLSTAT_TURNAROUND;
		ply->mode2 = ply->mode3 = 0;
		CASetAnim1(ply, STATUS_TURN_AROUND);		/* 0xc */
	} else {
		exit_comp_normal(ply);
	}
}
static short check_ply_27b(Player *ply) {	/* 2ccd6 */
	return (ply->x027b == 1);
}
	
static void comp_standattack_or_jump(Player *ply) {		/* 2cbd6 */
	if(comp_check_standattack(ply)){CompStateGroundAttack(ply, PLY_STAND); return;}
	if(check_comp_should_jump(ply)){comp_set_jump(ply); return;}
}

#pragma mark ---- Setstatus ----

static void comp_set_block4 (Player *ply) {
	ply->mode2=0;
	CASetAnim2(ply, STATUS_STAND_BLOCK, AF2);		// standing block
	comp_standattack_or_jump(ply);
}
static void comp_set_block5(Player *ply) {
	ply->mode2=2;
	CASetAnim2(ply, STATUS_STAND_BLOCK2, AF2);		// standing block, but crouch down
	comp_standattack_or_jump(ply);	
}
static void comp_set_block3(Player *ply) {
	ply->mode2=6;
	CASetAnim2(ply, STATUS_CROUCH_BLOCK2, AF2);		// crouching block
	comp_standattack_or_jump(ply);
}
static void comp_set_block2(Player *ply) {
	if(check_ply_27b(ply)){comp_set_block3(ply);return;}
	comp_set_block5(ply);
}
static void comp_set_block1(Player *ply) {
	ply->mode2=4;
	CASetAnim2(ply, STATUS_CROUCH_BLOCK, AF2);		// another crouchblock
	comp_footsweep_or_jump(ply);
}
static void comp_set_block6(Player *ply) {
	if(check_ply_27b(ply)){
		comp_set_block1(ply); 
	} else {
		comp_set_block4(ply);
	}
}

static void comp_attack_plycallback (Player *ply) { /* 2cc58 */
	void (*data_2cc66[])(Player *)={
		PLCBCompAttackRyuKen,
		PLCBCompAttackEHonda,			//sub_33016,
		PLCBCompAttackBlanka,
		PLCBCompAttackGuile,
		PLCBCompAttackRyuKen,
		PLCBCompAttackChunLi,
		PLCBCompAttackZangeif,
		PLCBCompAttackDhalsim,
		PLCBCompAttackMBison,
		PLCBCompAttackSagat,
		PLCBCompAttackBalrog,
		PLCBCompAttackVega,
	};
	data_2cc66[ply->FighterID](ply);	
}

#pragma mark ---- Comp NextAction ----


short comp_setnextaction(Player *ply) {		/* 2c5de */
	if(ply->ActionScript->Crouch) {
		/* 2c5c4 */
		
		if(check_round_result()) { 
			ply->NextAction = NEXTACT_ROUNDOVER;
			return 1;
		}
		ply->NextAction = NEXTACT_TURNAROUND;
		check_my_direction(ply);			/* no conditional */
		/* go to 2c62e */
		if(check_comp_should_jump(ply)) { 
			ply->NextAction = NEXTACT_JUMP;
			return 1; 
		}
		ply->NextAction = NEXTACT_STANDUP;
		if(comp_check_block(ply)==0) { return 1; }
		if(ply->x0216 == FALSE || ply->Opponent->ActionScript->Crouch == 0) {
			ply->CompDoBlock = TRUE;
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		if(check_round_result()) { 
			ply->NextAction = NEXTACT_ROUNDOVER;
			return TRUE; 
		}
		if(check_my_direction(ply)) { 
			ply->NextAction = NEXTACT_TURNAROUND;
			return TRUE; 
		}
		if(comp_check_block(ply)) { 
			ply->NextAction = NEXTACT_CROUCHDOWN;
			return TRUE; 
		}
		if(ply->x0216 == FALSE || ply->Opponent->ActionScript->Crouch == 0) {
			if(check_comp_should_jump(ply)) { 
				ply->NextAction = NEXTACT_JUMP;
				return TRUE; 
			}
		}
		return FALSE;
	}
}
void comp_do_next_action(Player *ply) {		/* 2c668 */
	switch (ply->NextAction) {
		case 0:		exit_comp_normal(ply);		             break;
		case NEXTACT_ROUNDOVER:	 PSStateRoundOver(ply);      break;
		case NEXTACT_TURNAROUND: turn_me_around(ply);        break;
		case NEXTACT_CROUCHDOWN: comp_check_set_crouch(ply); break;
		case NEXTACT_JUMP:		 comp_set_jump(ply);		 break;
		case NEXTACT_STANDUP:    comp_exit_stand_from_crouch(ply); break;
		FATALDEFAULT;
	}
}

/* process_plstat_frontend (ply)
 Perform AI computations, then process the ply->mode1
 */
static void process_plstat_frontend(Player *ply) {		/* 2ad2a */
	if(ply->AIForceDefensive) {		
		AIBeginDef(ply);
	} else {											/* 2ad3a */
		switch (ply->AIAgressive) {
			case 0:
				AIBeginAgg0(ply);
				break;
			case 2:
				AIBeginAgg1(ply);
				break;
			FATALDEFAULT;
		}
	}
	comp_proc_stat(ply);
}
static void comp_changetactics(Player *ply, short d0) {	/* 2c1b4 */
	ply->YokeSaved = ply->YokeAttackingMe;
	if(d0 != 2) {	/* Save current AI state for later retrieval, and set AIForceDefensive */
		ply->x0209 = ply->AIStrategy;
		ply->x020a = ply->AIParam1;
		ply->x020b = ply->AIParam2;
		ply->x020c = ply->AIParam3;
		ply->x020d = ply->AIParam4;
		ply->x0218 = ply->AIAgressive;
		ply->x0219 = ply->AIMode1;
		ply->AIForceDefensive = TRUE;
	}
	AIInitDefensive(ply);
	ply->AIAllowAggressive = ply->AIAggTimer0 = ply->AIAggTimer1 = ply->AISigAttack = ply->CompDoJump =
	ply->AICanJumpAttack = ply->AIMode1 = ply->AIMode2 = ply->CompDoBlockStun = ply->x0236 = 0;
	
	ply->AIWalkDirection |= STEP_STILL;
	
	printf("comp_changetactics side %d", ply->Side);
	if(g.Debug && g.JPCost & JP_DBGTACTICS) {
		queuesound(SOUND_CHALLENGER);
	}
	process_plstat_frontend(ply);
}

#pragma mark ---- COMP setters ----

short comp_diceroll(Player *ply) {	// 2c34a
	if (ply->AIVolley == FALSE || ply->DiceRollCount == 0) {
		ply->AIVolley = FALSE;
		return FALSE;
	} 
	if(data_2c388[ply->Difficulty] & (1<<RAND32)) { /* u32s */
		--ply->DiceRollCount;
		ply->Opponent->SufferHB5 = 0;
		return TRUE;
	} 
	return FALSE;
}
static void sub_2c4fa(Player *ply) {
	ply->mode1 = PLSTAT_STANDBLOCK;
	ply->mode2 = 0;
	if (ply->ActionScript->Crouch) {
		ply->mode2 = 4;
	}
}

#pragma mark ---- Exit to plstat ----
/* setstatus and plstat_* */	


void comp_exit_land(Player *ply) {		/* 2c57a was comp_exit_plstat_jump */
	set_ply_directions(ply);
	ply->Attacking		= FALSE;
	ply->IsJumpThreat	= FALSE;
	ply->mode1			= PLSTAT_JUMPING;
	ply->mode2			= 4;
	ply->LocalTimer		= 7;
	ply->Airborne		= AIR_ONGROUND;
	queuesound(SOUND_IMPACT8);
	CASetAnim1(ply, STATUS_LANDING);
	slib_ply_overlap();
	process_plstat_frontend(ply);
}
void exit_to_compdisp1(Player *ply) { /* 2c464 */
	set_ply_directions(ply);
	ply->mode1			= PLSTAT_CROUCH;
	ply->Attacking		= 0;
	ply->IsJumpThreat	= 0;
	ply->mode2			= 0;
	ply->mode3			= 0;
	CASetAnim2(ply, STATUS_CROUCH, 2);
	slib_ply_overlap();
	comp_plstat_crouch(ply);
}
static void comp_exit_stand_from_crouch(Player *ply) {	/* 2c4c8 */
	set_ply_directions(ply);
	ply->mode1			= PLSTAT_CROUCH;
	ply->mode2			= 2;				/* standing up */
	ply->Attacking		= FALSE;
	ply->IsJumpThreat	= FALSE;
	ply->mode3			= 0;
	CASetAnim1(ply, STATUS_STAND_UP);
	slib_ply_overlap();
	comp_plstat_crouch(ply);
}

void sub_2c516(Player *ply){			// 2c516 checked one external caller
	ply->mode1=PLSTAT_STANDBLOCK;
	ply->mode2 = (ply->ActionScript->Crouch ? 4 : 0);
	process_plstat_frontend(ply);
}	

void comp_setjumping_main(Player *ply) {	/* 2c534 */
	set_ply_directions(ply);
	ply->Attacking		= ply->IsJumpThreat = 0;
	ply->mode1			= PLSTAT_JUMPING;
	ply->mode2			= 4;
	ply->mode3			= 0;
	ply->LocalTimer		= 7;
	ply->Airborne		= AIR_ONGROUND;
	queuesound(SOUND_IMPACT8);
	CASetAnim1(ply, STATUS_LANDING);
	slib_ply_overlap();
	comp_plstat_jump(ply);
}
void exit_comp_normal(Player *ply) {	/* 0x2c408 */
	set_ply_directions(ply);			/* 2f8a */
	ply->Step |= STEP_STILL;
	ply->StepSave = ply->Step;
	ply->Attacking    = FALSE;
	ply->IsJumpThreat = FALSE;
	ply->mode1 = 0;
	ply->mode2 = 0;
	slib_ply_overlap(); 
	comp_plstat_normal(ply);
}
void exit_comp_plstatfe_normal(Player *ply) {	/* 2c436 */
	/* recover from a throw or reel */
	set_ply_directions(ply);
	ply->Step |= STEP_STILL;
	ply->StepSave = ply->Step;
	ply->Attacking = ply->IsJumpThreat = 0;
	ply->mode1 = PLSTAT_NORMAL;
	ply->mode2 = 0;
	slib_ply_overlap();
	process_plstat_frontend(ply);
}
void comp_exit_plstat_crouch(Player *ply) {	/* 0x2c496  */
	// was 'comp_exit_plstat' one caller
	set_ply_directions(ply);
	ply->mode1 = PLSTAT_CROUCH;
	ply->Attacking    = 0;
	ply->IsJumpThreat = 0;
	ply->mode2 = ply->mode3 = 0;
	CASetAnim2(ply, STATUS_CROUCH, 2);
	slib_ply_overlap();
	process_plstat_frontend(ply);
}

#pragma mark Comp Setstatus
/* setstatus but no plstat */

static void comp_turn_crouch(Player *ply) { /* 0x2ce84 1 caller */
	short temp;
	ply->Flip = ply->EnemyDirection;
	if(temp=comp_check_newtactics(ply))	{comp_changetactics(ply,temp); return;}
	if(check_compDoBlockStun(ply)) {comp_standblock(ply); return;}
	ply->mode1 = PLSTAT_TURNAROUND;
	ply->mode2 = 2;
	ply->mode3 = 0;
	CASetAnim1(ply, STATUS_TURN_AROUND);		/* 0xe */
}
static void comp_check_set_crouch(Player *ply) { /* 2ceb2 3 callers */
	short temp;
	if(temp=comp_check_newtactics(ply)) {
		comp_changetactics(ply,temp);
	} else if(check_compDoBlockStun(ply)) {
		comp_standblock(ply);
	} else {
		ply->mode1 = PLSTAT_CROUCH;
		ply->mode2 = 0;
		ply->mode3 = 0;
		CASetAnim1(ply, STATUS_CROUCH);
	}
}
static void comp_set_falling(Player *ply) { /* 2ce0e */
	ply->mode1 = PLSTAT_JUMPING;
	ply->mode2 = 2;
	ply->mode3 = 0;
	ply->VelX.full = 0;
	ply->VelY.full = 0;
	ply->AclY.full = 0x0048;
	ply->AclX.full = 0;
	ply->Airborne = AIR_JUMPING;
	CASetAnim1(ply, STATUS_FALLING);
}
void comp_set_jump(Player *ply) {		/* 2ceda was comp_jumps */
	short *traj;
	u8 jumpindex;
	
	ply->mode1 = PLSTAT_JUMPING;
	ply->mode2 = ply->mode3 = 0;
	
	jumpindex = ply->AIJumpSel;		// is a byte read of x0227 in asm
	if(ply->Flip) { jumpindex ^= 0x1; }
	ply->Step = jumpindex;
	jumpindex ^= ply->Flip;
	
	// todo: cast need removing when data is fixed, reread as signed shorts.
	traj = data_2aa30[ply->FighterID][jumpindex];
	
	ply->VelX.full = traj[0];
	ply->AclX.full = traj[1];
	ply->VelY.full = traj[2];
	ply->AclY.full = traj[3];
	
	if(ply->Flip != FACING_LEFT) {
		ply->VelX.full   = -ply->VelX.full;
		ply->AclX.full   = -ply->AclX.full;
	}
	CASetAnimWithStep(ply, STATUS_JUMP_START);
}
static void CompStateGroundAttack(Player *ply, short StandSquat) {		/* 2cdee */
	ply->mode1 = PLSTAT_ATTACKING;
	ply->Attacking = TRUE;
	ply->StandSquat = StandSquat;
	ply->mode2 = 0;
	ply->mode3 = 0;
	ply->InGroundAttack = TRUE;	/* x0161 */
}
static void comp_crouchblock(Player *ply) {	
	ply->mode1 = PLSTAT_STANDBLOCK;
	ply->mode2 = 2;
	ply->mode3 = 0;
	CASetAnim1(ply, STATUS_CROUCH_BLOCK);		// crouch block
}
static void comp_set_dunnokick(Player *ply) { /* 2cdca */
	ply->mode1 = PLSTAT_STANDBLOCK;		
	ply->mode2 = 8;
	ply->mode3 = 0;
	CASetAnim1(ply, STATUS_KICK);
}
static void comp_standblock(Player *ply) {		/* 2cd8e */
	if(ply->x0236) {
		comp_set_dunnokick(ply);	/* 2cdca */
		return;
	}
	if(ply->x027b == 2) {comp_crouchblock(ply); return; }
	ply->mode1 = PLSTAT_STANDBLOCK;
	ply->mode2 = 0;
	ply->mode3 = 0;
	CASetAnim1(ply, STATUS_STAND_BLOCK);
}

