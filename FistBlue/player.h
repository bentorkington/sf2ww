/* player.h */

#ifndef INC_PLAYER
#define INC_PLAYER

#include "sf2types.h"
#include "particle.h"

#ifndef PLAYER_USER_DATA_SIZE
#define PLAYER_USER_DATA_SIZE 0x80
#endif


/* randoms */
#define PLY_PUNCHING    0x0
#define PLY_KICKING		0x2

/* StandSquat */
#define PLY_STAND		0x0
#define PLY_CROUCH		0x2
#define PLY_JUMP		0x4
#define PLY_THROW		0x6

//Player.Airborne
#define AIR_ONGROUND	0
#define AIR_JUMPING		1
#define AIR_REEL1	   -1
#define AIR_REEL2	   -2

//JoyCorrect
#define JOYCO_UP		8
#define JOYCO_DOWN		4
#define JOYCO_AWAY		1
#define JOYCO_TOWARD	2


/* FighterID */
enum fighter_id_t {
	FID_RYU,		FID_E_HONDA,	FID_BLANKA,		FID_GUILE,
	FID_KEN,		FID_CHUN_LI,	FID_ZANGEIF,	FID_DHALSIM,
	FID_M_BISON,	FID_SAGAT,		FID_BALROG,		FID_VEGA,
};
typedef enum fighter_id_t FBFighterID;

/* PLMODE / Object mode1 goes in Player->mode0 */

#define PLMODE_KNOCKED_OUT      0x4


/* plstat / Object                Player->mode1 */
#define PLSTAT_NORMAL            0x0
#define PLSTAT_CROUCH            0x2
#define PLSTAT_JUMPING           0x4
#define PLSTAT_TURNAROUND		 0x6
#define PLSTAT_STANDBLOCK        0x8
#define PLSTAT_ATTACKING         0xa
#define PLSTAT_IN_POWERMOVE      0xc
#define PLSTAT_REEL				 0xe
#define PLSTAT_VICTORY          0x10
#define PLSTAT_LOSS             0x12
#define PLSTAT_TUMBLE			0x14	// was THROWN_RECOVERY

/* DIESTAT, also used in player->mode1 */
#define	DIESTAT_0				0
#define DIESTAT_2				2		//already lying on ground
#define DIESTAT_4				4
#define DIESTAT_ELECTROCUTED	6
#define DIESTAT_8				8
#define DIESTAT_10				10
#define DIESTAT_12				12

#define THROWTRAJ_0			0
#define THROWTRAJ_2			2
#define THROWTRAJ_4			4


#define ATTACK_STAND		0
#define ATTACK_SWEEP		1
#define ATTACK_JUMP			2
#define ATTACK_SPECIAL		3
#define ATTACK_PRIORITY		4

#define PROJHIT_FIERY	1
#define PROJHIT_ICY		2

/* Ply_ReactMode                    knockdown,       Resulting STATUS_ */
#define RM_VOMIT1		   0x0		// STATUS_STUN1					r2a
#define RM_VOMIT2		   0x2		// STATUS_STUN2					r2a
#define RM_0x04			   0x4		// STATUS_STUN2					r2a
#define RM_0x06			   0x6		// STATUS_STAND_BLOCK_FREEZE	r2a
#define RM_0x08			   0x8		// STATUS_CROUCH_BLOCK_FREEZE	r2a
#define RM_0x0a			   0xa		// STATUS_CROUCH_STUN			r2a
#define RM_0x0c			   0xc		// STATUS_STUN2					r2a
#define RM_FOOTSWEPT       0xe		//*STATUS_STUN2			RMFootSwept()
#define RM_HITINAIR       0x10		// STATUS_STUN2			RMHitInAir()
#define RM_FIREBALLDOWN   0x12		// STATUS_STUN2			RMFireball()
#define RM_0x14			  0x14		// STATUS_STUN1					r2a
#define RM_MULTIHIT       0x16		// STATUS_STUN2					r2a  /* birdkick, chunli multi, enhond multi? */
#define RM_YOGAFIREHIT    0x18		//*STATUS_STUN1			RMFireball()
#define RM_ELECTROCUTED   0x1a		//*STATUS_STUN1			RMElectrocuted()
#define RM_TUMBLE1		  0x1c		//*						RMTumble32()
#define RM_TUMBLE2		  0x1e		//*						RMTumble34
#define RM_HURRICANE      0x20		// STATUS_STUN1					r2a
#define RM_FIREBALLHIT    0x22		//*STATUS_STUN1			RMFireball()
#define RM_0x24			  0x24		// STATUS_STUN1					r2a
#define RM_0x26		      0x26		// STATUS_STUN2					r2a

