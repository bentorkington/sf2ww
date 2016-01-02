/* structs.h */

#ifndef INC_STRUCTS
#define INC_STRUCTS

#include "particle.h"
#include "player.h"
#include "coinage.h"
#include "actions_198a.h"
#include "sf2const.h"

struct damageresult {
    int damage; /* %d4 */
    int d5;
};
typedef struct damageresult DR;

/*!
 @struct HiScore
 @abstract stores a high score and corresponding initials
 @field score The score in BCD notation
 @field name Three printable ASCII characters, chosen by the player achieving the score
 */
struct HiScore {
	u32 score;
	char name[4];
};

typedef struct {
    u16 ObjBase;			//2a
    u16 Scroll1Base, Scroll2Base, Scroll3Base, RowScrollBase;
    u16 PaletteBase;		//34
    u16 Scroll1X, Scroll1Y;	//36
    u16 Scroll2X, Scroll2Y;	//3a
    u16 Scroll3X, Scroll3Y;	//3e
    u16 Star1X, Star1Y;		//42
    u16 Star2X, Star2Y;		//46
    u16 RowScrollOffset;	//4a
	u16 VidControl;			//4c
    /* missing */
    u16 DispEna;            // 0x52
    /*  so on ... */
	u16	Prio[4];            // 0x54
	/* Priority Masks */
	u16 Scroll1XDash, Scroll1YDash;
	u16 Scroll2XDash, Scroll2YDash;
	u16 Scroll3XDash, Scroll3YDash;
	u16 Star1XDash, Star2XDash;
} CPSParam;


struct state_playerselect{
	char	mode0;				// 5dbe
	char	mode1;				// 5dbf
	
	u8		TimerCoarse;		//5dc2q
	u8		TimerFine;
	char	TimeExpired;
	char	PLSLNewPlayers;
	short	ExitDelay;				// 5dc6
	char	x5dc8;
	char	ChallengerDelay;
	char	IntroDelay;
} ;


/*
 *
 * The main game structure 
 *
 */
/* compile-time params */
#define EFFECT_QUEUE_LENGTH 128
#define SOUND_QUEUE_LENGTH  128

typedef struct gamestate {
    u16 mode0, timer0, mode1, timer1, mode2, timer2, mode3, timer3;
    u16 mode4, timer4, mode5, timer5, mode6, timer6;
} GameState;	

