/* reactmode.c */

#include "sf2.h"

#include "player.h"

#include "playerstate.h"
#include "particle.h"
#include "sound.h"

#include "lib.h"
#include "rules.h"
#include "reactmode.h"
#include "sprite.h"
#include "actions.h"
#include "gfxlib.h"

#include "computer.h"


#define	M_TUMBLE_POST						\
case 2:	RMTumble(ply);			break;		\
case 4: _RMGetBackUp(ply);		break;		\
case 6: _RMRecoverTumble(ply);  break;		\
case 8: PSDizzyState(ply);		break;

#define	M_THROW_POST						\
case 2:	ply_throwtumble(ply);	break;		\
case 4: _RMGetBackUp(ply);		break;		\
case 6: _RMRecoverTumble(ply);  break;		\
case 8: PSDizzyState(ply);		break;


static const u16 reactmode_to_status[20]={		/* 28f96 Stun anim selectors per reactmode */
	STATUS_STUN1,				STATUS_STUN2,
	STATUS_STUN2,				STATUS_STAND_BLOCK_FREEZE,
	STATUS_CROUCH_BLOCK_FREEZE, STATUS_CROUCH_STUN,
	STATUS_STUN2,				STATUS_STUN2,
	STATUS_STUN2,				STATUS_STUN2,
	STATUS_STUN1,				STATUS_STUN2,
	STATUS_STUN1,				STATUS_STUN1,
	STATUS_STUN1,				STATUS_STUN1,
	STATUS_STUN1,				STATUS_STUN1,
	STATUS_STUN1,				STATUS_STUN2,
};

extern struct smalladjust data_29126[];


/* pushbacks, move to playerdata.h ? */
extern char data_29144[];
extern char data_29150[];
extern char data_29162[];

extern char *data_2913e[];

/* randomly selected, copied to ply->NoThrow_t */
extern char data_292d4[];


static void _RMGetBackUp(Player *ply);
static void _RMTumbleSM3(Player *ply);		/* 2a21a tumble sm */


static void _RMKOSound(Player *ply) {		/* 29c66 */
	if (ply->FighterID == FID_CHUN_LI) {
		queuesound(SOUND_KO_FEMALE);
	} else {
		queuesound(SOUND_KO_MALE);
	}		
}

static void tumble_until_still(Player *ply) {		// 29cfe
	/* inlined 29cfe */
	SMTumble(ply);
	if (ply->Tumble == 0) {
		NEXT(ply->mode2);
		ply->PSFinishedParticipating = TRUE;
	}
}
static void choose_blockstun(Player *ply) {		// 29060
	if(ply->ReactMode == RM_0x06 || ply->ReactMode == RM_0x08) {
		if(ply->Human) {
			if(is_holding_down(ply)){
				CASetAnim1(ply, STATUS_STAND_BLOCK_FREEZE);
			} else {
				CASetAnim1(ply, STATUS_CROUCH_BLOCK_FREEZE);
			}
		} else {
			if(ply->ActionScript->Crouch) {
				/* 0x290e4 */
				if(0x06 == ply->Opponent->ActionScript->Yoke || 
				   0x11 == ply->Opponent->ActionScript->Yoke || 
				   0x15 == ply->Opponent->ActionScript->Yoke || 
				   0x17 == ply->Opponent->ActionScript->Yoke || 
				   0x18 == ply->Opponent->ActionScript->Yoke) {
					CASetAnim1(ply, STATUS_STAND_BLOCK_FREEZE);
				}
			} else {
				/* 0x29094 */
				if(0x04 == ply->Opponent->ActionScript->Yoke || 
				   0x05 == ply->Opponent->ActionScript->Yoke || 
				   0x10 == ply->Opponent->ActionScript->Yoke || 
				   0x14 == ply->Opponent->ActionScript->Yoke || 
				   0x16 == ply->Opponent->ActionScript->Yoke || 
				   0x12 == ply->Opponent->ActionScript->Yoke || 
				   0x13 == ply->Opponent->ActionScript->Yoke ) {
					/* 2911e */
					CASetAnim1(ply, STATUS_CROUCH_BLOCK_FREEZE);
				}
			}
		}
	}
}