#define ROUNDREACT_VICTORY		0
#define ROUNDREACT_LOSS1		2
#define ROUNDREACT_LOSS2		4

#define STATUS_STAND             0x0		/* substat 4 */
#define STATUS_WALKING			 0x0		/* forward 0 backward 2 */
#define STATUS_NORMAL            0x2   /* STATUS_CROUCH is bad */
#define STATUS_CROUCH            0x4
#define STATUS_STAND_UP          0x6
#define STATUS_JUMP_START        0x8
#define STATUS_LANDING           0xa
#define STATUS_TURN_AROUND       0xc
#define STATUS_CROUCH_TURN		 0xe
#define STATUS_FALLING          0x10 
#define STATUS_STAND_BLOCK      0x12
#define STATUS_STAND_BLOCK2		0x14
#define STATUS_CROUCH_BLOCK     0x16
#define STATUS_CROUCH_BLOCK2	0x18
#define STATUS_STUN1			0x1a
#define STATUS_STUN2			0x1c
#define STATUS_STAND_BLOCK_FREEZE		0x1e
#define STATUS_CROUCH_BLOCK_FREEZE		0x20
#define STATUS_CROUCH_STUN		0x22
#define STATUS_FOOTSWEPT        0x24
#define STATUS_KNOCKDOWN		0x26
#define STATUS_BACK_UP			0x28
#define STATUS_DIZZY			0x2a
#define STATUS_HIT_AIR			0x2c
#define STATUS_ELECTROCUTED     0x2e
#define STATUS_TUMBLE_30		0x30
#define STATUS_TUMBLE_32		0x32
#define STATUS_TUMBLE_34		0x34
#define STATUS_TUMBLE_36		0x36
#define STATUS_PISSED_OFF		0x3c
#define STATUS_GETTING_THROWN   0x3e
/* not e.honda or blanka */
#define	STATUS_PUNCH			0x40
#define STATUS_KICK				0x42
#define STATUS_CROUCH_PUNCH		0x44
#define	STATUS_CROUCH_KICK		0x46
#define STATUS_JUMP_PUNCH		0x48
#define STATUS_JUMP_KICK		0x4a	

// get these out
#define STATUS_BOUNCE_WALL      0x4c  /* chun li */

#define STATUS_HADOUKEN			0x4c
#define STATUS_RYUKEN_THROW		0x54

struct player_t {
#include "std_object.h"    
    FIXED8_8	AclX;
    FIXED8_8	AclY;
    u16			x0044;
    signed char UserByte;
    char		Timer2;				/* 0047 */
    char		BoundCheck;			/* also a void * */
    char		BlockStun;
    char		SufferHB5;
	char		NextReactMode;			/* 4b */
	char		NextReactMode2;		/* 4c  vomit / blood etc select for act20 */
	char		OnPlatform, OnPlatform2;	// 4d , 4e
	u8			HitBoxUnused[3];    /* for hitboxes 1,2,3 */
	short		DSOffsetX;
	char		Direction;			/* 0x54 FACING_LEFT=0, FACING_RIGHT=1 */
	u8			ReactMode, ReactMode2;		// 55, 56
	u8			StepSave;					// 57
	short		Difficulty;					// 58
	char		OnGround;					// 5a
	char		PlatformFallDir;      /* direction falling off platform */
	short		DizzyClearCnt;
	short		DizzyCount;
	short		DizzySpell;
	short		IsBlockingSuper;		//0062 should be char?
	char		ThrowStat;
	short		Throw[4];
	short		ThrowCatchX;
	short		ThrowCatchY;
	char		x0070;
	char		ProjectilePushBack;				// being pushed back by projectile
	
	u8			ThrowTrajectory;    /* 0x0076 */
	//char		x0077;				// unused?
	short		UndealtDamage;
	u16			RewardID;
	short		Damage1;            /* 0x7c */
	short		Damage2;
    u8			UserData[PLAYER_USER_DATA_SIZE];				// 0x080
    FIXED8_8    NextVelX, NextVelY, NextAclX, NextAclY;			// 0x100
    FIXED8_8    Next2VelX, Next2VelY, Next2AclX, Next2AclY;		// 0x108
    FIXED8_8    Next3VelX, Next3VelY, Next3AclX, Next3AclY;     // 0x110
	