typedef struct game {
    u16 mode0, timer0, mode1, timer1, mode2, timer2, mode3, timer3;
    u16 mode4, timer4, mode5, timer5, mode6, timer6;
    u8  tick, libsplatter;
    u16 effectNext, effectCurrent;     /* 0x1e CQ cursors */
    u16 soundNext,  soundCurrent;    

    CPSParam CPS;    /*  0x2a all the CPS registers */
    
    /* input variables */
    u8  RawButtons0;			//76
    u8  RawButtons0Dash;		//77
	u8	RawButtons0Dash2;
	u8	RawButtons0Dash3;
	//gap
    DUAL ContrP1, ContrP1DB, ContrP2, ContrP2DB;		//7e,80,82,84
	// gap
    u8  JPCost, JPDifficulty, JPParam;					//8a,8b,8c
	u8	x008d;											//8d interrupt flipdisplay
	// gap
	u8 *x0092,*x0096;		/* pointers to gfx ram (!!) for something funky at 22b0 */
    /* end input vars */
	const u16		*DemoJoyP1, *DemoJoyP2;				//0092,0096
	short	DemoJoyP1Timer, DemoJoyP2Timer;		//009a,009c
	
    int safetyVariable;         /* deleteme */

    u16 effectQueue[EFFECT_QUEUE_LENGTH];       /*  a2 */
#ifdef SOUND
    u16 soundQueue[SOUND_QUEUE_LENGTH];         /* 1a2 */
#endif
    
    Coinslot    coinslot1, coinslot2;
	u8	ContinueCredits;
	u8	x02a5;		

    u8		StartServiceButtons;
    u8		SoundOutstanding;
    u16		ObjTileBudget;      /* x02b6 tile draw budget */
    u16		x02b8;				/* x02b8 screen dirty? seems not used */
	u16		x02ba;				/* also unused? */
	u8		DisableDblBuf;		/* x02bc */
	short	*x02be;				/* RowScroll Cursor used in spritecoords for scroll2 follow */
    u16		Version;			/* 0x02c2 JapEtcUsa */
    u8		randSeed1, randSeed2;			/* 0x2c4,5 */
    u16		Difficulty;			/* 02c6 */
    /* 0x02c8 - 0x02cd unused */
    u8		AllowContinue;  /* 02ce */
    u8		SpecialCostJumper, DemoSound, FlipDisplay, FreePlay, TwoCreditsToStart;
    u8		FreezeMachine;  /* 02d6 */
    u8		Debug, InTestMode, ActiveHumans; 
	u8		CoinStatus, DemoStarted, InDemo, NumberCredits, NumberCreditsDash;	//2db...
 
    char	x02e0, WaitMode, NoInterrupt, DebugNoCollide;
    u8		PlayersOnline;
    
    u8		NewChallengerWait;  // 2e5
    char	TimeWarpTimer;
    char	PreRoundAnim;
	char	x02e8;
    char	PlyDrawOrder;		/* 0x02e9 */
    char	FirstFight;
    char	x02eb, x02ec, OnFinalStage, ContinueCount;
	
	char	x02f0;
	short	CoinsTaken;		// 02f4
    char	Uptime[4];		// 2f6 - a   
	
	u16		DemoStageIndex;		// 02fe
	short	DemoFightTimer;
	char	x0302;
    char	TimeWarpSlowdown, TimeWarpSlowdown2;	/* 0x303-4 */
    char	x0305;			/* Bison cape signal */
    char	x0306;			/* Bison cape done */
    char    NotUsed;    /* some debug signal */
	short	UsageCount[8];		// 308-318

	char	x0318;
    u8		SeqPause;				// 31a
    u8		FastEndingFight;		// 31b
	u8		Diff_GameCnt;		/* difficulty */
    u8		DifDebugJumper;
	u8		x031e;
    u8		SkipEnding;
	u8		x0320;		/* affects sonic holdback timer, setter not found */
    char    TwoHumans;
    char	Debug2;		// 0322
    u8		JapanJumper;
    
	/* The Player Structures. 
	   These are at 0x03c6 and 0x06c6 on sf2ua */

    struct	player_t Player1, Player2;   

	/* begin 9ca-b4a block */
	
    short	CurrentStage, LevelCursor, CurrentDifficulty;		/* 0x9ca */
	
	/* check, may be 8_8fix */
    u16		StageMinX, StageMaxX, StageMinY, StageMaxY;
    u16		GroundPlaneY;		// 9d8
    u8		NewPlayers;			// 9da
    u8		PlayerSelectDone;   /* PlaneFlightDone  0x09db */
    u8		PlayersSelected;
    u8		PlayersSelectedDash;
    u8		BattleOver;
    char	OnBonusStage;
    char    Pause_9e1;
    u16		ActionLibSel;
    u16		Palette1;       /* 0x09e4 */
    u8		BattleWinner;
    u8		BattleLoser;
    u8		BisonBeater;
    u8		Defeated[12];   /* 0x09e9 */
    u8		NoLoser;        /* set to TRUE to avoid chant, for bonus stages */
	u8		x09f5;			// found set, not used
	u8		x09f6;			// found set, not used
    u8		BattleWinSide;  /* 0x09f7 */
    u8		PlyLostToPly;
    u8		x09f9;			
    u8		BonusDone;
    u8		UpToBosses;						// 9fb
	u8		ContinueBits;				/* 0x09fc */
	
	u8		x09fe;
	u8		OnLevel8;
    u16		LastFightStage;
	char	x0a02;
	char	x0a03;
    short	Diff_0a04;				/* Nett difficulty index */
    short	Diff_0a06;
    u16		Diff_0a08;
	u16		x0a0a;
	short	x0a0c;				// incremented each bumpdifficulty_01
	char	x0a0e;              // number of continues
	u8		x0a0f;
	
	u16		x0a10;				// not found set
	Player	*ClockingPly;		/* 0a12 was U16 ptr */
	u8		BisonBeatSide;			/* x0a14 */
	u8		VictoryCnt;					/* 0a15 incremented each victory */
	u16		x0a16;			// difficulty, array index
    u8		x0a18;			// Max Difficulty if true, not found set yet
	u8		ComputerWon;	/* 0x0a19 */
    
	
	
    /* a4a - a6a cleared init_fight_vars */
	char	x0a4a;
    char    GSInitComplete;      /*  0a4b GStates Init Complete */
    u16		RoundCnt;   /* 0x0a4c */

    u8		WinningFighter, LosingFighter;
	
	u8		HumanWinner;		// 0a52
	u8		HumanLoser, RoundWinnerSide;
    u8		RoundLoserSide, PlyLostToComp;
    u8		x0a50;

	char	Diff06Cnt;		// 0a55
		
	short	CarOffX;		// 0a58
	short	CarOffY;
	char	x0a5c;			// not found set
	u8		CarWasted;		/* 0x0a5d */
	/* end init_fight_vars */
    u8		x0a6a;			/* placeholder */
							 
	/* struct roundvars RoundVars;	*/ 
							 
							 /* 0xace to 0xb4e */

	/* Begin 0x80 bytes from ace to b4e reset at 2c8a */
	u8		TimeRemainBCD;			/* 0xace */
    u8		TimeRemainTicks;
    
	u8		DisableTimer;		    /* 0ad0 unused? diables game timer ticks */
    char    RoundComplete;			/* 0ad1 */
    char    RoundResult;
    
    char    TimeResult;				/* 0ad4 */
    char    TimeOut;
    short	Diff_0ad6;              /* not found set */
	u8		Diff_WeakBoxCnt;		/* incremented when hitbox4 hit */
	
	char	ScreenWobbleStarted;	// 0adb
    char    ScreenWobbleMagnitude;			/* 0adc */
    u8      PlayersThrowing;
    u8		x0ade;					// not found set
	char	LastDrawOrder;			/* last draw order */

	u8		ThrowEndHoldoff;
	u8		KillAct48;				/* 0ae0 signal for Act48 hmm maybe wrong */

    char    FightOver;				/* 0ae1 */
	u16		HumanMoveCnt;			
	u16		x0ae4;
	char	BonusComplete;			/* ae6 */
	char    x0ae7;				// not read yet

//	u8		x0ae8;					/* mode for low time remainining blinker */
//	u8		x0ae9;					/* Timer for flash */
    u8		FlagTimeWarp;			/* 0aeb time slows down */
	
	/* End 0x80 bytes cleared at 2cba  and 0x180 bytes at 2af2 */
	char	x0b4a;		/* placeholder */
	
	// GStates1,2,3 where here, b52 bd2 c52 cd2 (rowscroll)  (0xb52-0xd51)

	/* Arrays of free object in each layer 
	   These were SPs in CPS version, but converted to arrays here */
	
#ifndef CPS	
    Object *FreeStack_118a[ 2 ];	// 0xd52 shadows unused
    Object *FreeStack_Layer1[  COUNT_LAYER1 ];	// Layer1
    Object_G2 *FreeStack_Layer2[ COUNT_LAYER2 ];	// Layer2
    Object *FreeStack_Layer3[ COUNT_LAYER3 ];	// Layer3
            /* up to 0x0dfe on SF2ua */
#endif

    u8	FreeShadows;		      /* 0x1156 free shadows @ 118a */
    u8  FreeLayer1;   /* projectiles for AllocProjectile */
        
    u8  Layer1Grp1Cnt;									/* 0x115a */
    u8  Layer1Grp2Cnt;
    u8  FreeLayer2;						/* for pop_1174 , freecount_198a */
	u8  Layer2Grp1Cnt, Layer2Grp2Cnt, Layer2Grp3Cnt;	/* 0x115d-f */
    u8  FreeLayer3;										/* 0x1160 AllocActor */    
    u8  Layer3Grp1Cnt, Layer3Grp2Cnt, Layer3Grp3Cnt;
    u8  Layer3Grp4Cnt, Layer3Grp5Cnt, Layer3Grp6Cnt;      /* 0x1166 */
    
    /* pointers that we don't use any more omitted */
    Object Ply1Shadow;      /* 0x118a if you change the type change the param */
    Object Ply2Shadow;      /* 0x128a to memclear() in LBResetState()           */    
    Object Objects1[COUNT_LAYER1];		/* sizeof 0xc0 138a - 258a */
    Object_G2 Objects2[COUNT_LAYER2];    /* sizeof 0xc0 198a - 258a */
    Object Objects3[COUNT_LAYER3];    /* sizeof 0xc0 258a - 528a */
    
	struct HiScore HiScoreTable[6];		/* 528a - 52ba */
	
	struct HiScore *x52da[6];
	u32	TopScore;				/* x5302 */
	
	Object2 x530a[20];     /* sizeof 0x80 */

	u8	FreeCount_530a;		/* 5d0a  stack is 530a */
	//Object2 **x5d0c;
    Object2 *FreeStack_530a[20];	/* 0x5d0e */
	
	/* 5d36 is top of free stack for 5d0c */
	
	/* 5d3a - 5d59 gone to struct effectstate */
	
	/*****************/
	/* Player Select */
	/*****************/
	
	short	LevelScript[16];	/* 5d7e */
	struct state_playerselect PLSL;		// 5dbe, sizeof 0x20;
	char	x5dde;		/* placeholder only */

	
	// Tasks stuff moved to task.c
	Object  x5dfe;			// used for the Hiragana draw at 7e884

	
	//////////////////
    // module globals/
	//////////////////
	
	
	u8		x8a30;
	short	x8a36;	/* difficulty */
	short	x8a38;	
	short	x8a3a;
	short	x8a3e;
	short	x8a3c;
	short	x8a40;	/* difficulty */
	u32		bin2bcd_result;  /* bin2bcd result */
	
	char	x8a4b;
	
	char	PLSL_FIDSave;		// 8a56
	
	u8		x8a5c;
	
	u8		x8a5e;				// flag for highscore enqueue_and_layer
	u8		x8a60[2];
	u8		x8a62[2];
	u8		x8a64[2];
	char	x8a66[2];			// see action_0d
	char	PlaneLandedInCity[12];	// 8a68 
	char  	x8a74;				// skyscraper anim flags
	unsigned char x8a75;		// skyscraper anim timer
    


 //     End-of-round Score counter
	u16		TimeBonusSprite[10];		// time remaining score bonus sf2ua: 0x8a76
	u16		VitalBonusSprite[10];      // vital score bonus sf2ua: 0x8a8a
	u16		TotalBonusSprite[10];      // total of both sf2ua: 0x8a9e
 
    
    
    
	// u32 x8aa8;
	u32		x8aac;

    // act3b score counter
    u16		TotalBonusCount;            // sf2ua: x8ab2
	u8		ScoreCountFlags;            // sf2ua: x8ab4
	u8		CanSpeedUpScoreCount;		// sf2ua: x8ab5, set when total bonus being counted
	
    // bonus stage flags
	Object		*x8ab6;			/* Last barrel to appear */
	signed char x8ab8;
	u8			x8ab9;			/* some other remain counter */

	u8			x8abe;					// car
	
	Player *CheckPlyFirst;			// 0x8ac0 
	Player *CheckPlySecond;
	Player *PlayerFirst;			/* 8ac4 player pointers used wierdly by random p1/p2 */
	Player *PlayerSecond;			/*      selection for draw /collide order            */

	/*********************************************/
	/* Collision Globals - start of the "geepee" */
	/*********************************************/

	short	GPHitBoxDamage[3];	/* 8ac8,a,c */
	short	GPHitBoxCoords[3][2];	    /* 8ace Victims Hitboxes are copied here*/
	short	GPLeftEdge;			/* 8ada */
	short	GPTopEdge;			// 8adc
	short	VictimLeftEdge;		// 8ade
	short	VictimTopEdge;		// 8ae0
	short	GPCollX;			// 8ae2
	short	GPCollY;
	short	GPReg1;				/* 8ae6 */
	short	GPPointsReward;			/* CQArg points reward */
	
	Object		*x8aea;				/* used as a temp shortpointer */
	u16			x8aec;
	
	u8		GPHitBox4Hit;		/* default hitbox collision ? */
	u8   	GPHitBoxHit;		/* 8aef selected hitbox indicator * 2 */
	u8		GPHitBlocked;		/* GPDamageMod 8af0 */
	u8		GPWasProjectile;	/* Indicates collision from projectile not player */

	u32		x8af2;				/* used as a temp save */
	short	x8af6;				/* temp save 7e4b2 */
	
	/**********************/
	/* Drawsprite globals */
	/**********************/
	
	short	DSOffsetX;			/* 0x8af8 */
	short	DSOffsetY;
	short	hitbox_center_x;	/* 0x8afc */
	short	hitbox_center_y;
	u16		DBGPalette;			/* 8b00 */
	short	GPHitBoxCopy[4];	/* 8b02, 8b04, 8b06, 8b08 copied into at 7f60c*/
								/* 8b0a-d unused */
	short	x8b0e;				/*temp X for sin,cos */
	short	x8b10;
    short   GPCollDetect;		/* 8b12 boolean to indicate collision detected */
    short	x8b14;              /* gstate scroll1x minus temp in maint_scroll1x */
	short	x8b16;
	
	/************************/
	/* Testscreen globals   */
	/************************/
	
	u16		*ts_scr1;	// 8b18 
	u16		*ts_scr2;	// 8b1c
	u16		*ts_scr3;	// 8b20
	u16		x8b24, x8b26, x8b28, x8b2a;
	short	x8b2c;	/* 8b2c test screen index */
	short	x8b2e;	/* 8b2e test screen flips */
	char	x8b30;	// test autorepeat flag
	
	short	x8b5c;	/* used in test_offset_scroll1 */
	short	x8c02;	/* scroll1x adjustment */
	
	
    Object *x8e02;      /* free counter x1157 */
    Object *x8e06;
    
										// Sprite Layers
    Object *x8e16[4];			// L1G1
    Object *x8e26[4];			// L1G2
    Object *x8e46[8];			// L2G2
    Object *x8e66[8];			// L2G2
    Object *x8e86[8];			// L2G3
    Object *x8efe[30];			// L3G1
    Object *x8f76[30];			// L3G2
    Object *x8fee[30];			// L3G3
    Object *x9066[30];			// L3G4
    Object *x90de[30];			// L3G5
    Object *x9156[30];			// L3G6
    
	
	/* position unknown stuff */
	char	TextEffectBusy;
	char	GameMode;
} Game;

#endif /* INC_STRUCTS */