void RM_SM_knockedout(Player *ply) {			// 29c4c
	switch (ply->mode1) {
		case DIESTAT_0:			/* 29c7a */
		case DIESTAT_4:
		case DIESTAT_12:
			/* set a trajectory, make KO sound, set PSFinishedParticipating when still 
			 TUMBLE_0x26 */
			switch (ply->mode2) {
				case 0:			
					NEXT(ply->mode2);
					ply->mode3 = 0;
					_RMKOSound(ply);
					PLY_SETTRAJ(ply, 0x300, 0x0, 0x700, 0x48,
								0x280, 0x0, 0x300, 0x48,
								0x100, 0x14, 0x0, 0x0);
					ply->TumbleStatus = STATUS_KNOCKDOWN;
					if (ply->mode1 != 0) {
						ply->ProjHit = 1;
						if (ply->mode1 == DIESTAT_12) {
							ply->ProjHit = 2;
						}
					}
					/* FALLTHRU */
				case 2:
					tumble_until_still(ply);		/* sub declaration here */
					break;
				case 4:
					PLAYERTICK;
					break;
				FATALDEFAULT;
			}
			break;
		case DIESTAT_2:
			/* already lying down, just set PSFinishedParticipating */
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->PSFinishedParticipating = TRUE;
					/* FALLTHRU */
				case 2:
					PLAYERTICK;
					break;
				FATALDEFAULT;
			}
			break;
		case DIESTAT_ELECTROCUTED:
			/* Electrocuted -> KO no TUMBLE_ */
			switch (ply->mode2) {
				case 0:			/* 29d4c */
					NEXT(ply->mode2);
					ply->LocalTimer = 1 * TICKS_PER_SECOND;
					CASetAnim1(ply, STATUS_ELECTROCUTED);	
					_RMKOSound(ply);
					break;
				case 2:			/* 29d62 */
					if (--ply->LocalTimer) {
						PLAYERTICK;
					} else {
						NEXT(ply->mode2);
						ply->mode3 = 0;
						PLY_SETTRAJ(ply, 0x300, 0, 0x700, 0x48,
									0x280, 0x0, 0x300, 0x48,
									0x100, 0x14, 0, 0x26);
						ply->VelY.full = 0;
						ply->Timer2 = 0;
						SMTumble(ply);
						NEXT(ply->mode2);
						CASetAnim1(ply,STATUS_ELECTROCUTED);	
					}
					break;
				case 4:		/* 29dda */
					SMTumble(ply);
					if (ply->VelY.full < 0) {
						NEXT(ply->mode2);
						CASetAnim2(ply, STATUS_KNOCKDOWN, 0);
					}
					break;
				case 6:
					tumble_until_still(ply);
					break;
				case 8:
					PLAYERTICK;
					/* 29d14 inlined */
					break;
				FATALDEFAULT;
			}
			break;
		case DIESTAT_8:
			/* 29df4 KOSound, Trajectory,  TUMBLE_0x32 */
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->mode3 = 0;
					_RMKOSound(ply);
					PLY_SETTRAJ(ply, 0x300, 0, 0x700, 0x48,
								0x280, 0, 0x300, 0x48,
								0x100, 0x14, 0, 0);
					ply->TumbleStatus = STATUS_TUMBLE_32;
					/* FALLTHRU */
				case 2:
					tumble_until_still(ply);
					break;
				case 4:
					PLAYERTICK;
					break;
				FATALDEFAULT;
			}
			break;
		case DIESTAT_10:		/* 0xa */
			/* 29de64 KOSound, reverse Traj, TUMBLE_0x34 */
			switch (ply->mode2) {
				case 0:
					NEXT(ply->mode2);
					ply->mode3 = 0;
					_RMKOSound(ply);
					PLY_SETTRAJ(ply, -0x300, 0, 0x700, 0x48,
								-0x280, 0, 0x300, 0x48,
								-0x100, -0x14, 0, 0);
					ply->TumbleStatus = STATUS_TUMBLE_34;
					/* FALLTHRU */
				case 2:
					tumble_until_still(ply);
					break;
				case 4:
					PLAYERTICK;
					break;
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
}	