    char		GroundSoundDisa;
    char		Tumble;
    const char	*PSPushBacks; /* 11a footsweepoffsets */
    struct smalladjust     *x011e;			
    char		x0122;			/* unused */
    char		DizzyFall;
    char		DizzyStun;
	char		DizzyStruggle;	/* 0x125 */
    u8			TmrWallBounce;
    char		NextAction; /* 0x127 */
    u16			TumbleStatus;
    short		x012a;
    u8			MultiHitToggle;
	u8			TmrWallBounce2;
	u8			ReactScreenWobble;					// 0x012e

	const unsigned char	*AIStratAgg0;				// 0x0130
    const unsigned char	*AIStratAgg1;	
	u16					AIStratIndexAgg0;			/* 0x0138 AI index of x0130 */
	u16					AIStratIndexAgg1;			/* 0x013a AI index of x0134 */
	/**/
	short		x0140;				/* 0140 */
	char		x0142;			/* associated with action04 */
    u8			NoThrow_t;		/* 0x0143 */
    char		ThrownFromDizzy;
    u8			ReactTimer;				// 0x145
    u8			CompDoThrow;					// 0146
    char		AISigAttack;				// was CompDoStandAttack
	char		AISigSpecial;				// was CompDoPowerMove
	char		AIMultiCount;					// set in AI Special Params.
	char		AIVolley;					// 014a
    char		MultiHoldoff;			/* 0x14b */
    char		VegaHasClaw;			/* 0x14c */
	char		CompDoAirThrow;
	u8			x014e;					// some timer, never found set
	
	char		x0150;
    u8			TimerInvincible;		/* 0x151 */
    u32			BonusScore;
    int			BonusScoreDash;			/* 0x156 */
    u8			DidCollide;					// set after collision, copied to 15b in PSPlayerDamage
    u8			DidCollideDash;					
    u32			x015c;						/* 15c packed bcd */
    /* 0x160 */
	u8			PushDirection;				/* being pushed */
    char		InGroundAttack;			/* 0x0161 */
    char		ClawCnt;
	char		x0163;
    /**/
    signed char Move;				/* 0x180 */
    signed char Jumping;
    short		OppXDist;
    short		OppYDist;			/* 0x0184 */
    signed char PunchKick;			// 0186
    signed char ButtonStrength;		// 0187
    signed char StandSquat;			// 0188
    
    char		Attacking;      /* 0x018b */
	
    char		ProjHit;		/* 0x0190 */
    char		ThrowDisable;    /* 0x0191 */
    
    char		PSFinishedParticipating;			// 01a0
    char		TCollDis;
    u8			ExtraSpriteEna;
    u8			PSRoundReactMode;
    short		Draw_OffsetX, Draw_OffsetY;
    struct		image *Image2;	/* 01a8 */
    char		x01ac;
	char		x01ad;				/* for action04 */
	char		x01ae;				/* for action04 */
	char		IsWithinBounds;		/* 1af TRUE if X is within 0,0x280 */
    u16			x01b0;				/* incremented on throw, not found anywhere else */
    char		IsJumpThreat;		/* x01b2 another 'in air' flag */
	u8			CompImmune;			/* 0x01b3 */
    struct      extrasprite *Sprite2;	/* 0x01b4 */
	FIXED16_16  OldOldY;				/*  0x1b8 old old Y */
    short		EnergyCursor;
    u8			EnemyDirection;		/* 0x01be */
    char		Invincible;
    short		Size;				/* x01c2 The average width of my sprite */
    FIXED16_16	DeltaX;				/* x01c4 */
	FIXED16_16  DeltaY;
	FIXED16_16  OldX;				/* x01cc  */
    FIXED16_16  OldY;				
	Object		*Projectile;		/* 0x01d4 LimitNextShot cheat */
	short		RoughTimeRemain;	/* 0x01d6  rough time remaining */
	u8			AITimers[8];		// 01d8
    
	/* don't know 0x1e0-1ff */
	
