/* sf2const.h  Street Fighter and CPS constants */


#ifndef INC_SF2CONST
#define INC_SF2CONST


#define PLAYER_USER_DATA_SIZE   0x80		

#define IMAGE_ATTR	0x8000		/* images are in tile,attr pairs */

#define TICKS_PER_SECOND  40		/* 40 decimal */


#define SCREEN_WIDTH   384
#define SCREEN_HEIGHT  256        /* only 224, 0xe0 according to MAME */

#define VISIBLE_MARGIN  0x40
#define STAGE_LENGTH	640		//0x280



/* game params */
#define ENERGY_START	0x90 
#define VS_SCREEN_DELAY 180 /* ticks */


/* end compile-time params */


#define TRUE  1
#define FALSE 0



/* end CPS I/O Constants */


/* other IO */
//ContrP[12].full
#define JOY_UP        0x8
#define JOY_DOWN      0x4
#define JOY_LEFT      0x2
#define JOY_RIGHT     0x1
#define BUTTON_A	 0x10
#define BUTTON_B	 0x20
#define BUTTON_C	 0x40
#define BUTTON_D	0x100
#define BUTTON_E	0x200
#define BUTTON_F	0x400
#define BUTTON_MASK	( BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_D | BUTTON_E | BUTTON_F )
//RawButtons
#define BUTTON_P1ST			0x10
#define BUTTON_P2ST			0x20
#define BUTTON_TESTMENU     0x40

#define STRENGTH_LOW		0
#define STRENGTH_MED		2
#define STRENGTH_HIGH		4

// XXX these are toward and away
#define STEP_RIGHT		1		
#define STEP_LEFT		0
#define	STEP_STILL		2

#define FACING_LEFT		0
#define FACING_RIGHT	1



// Jumpers

//JPParam
#define JP_DBGWAITMODE	0x01
#define JP_DBGNOCOLLIDE 0x02
#define JP_FREEPLAY		0x04
#define JP_FREEZE		0x08		// live
#define JP_FLIP			0x10		// live
#define JP_DEMOSOUND	0x20		// live
#define JP_ALLOWCONT	0x40
#define JP_DEBUG2		0x80		// live

//JPDifficulty
#define JP_DIFF1		0x01
#define JP_DIFF2		0x02
#define JP_DIFF4		0x04
#define JP_DIFFMASK		(JP_DIFF1|JP_DIFF2|JP_DIFF4)
#define JP_JAPANJUMP	0x08

//JPCost
#define JP_DBGSLEEP		0x01
#define JP_DBGNOTICK	0x08
#define JP_DBGTACTICS	0x10


#define SF2_TILE_OBJ_PEACE		0x815f
#define SF2_TILE_OBJ_DOTCLOTH	   0x4





#pragma mark Projectile Library
#define SF2PROJ_HADOUKEN		 0x0
#define SF2PROJ_SONICBOOM		 0x3


#pragma mark Actor Library
#define SF2ACT_0X02				 0x2	// China deco bicycle people
#define SF2ACT_DAS_BOAT			 0x3	// USA Ken deco 
#define SF2ACT_INDIA_ELEPHANTS   0x7	
#define SF2ACT_0X09				0x09		// makes lots of ACT_0E
#define SF2ACT_0X0E				0x0e
#define SF2ACT_0X10				0x10	// child of 06
#define SF2ACT_SF2LOGO          0x12
#define SF2ACT_0X13				0x13		// ryu sign frags
#define SF2ACT_0X1B				0x1b		// continue counter
#define SF2ACT_USAKEN_DRUMFRAGS	0x1c
#define SF2ACT_0X1D				0x1d
#define SF2ACT_HITSTUN          0x20
#define SF2ACT_0X21				0x21
#define SF2ACT_PREROUNDANIM		0x22
#define SF2ACT_VOMIT			0x23
#define SF2ACT_0X25				0x25		// perhaps ending seqs
#define SF2ACT_VEGA_LOSE_CLAW	0x28
#define SF2ACT_WWLOGO           0x29
#define SF2ACT_0X2E				0x2e		//Plane
#define SF2ACT_ATTRACT_FIGHT	0x2f
#define SF2ACT_0X31				0x31
#define SF2ACT_GROUNDDUST		0x35
#define SF2ACT_SCREENWOBBLE		0x36
#define SF2ACT_TIMEWARP			0x38
#define SF2ACT_SCORECOUNTER		0x3b
#define SF2ACT_BISONCAPE        0x3c
#define SF2ACT_CAPCOMLOGO       0x3e
#define SF2ACT_0X3F				0x3f
#define SF2ACT_0X40				0x40
#define SF2ACT_SPEAK_WINLOSE    0x48

 