void react_to_attack(Player *ply) {							/* 0x28ed8 */
	static const char data_29144[]={4,4,4,4,3,3,3,2,1,1,1,-1};
	static const char data_29150[]={6,6,6,5,5,5,4,4,3,3,2,1,1,1,1,1,-1,-1};
	static const char data_29162[]={8,8,8,7,7,7,6,5,5,4,3,2,1,1,1,1,1,1,1,1,-1,-1};
	
	static const char *data_2913e[]={data_29144, data_29150, data_29162};
	
    int a,b;
    switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			if(ply->ReactMode == RM_0x0c) {
				ply->ReactMode2 = 0;
			} else if (ply->ReactMode == RM_0x14) {
				ply->ReactMode2 = 2;
			} else if (ply->ReactMode == RM_MULTIHIT) {
				ply->ReactMode2 = 0;
			}
			ply->x011e = data_29126;     
			ply->DSOffsetX = 0;
			ply->PSPushBacks = data_2913e[ply->ReactMode2 / 2];
			
			ply->Flip = ply->Direction ^ 1;
			
			if(ply->ReactMode == RM_MULTIHIT) {
				CASetAnim2(ply, reactmode_to_status[ply->ReactMode/2 + ply->MultiHitToggle], ply->ReactMode2);
			} else {
				CASetAnim2(ply, reactmode_to_status[ply->ReactMode/2], ply->ReactMode2);
			}
			action_start_35(ply);
			
			if (ply->ReactMode2 == 2) {
				if(ply->ReactMode == RM_VOMIT1) {
					ActStartVomit(ply);         
				} else if (ply->ReactMode == RM_VOMIT2) {
					action_1e336(ply);     /* vomit / blood etc */
				}				
			}
			break;
		case 2:
			if(ply->x011e->x != 17) {
				ply->DSOffsetX += ply->x011e->x;
				ply->x011e++;
			}
			if(ply->Timer2) {
				ply->Timer2--;
				return;
			}
			a=ply->PSPushBacks[0];
			if(a >= 0) {
				ply->PSPushBacks++;   
				if (ply->Direction == FACING_LEFT) {
					a = -a;
					b=1;
				} else {
					b=2;
				}
				if(ply->ProjectilePushBack !=0 || (ply->BoundCheck != b && ply->PlatformFallDir != b)){
					ply->X.part.integer += a;
					if(check_platform_end(ply)) {
						set_falling_from_platform(ply);
					} else {
						choose_blockstun(ply);
						PLAYERTICK;
					}
					return;
				}
				if(ply->DidCollideDash == FALSE) {    
					ply->Opponent->X.part.integer -= a;
					choose_blockstun(ply);
				}
				PLAYERTICK;
			} else {
				/* 0x2903e */
				ply->Invincible = FALSE;
				ply->DSOffsetX  = 0;
				if(ply->ActionScript->Block) {          /* 28826 */
					if(ply->Human) {
						ply->DSOffsetX = 0;
						ply->mode1     = PLSTAT_STANDBLOCK;
						if(ply->ActionScript->Crouch) {
							ply->mode2 = 4;
						} else {
							ply->mode2 = 0;
						}

					} else {
						ply->DSOffsetX = 0;
						ply->x01ac     = TRUE;		// never compared
						comp_recovery_complete(ply);
						sub_2c516(ply);
						return;
					}
				} else {
					if(ply->ActionScript->Crouch) {
						ply_exit_crouch(ply);
					} else {
						ply_exit_stand(ply);
					}
				}
			}
			break;
		FATALDEFAULT;
    }
}
void RMFootSwept(Player *ply) {		/* 29178 */
    int temp, temp2;
    
    switch (ply->mode2) {
		case 0x0:
			NEXT(ply->mode2);
			ply->Flip = ply->Direction ^ 1;
			CASetAnim2(ply,STATUS_FOOTSWEPT,0);
			break;
		case 0x2:
			if(ply->Timer2) {
				ply->Timer2--;
			} else {
				NEXT(ply->mode2);   /* 291b6 */
				ply->GroundSoundDisa = FALSE;
				ply->ReactTimer = 0;
				ply->Airborne   = -1;
				ply->VelX.full  = 0x0200;		//2.00
				ply->AclX.full  = 0x0000;
				ply->VelY.full  = 0x05c0;		//5.75
				ply->AclY.full  = 0x0048; /* standard gravity 0.28125 */
				if(ply->Flip) {ply->VelX.full = -ply->VelX.full;}
				ply->x012a = ply->VelX.full;
				ply->PSPushBacks = data_29150;
			}
			break;
		case 0x4:
			if(ply->ReactTimer) {
				ply->GroundSoundDisa = TRUE;
				if(--ply->ReactTimer) { return; }
			}
			CATrajectory((Object *)ply); 
			if((ply->PlatformFallDir != 0 || ply->BoundCheck != 0) && ply->GroundSoundDisa == FALSE ) {  
				// we've hit a wall
				queuesound(SOUND_GROUND_THUMP);
				ply->ReactTimer = 12;               
				ply->VelX.full  = 0;
				if(ply->VelY.full >  0) { ply->VelY.full = 0; }
			}
			if(PLAYERGROUND) { sub_29280(ply); return; }
			
			
			temp = ply->PSPushBacks[0];
			if(temp < 0) {
				temp = 0; 
			} else {
				++ply->PSPushBacks;
			}
			if(ply->ProjectilePushBack) {
				temp2=1;
				if(ply->x012a >= 0) {
					temp = -temp;
					temp2=2;
				}
				if (temp2 == ply->PlatformFallDir && ply->DidCollideDash == FALSE) {
					ply->Opponent->XPI += temp;
				}
			}
			PLAYERTICK;
			break;
		case 0x6: _RMGetBackUp(ply);        break;
		case 0x8: _RMRecoverTumble(ply);	break;
		case 0xa: PSDizzyState(ply);		break;
			FATALDEFAULT;
    }
}
static void sub_29426(Player *ply, short d6) {		
	const char *cur = ply->PSPushBacks;
	short a,b;
	if(*cur >= 0) {
		a=*cur++;
	} else {
		a=0;
	}
	ply->PSPushBacks=cur;
	if(ply->ProjectilePushBack == FALSE){
		if(ply->x012a >= 0) {
			a = -a;
			b = 2;
		} else {
			b=1;
		}
		if(b == d6 && ply->DidCollideDash == FALSE) {
			ply->Opponent->XPI += a;	
		}
	}
	PLAYERTICK;
}
void RMHitInAir(Player *ply) {			// 2933e 
    switch(ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->mode3 = 0;
			ply->Flip = ply->Direction ^ 1;
			CASetAnim2(ply, STATUS_HIT_AIR, 0);
			break;
		case 2:
			if(ply->Timer2) {--ply->Timer2; return; }
			NEXT(ply->mode2);
			ply->GroundSoundDisa = FALSE;
			ply->ReactTimer   = 0;
			ply->Airborne     = -1;
			ply->VelX.full    = 0x0200;
			ply->AclX.full    = 0x0000;
			ply->VelY.full    = 0x05c0;
			ply->AclY.full    = 0x0048;
			if (ply->Flip) {ply->VelX.full = -ply->VelX.full; }
			ply->x012a = ply->VelX.full;
			ply->PSPushBacks = data_29150;
			ply->YPI += 16;
			break;
		case 4:
			if(ply->ReactTimer) {
				ply->GroundSoundDisa = TRUE;
				if(--ply->ReactTimer) { return; }
			}
			CATrajectory((Object *)ply);
			if((ply->PlatformFallDir || ply->BoundCheck) && ply->GroundSoundDisa == FALSE ) { 
				ply->ReactTimer = 12;
				ply->VelX.full  = 0;
				if (ply->VelY.full >= 0) {
					ply->VelY.full = 0;
				}
			}
			if(PLAYERGROUND) { 
				ply->NoThrow_t       = data_292d4[RAND32];
				ply->TimerInvincible = 120;
				ply->Invincible      = FALSE;
				
				ply_exit_air(ply);				
			} else {
				sub_29426(ply, ply->PlatformFallDir);			/* handle pushback */
			}
			break;
		FATALDEFAULT;
    }
}
void RMElectrocuted(Player *ply) {
    switch(ply->mode2) {    
    case 0:
        NEXT(ply->mode2);
        ply->LocalTimer = 1 * TICKS_PER_SECOND;
        CASetAnim1(ply, STATUS_ELECTROCUTED);
        break;
    case 2:
        if(--ply->LocalTimer) {PLAYERTICK; return; }
        NEXT(ply->mode2);
        
        ply->PSPushBacks = data_29150;
			
		PLY_SETTRAJ(ply, 0x200, 0, 0x600, 0x48,
					0x280, 0, 0x280, 0x48,
					0x100, 0x14, 0x100, 0);
        
        ply->TumbleStatus = STATUS_KNOCKDOWN;
        
        ply->VelY.full = 0;
        ply->VelX.full = 0;
        ply->Timer2    = 0;

        SMTumble(ply);	
        NEXT(ply->mode2);
        CASetAnim1(ply, STATUS_ELECTROCUTED);
        PSPushBack(ply);
        break;
    case 4:
        SMTumble(ply);
        if(ply->VelY.full < 0) {
            NEXT(ply->mode2);
            CASetAnim1(ply, STATUS_KNOCKDOWN);
        }
        PSPushBack(ply);
        break;
    case 6:
        SMTumble(ply);
        if(ply->Tumble) {PSPushBack(ply); return; }
        NEXT(ply->mode2);
        ply->LocalTimer = 12;
        check_dizzy(ply);
        break;
	case 8:		_RMGetBackUp(ply);			break;
    case 0xa:	_RMRecoverTumble(ply);		break;
    case 0xc:   PSDizzyState(ply);			break;
	FATALDEFAULT;
    }
}
void RMFireball(Player *ply) { /* 29460 */
    switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->mode3 = 0;
			ply->PSPushBacks = data_29150;   /* footsweepoffsets */ 
			PLY_SETTRAJ(ply, 0x200,  0x0, 0x500, 0x48, 
						     0x280,  0x0, 0x280, 0x48, 
						     0x100, 0x14,   0x0,  0x0);
			
			ply->TumbleStatus = STATUS_KNOCKDOWN;
			if(ply->ReactMode == RM_YOGAFIREHIT) {
				ply->ProjHit = PROJHIT_FIERY;   
			} else if (ply->ReactMode == RM_FIREBALLHIT) {
				ply->ProjHit = PROJHIT_ICY;  
			}
			/* FALLTHRU */
			M_TUMBLE_POST
			FATALDEFAULT;
    }
}    
void RMTumble32(Player *ply)  {		/* 295fa */
	switch (ply->mode2) {
		case 0:
			ply->mode2 += 2;
			ply->mode3  = 0;
			ply->PSPushBacks = data_29150;
			PLY_SETTRAJ(ply, 0x200,  0x0, 0x600, 0x48, 
							 0x280,  0x0, 0x280, 0x48, 
							 0x100, 0x15,   0x0,  0x0);
			ply->TumbleStatus = STATUS_TUMBLE_32;
			ply->VelX.full = 0;
			/* FALLTHRU */
		M_TUMBLE_POST
		FATALDEFAULT;
	}
}
void RMTumble34(Player *ply) {		// 29676
	switch (ply->mode2) {
		case 0:
			NEXT(ply->mode2);
			ply->mode3 = 0;
			ply->PSPushBacks = data_29150;
			PLY_SETTRAJ(ply, -0x200, 0, 0x600, 0x48, 
						-0x280, 0, 0x280, 0x48,
						-0x100, -0x14, 0, 0);
			ply->TumbleStatus = STATUS_TUMBLE_34;
			ply->VelX.full = 0;
			RMTumble(ply);
			break;
		M_TUMBLE_POST
		FATALDEFAULT;
	}
}