	u8	AIAgressive;		/* 0x200 AI */
	u8	AIMode1;
	u8	AIMode2;
	u8	x0203;			/* used in AI Test Screen */
	unsigned char		AIStrategy, AIParam1;
	signed char			AIParam2, AIParam3, AIParam4;
	
	/* future AIStrategy-AIParam4 */
	unsigned char	x0209, x020a;
	signed char x020b, x020c, x020d;
	
	u8		AITypeAgg0;			/* 0x020e */
	u8		AITypeAgg1;			/* RND NO= */
	char	AISaveState;		/* 0210 def. signed */
	u8		AIWalkDirection;
	u8		CompDoBlock;			/* x0212 */
	u8		CompDoJump;		    	/* x0213 */
	
	u8		CompDoBlockStun;		/* x0215 */
	u8		x0216;
	u8		AIWallBounce;					/* 217 */
	u8		x0218,x0219;		/* backup of AIAggressive and AIMode1 */
	
	u8		AITypeDef;
	u8		AIStartAgain;					// 21d
	char	AIBlockStunTimer;
	u8		AITimer;			// 220 timer 
	char	DiceRollCount;				// 221 Diceroll Attack thingee
	short	AIWalkTarget;		// 0x0222 X coord of walk target
	short	AIJumpSel;		//  226;
	u16		x0228;
	char	x022a;
	u8		AICanJumpAttack;	//22b	
	const unsigned char	*AIStratDef;		// 22c Defensive Strategeis
	u16					AIStratIndexDef;		// 230
	short			AITooClose;				// 0232 AITooClose
	short			AIOppJumpHeight;
	u8	x0236;		/* vega backward flip defence ? */
	u16 x0238;
	u16 x023a;
	u16 x023c;			// some jump height
	u8	x023e;
	
	
	u8	AIAllowAggressive;
	u8	AIAggTimer0;					// 0271
	u8	AIAggTimer1;
	u8	AIThreatCheckMode;			/* 0273 initially 2 */
	u8	YokeAttackingMe;			// 0275
	u8	x0276;						// apparently unused?
	u8	AITimerThreatCheck;
	u8	AIThreatFound;					// 0278 
	u8	AIForceDefensive;		/* x0279 */
	char	YokeSaved;
    char    Airborne;			// see AIR_*
    u8	x027b;
	// END AI
	
    signed char  Alive; /* 0x280 on not cleared in resetstate */
    signed char  Side;
    u8  BlinkerMode0;			// 282
    u8  BlinkerMode1;
	u8	BlinkerMode2;
    /**/
    short   ContinueSecs;	/* 0x286 */
    short   ContinueTick;
	/**/
    char	Human;			/* 0x28e */
    u8		SelectComplete;
    u8		RoundsWon; 
    u8  	FighterID;      /* 0x0291 */
    DUAL    JoyDecode;
    DUAL    JoyDecodeDash;
    u32     Score;       /* in BCD */
    struct	player_t	*Opponent;
    u8      OpponentID;			/* 0x02a2 */
    u8      FighterSelect;
	u8		ContinueCredits;
    char    Continuing;	
	u8  	JoyCorrect;
	u8  	JoyCorrect2;
	u8		JoyCorrectDash;				    // joycorrectdash
	u8		JoyCorrectDash2;				// joycorrectdash2
	u8		BlinkerTimer;
	
	char	SelectDelayTmr;
	/* "copy of 28e" */
	char	x02ae;			
	
    u8		x02de;
	
	short	x02e0;	/* 0x02e0  Highscore timer */
	short	x02e2;	/*         Highscore autorepeat */
	char	x02e4;
	char	x02e5;
	u8		x02e6;		/* Timer */
	char	x02e7;
	char	x02e8[4];	/* Hi score string */
	short	x02ec;
};
typedef struct player_t Player;


/* player.c */
void hitsound(int sound);
void LBStartTimeWarp(void);
void check_ply_x_bounds(Player *ply);
int find_apex(Player *ply);

void LBGetDamage(Player *ply, Player *opp, int d2);
void set_throw_trajectory(Player *ply, int trajectory, int direction, short damage);	//3668 