#define BOTH_COMPUTER  0
#define ONLY_P1        1
#define ONLY_P2        2
#define BOTH_HUMAN     3


#define VERSION_JAP 0
#define VERISON_ETC 1
#define VERSION_USA 2

#define SCROLL_2	0
#define SCROLL_1	2
#define SCROLL_3	4



#define COUNT_LAYER1	8
#define COUNT_LAYER2	16
#define COUNT_LAYER3	60


#define PALETTE_0C       0xc
#define PALETTE_0D       0xd
#define PALETTE_OBJ_GOLDFIREBALL	15


enum sf2stages {
	STAGE_JAPAN_RYU,
	STAGE_JAPAN_EHONDA,
	STAGE_BRAZIL_BLANKA,
	STAGE_USA_GUILE,
	STAGE_USA_KEN,				/* 4 */
	STAGE_CHINA_CHUNLI,
	STAGE_USSR_ZANGIEF,
	STAGE_INDIA_DHALSIM,
	STAGE_THAILAND_BISON,		/* 8 */
	STAGE_THAILAND_SAGAT,
	STAGE_USA_BALROG,
	STAGE_SPAIN_VEGA,
	STAGE_BONUS_CAR,			/* 0xc */
	STAGE_BONUS_BARRELS,
	STAGE_BONUS_DRUMS,
	STAGE_BONUS_UNKNOWN,
	STAGE_DRUGS_HISCORES,		/* 10 */
	STAGE_UNKNOWN1,				// 11 skyscraper, worldmap, vs screen
	STAGE_UNKNOWN2,
	STAGE_UNKNOWN3,
};

#define LIBTEXT_ERASE		0x80
enum libtextgfx_sel {
	INSERT_COIN_P1,
	INSERT_COIN_P2,
	FREE_PLAY_P1,
	FREE_PLAY_P2,
	PUSH_START_P1,		//4
	PUSH_START_P2,
	ADD_COIN_P1,
	ADD_COIN_P2,
	GAME_OVER_P1,		//8
	GAME_OVER_P2,
	DRAW_GAME,			/* 0x0a */
	DOUBLE_KO,
	CONTINUE_P1,		//c
	CONTINUE_P2,
	TIME,				//e
	VITAL,
	BONUS,				//0x10
	ZEROZERO,			
	TIME_OVER,			
	BLANK_BAR_P1,
	BLANK_BAR_P2,		// 0x014
	GAME_OVER,
	PLAYER_SELECT,
	PLAYER_1,
	PLAYER_2,			//0x018
	HERE_COMES,
	CHALLENGER,
	HERE_COMES_2,
	CHALLENGER_2,		// 0x1c
	PERFECT,			/* 29 */
};

enum syslib04sel {
	COPYRIGHT_JAP,
	COPYRIGHT_USA,
	COPYRIGHT_ETC,
	DEMO,
	TITLE,						//4
	PLAY_DEMO,
	RANKING,
	CREDIT_EQUALS,	
	COIN_EQUALS,				//8
	INSERT_COIN,
	FREE_PLAY,
	PUSH_1P_START,
	PUSH_1POR2P_START,			//0xc
	INSERT_ADDITIONAL,
	START1PORADD2P,
	PLAYERSELECT,
	SANKA,						//0x10
	SL4_VERSION_JAP,
	SL4_VERSION_USA,
	SL4_VERSION_ETC,
	BATTLE,
	DEVCREDITS1,
	DEVCREDITS2,
	DEVCREDITS3,
	DEVCREDITS4,
	DEVCREDITS5,
};

enum syslib0csel {
	SL0C_FO_CL    = 0,
	SL0C_CLO      = 2,
	SL0C_CLS1     = 4,
	SL0C_CLS2     = 6,
	SL0C_CLS3     = 8,
	SL0C_FLS1     = 10,
	SL0C_FLS2     = 12,
	SL0C_FLS3     = 14,
	SL0C_FI_W     = 16,
	SL0C_FOCL_OBJ = 18,
	SL0C_FOCL_S1  = 20,
	SL0C_FOA_CLS2 = 22,
	SL0C_FOCL_S3  = 24,
	SL0C_FO_CLNOT1= 26,
	SL0C_FO_CLW	  = 28,
	SL0C_FOCLSA   = 30,
	SL0C_FOCLNOT1X= 32,
};

#define SL04	0x0400
#define SL0C	0x0c00
#define SL10	0x1000
#define SL1C	0x1c00

	


#endif /* INC_SF2CONST */