static void ply_throwtumble(Player *ply) {		// 298f4
	_RMTumbleSM3(ply);
	if (ply->Tumble == 0) {
		NEXT(ply->mode2);
		ply->LocalTimer = 12;
		if (ply->Energy >= 0) {		
			check_dizzy(ply);	
		} else {
			/* I died */
			NEXT(ply->mode0);
			ply->mode1 = DIESTAT_2;
			ply->mode2 = ply->mode3 = 0;
			ply->Attacking = FALSE;
			ply->IsJumpThreat = ply->ProjHit = ply->DizzyStun = FALSE;
		}
	}
}

void proc_plstat_thrown_recovery(Player *ply) {		/* 2986a */
	switch (ply->ThrowTrajectory) {
		case 0:					/* 29884 */
			switch (ply->mode2) {
				case 0:
					/* 2989a */
					NEXT(ply->mode2);
					ply->mode3 = 0;
					PLY_SETTRAJ(ply, 0x380, 0x0, 0x380, 0x48, 
								     0x280, 0,   0x280, 0x48,
									 0x100, 0x14,0,     0);
					ply->ReactScreenWobble = FALSE;
					ply->TumbleStatus = STATUS_TUMBLE_30;
					break;
				M_THROW_POST
				FATALDEFAULT;
			}
			break;
		case 2:					/* 29938 */
			switch (ply->mode2) {
				case 0:
					if (ply->Timer2) {
						ply->Timer2--;
					} else {	
						NEXT(ply->mode2);
						ply->mode3 = 0;
						PLY_SETTRAJ(ply, 0x200, 0, 0x600, 0x58,
									0x280, 0, 0x280, 0x48,
									0x100, 0x14, 0x0, 0x0);
						ply->ReactScreenWobble = FALSE;
						ply->TumbleStatus = STATUS_TUMBLE_36;
						ply_throwtumble(ply);
					}
					break;
				M_THROW_POST
				FATALDEFAULT;
			}
			break;
		case 4:
			switch (ply->mode2) {
				case 0:
					if (ply->Timer2) {
						ply->Timer2--;
					} else {	
						NEXT(ply->mode2);
						ply->mode3 = 0;
						PLY_SETTRAJ(ply, 0x400, 0, 0x400, 0x58,
										 0x180, 0, 0x180, 0x48,
										 0x100, 0x14, 0x0, 0x0);
						ply->ReactScreenWobble = FALSE;
						ply->TumbleStatus = STATUS_TUMBLE_30;
						ply_throwtumble(ply);
					}
					break;
				M_THROW_POST
				FATALDEFAULT;
			}
			break;
		case 6:
			switch (ply->mode2) {
				case 0:
					if (ply->Timer2) {
						ply->Timer2--;
					} else {	
						NEXT(ply->mode2);
						ply->mode3 = 0;
						PLY_SETTRAJ(ply, 0x300, 0, 0x600, 0x58,
									0x280, 0, 0x280, 0x48,
									0x100, 0x14, 0x0, 0x0);
						ply->ReactScreenWobble = FALSE;
						ply->TumbleStatus = STATUS_TUMBLE_36;
						ply_throwtumble(ply);
					}
					break;
				M_THROW_POST
				FATALDEFAULT;
			}
			break;
		case 8:
			switch (ply->mode2) {
				case 0:
					if (ply->Timer2) {
						ply->Timer2--;
					} else {	
						NEXT(ply->mode2);
						ply->mode3 = 0;
						PLY_SETTRAJ(ply, 0x380, 0, -0x600, 0x80,
									0x280, 0, 0x400, 0x48,
									0x100, 0x14, 0x0, 0x0);
						ply->ReactScreenWobble = TRUE;
						ply->TumbleStatus = STATUS_TUMBLE_30;
						ply_throwtumble(ply);
					}
					break;
				M_THROW_POST
				FATALDEFAULT;
			}
			break;
		case 0xa:
			switch (ply->mode2) {
				case 0:
					if (ply->Timer2) {
						ply->Timer2--;
					} else {	
						NEXT(ply->mode2);
						ply->mode3 = 0;
						if (ply->FighterID == FID_DHALSIM) {
							PLY_SETTRAJ(ply, -0x200, 0, 0x760, 0x58,
										-0x280, 0, 0x280, 0x48,
										-0x100, -0x14, 0x0, 0x0
							);
						} else {
							PLY_SETTRAJ(ply, -0x200, 0, 0x600, 0x58,
											 -0x280, 0, 0x280, 0x48,
											 -0x100, -0x14, 0x0, 0x0
							);
						}
						ply->ReactScreenWobble = FALSE;
						if (ply->FighterID == FID_DHALSIM) {
							ply->TumbleStatus = 0x52;
						} else {
							ply->TumbleStatus = STATUS_TUMBLE_30;
						}
						ply_throwtumble(ply);
					}
					break;
				M_THROW_POST
				FATALDEFAULT;
			}
			break;
		case 0xc:
			switch (ply->mode2) {
				case 0:
					if (ply->Timer2) {
						ply->Timer2--;
					} else {	
						NEXT(ply->mode2);
						ply->mode3 = 0;
						PLY_SETTRAJ(ply, 0x700, 0, 0x100, 0x58,
									0x580, 0, 0x180, 0x48,
									0x180, 0x14, 0x0, 0x0);
						ply->ReactScreenWobble = TRUE;
						ply->TumbleStatus = STATUS_TUMBLE_30;
						ply_throwtumble(ply);
					}
					break;
				M_THROW_POST
				FATALDEFAULT;
			}
			break;
		FATALDEFAULT;
	}
	
}