void ply_grip_release(Player *ply, int direction);
void sub_36d6(Player *ply, short x, short y, short throwtrajd0, char direction, int damage, int sound, short subsel_d3);
void LBThrowClear(Player *ply, Player *opp);
void LBInitHitBoxes(Player *ply);
void ply_thrown(Player *ply);
void random_damage_adjust_1(Player *ply, int damage_d0, int damage_d1);
void random_damage_adjust_2(Player *ply, int damage);
void proc_player_actions(void);
void sub_99142(Player *ply);
int _EnergyDamageAdjust(Player *ply, int damage);
void setstatus4(Player *ply, short d1);
void CASetAnimWithStep(Player *ply, int status);
void CASetAnim2(Player *ply, int statusd1, int argd0);
void CASetAnim1(Player *ply, short status);

void CASetAnim_BurningFall(Player *ply);
void CASetAnim_IcyFall(Player *ply);

short ply_cb_standmove(Player *ply);	/* 2a81a */
short ply_cb_crouchmove(Player *ply);	/* 2a84a */
short ply_cb_jumpmove(Player *ply);		/* 2a87a */

void check_powermove_input(Player *ply);

#define AF2 (ply->AnimFlags & 0xff)
#define AF1 (ply->AnimFlags & 0x8000)

#define PLAYERTICK actiontick((Object *)ply);
#define PLAYERGROUND check_ground_collision((Object *)ply)

#define CA_ENDFLAG AnimFlags & 0x8000
#define CA_ANIMFLAG AnimFlags & 0xff

#define PLY_SETTRAJ(ply,arga,argb,argc,argd,arge,argf,argg,argh,argi,argj,argk,argl) \
ply->NextVelX.full    = arga;\
ply->NextAclX.full  = argb;\
ply->NextVelY.full    = argc;\
ply->NextAclY.full  = argd;\
ply->Next2VelX.full   = arge;\
ply->Next2AclX.full = argf;\
ply->Next2VelY.full   = argg;\
ply->Next2AclY.full = argh;\
ply->Next3VelX.full   = argi;\
ply->Next3AclX.full = argj;\
ply->Next3VelY.full   = argk;\
ply->Next3AclY.full = argl;

#define PLY_TRAJ_NEXT_1						\
ply->VelX.full   = ply->NextVelX.full;		\
ply->AclX.full = ply->NextAclX.full;	\
ply->VelY.full   = ply->NextVelY.full;		\
ply->AclY.full = ply->NextAclY.full;

#define PLY_TRAJ_NEXT_2						\
ply->VelX.full   = ply->Next2VelX.full;		\
ply->AclX.full = ply->Next2AclX.full;	\
ply->VelY.full   = ply->Next2VelY.full;		\
ply->AclY.full = ply->Next2AclY.full;

#define PLY_TRAJ_NEXT_3						\
ply->VelX.full   = ply->Next3VelX.full;		\
ply->AclX.full = ply->Next3AclX.full;	\
ply->VelY.full   = ply->Next3VelY.full;		\
ply->AclY.full = ply->Next3AclY.full;

#define PLAYER_PUSH(x) g.Player1.PushDirection = 1-x; g.Player2.PushDirection = x;


#define PLY_THROW_SET(arg0, arg1, arg2, arg3)   \
ply->Throw[0] = arg0;							\
ply->Throw[1] = arg1;							\
ply->Throw[2] = arg2;							\
ply->Throw[3] = arg3;							\

#define PLY_TRAJ0(arg0, arg1, arg2, arg3)		\
ply->VelX.full = arg0;							\
ply->VelY.full = arg1;							\
ply->AclX.full = arg2;						\
ply->AclY.full = arg3;						\

#define PLY_NEWBUTTONS (~ply->JoyDecodeDash.full & ply->JoyDecode.full)


#define QUICKMOVE(MOVE_ID)      \
ply->Move = MOVE_ID;			\
quirkysound(MOVE_ID);			\

#define STDANIM(ARG_A,EXITROUTINE)	\
switch (ply->mode2) {				\
case 0:							\
NEXT(ply->mode2);			\
CASetAnim2(ply, ARG_A, ply->Move);	\
break;						\
case 2:							\
if (AF1) {					\
EXITROUTINE(ply);		\
} else {					\
PLAYERTICK;				\
}							\
break;						\
default:						\
break;						\
}

#define STDPUNCHKICK(A,B)			\
switch (ply->PunchKick) {			\
case PLY_PUNCHING:				\
A(ply);						\
break;						\
case PLY_KICKING:				\
B(ply);						\
break;						\
}



#endif /* INC_PLAYER */
