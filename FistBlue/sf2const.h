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



#define STRENGTH_LOW		0
#define STRENGTH_MED		2
#define STRENGTH_HIGH		4

// XXX these are toward and away
#define STEP_RIGHT		1		
#define STEP_LEFT		0
#define	STEP_STILL		2

// for Object.Flip and Object.EnemyDirection
#define FACING_LEFT		0
#define FACING_RIGHT	1


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
#define SF2ACT_ATTRACT_SPECT	0x30
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

//XXX 
#ifndef BUTTON_MASK
#define BUTTON_MASK 0x770
#endif

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

#endif /* INC_SF2CONST */