/* Tumble State Machine Mode 3 */
static void _RMTumbleSM3(Player *ply) {		/* 2a21a tumble sm */
	switch (ply->mode3) {
		case 0:
			/* 2a234 */
			NEXT(ply->mode3);
			ply->Tumble          = TRUE;
			ply->GroundSoundDisa = FALSE;
			ply->ReactTimer      = FALSE;
			ply->Airborne		 = AIR_REEL2;
			CASetAnim2(ply, ply->TumbleStatus, 0);
			break;
		case 2:
			/* 2a256 */
			if (ply->Timer2) {
				if(--ply->Timer2) {return;}
			} 
			NEXT(ply->mode3);
			PLY_TRAJ_NEXT_1;
			if (ply->Direction == FACING_LEFT) {
				ply->VelX.full = -ply->VelX.full;
			}
			break;
		case 4:
			/* 2a28a */
			if(ply->ReactTimer) {
				ply->GroundSoundDisa = TRUE;
				ply->ReactTimer--;
				return;
			} 
			CATrajectory((Object *)ply);
			if((ply->PlatformFallDir || ply->BoundCheck) && ply->GroundSoundDisa == FALSE) {
				queuesound(SOUND_GROUND_THUMP);
				ply->ReactTimer = 12;
				ply->VelX.full = 0;
				if (ply->VelY.full >= 0) {
					ply->VelY.full = 0;
				}
				if(ply->UndealtDamage){				
					ply->Energy     -= ply->UndealtDamage;
					ply->EnergyDash -= ply->UndealtDamage;
					ply->UndealtDamage = 0;
					QueueEffect(ply->RewardID, ply->Side ^ 1);
				}
			}
			/* 2a2f2 */
			if (ply->VelY.full >= 0 || PLAYERGROUND) {
				PLAYERTICK;
			} else {
				if (ply->UndealtDamage) {
					ply->Energy     -= ply->UndealtDamage;
					ply->EnergyDash -= ply->UndealtDamage;
					ply->UndealtDamage = 0;
					QueueEffect(ply->RewardID, ply->Side ^ 1);
				}
				/* 2a322 */
				NEXT(ply->mode3);
				ActStartGroundDust(ply);
				queuesound(SOUND_GROUND_THUMP);
				if(ply->ReactScreenWobble) {		/* u8 */
					ActStartScreenWobble();
				}
				ply->LocalTimer = 4;
				ply->Airborne = 0;
				CASetAnim2(ply, ply->TumbleStatus, 1);
			}
			break;
		case 6:
			/* 2a358 */
			if(--ply->LocalTimer) {
				PLAYERTICK;
			} else {
				NEXT(ply->mode3);
				PLY_TRAJ_NEXT_2
				if (ply->Direction == 0) {
					ply->VelX.full = - ply->VelX.full;
				}
				ply->Airborne = AIR_JUMPING;
				CASetAnim2(ply, ply->TumbleStatus, 2);
			}
			break;
		case 8:
			/* 2a398 */
			CATrajectory((Object *)ply);
			if(ply->PlatformFallDir || ply->BoundCheck) {
				ply->VelX.full = 0;
			}
			if(PLAYERGROUND) {
				NEXT(ply->mode3);
				PLY_TRAJ_NEXT_3;
				ply->Airborne = FALSE;
				if (ply->Direction != FACING_LEFT) {		// ??
					ply->VelX.full = -ply->VelX.full;
					ply->AclX.full = -ply->AclX.full;
				}
				CASetAnim2(ply, ply->TumbleStatus, 3);
			} else {
				PLAYERTICK;
			}
			break;
		case 10:
			/* 2a3ee */
			if(find_apex(ply)==0 || ply->PlatformFallDir || ply->BoundCheck) {
				ply->mode3 = 0;
				ply->Tumble = FALSE;
			}
			PLAYERTICK;
			break;
		FATALDEFAULT;
	}
}


static void _RMGetBackUp(Player *ply) {		//292ba
	if(--ply->LocalTimer == 0) {
		NEXT(ply->mode2);
		ply->TimerInvincible = 1 * TICKS_PER_SECOND;
		CASetAnim1(ply,STATUS_BACK_UP);
	}
}
