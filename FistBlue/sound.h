/* lib.h */
#ifndef INC_SF2SOUND 
#define INC_SF2SOUND


/*
 *  sound.h
 *  GLUTBasics
 *
 *  Created by Ben on 20/01/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

void sound_cq_f0f7(void);
void queuesound(int data);
void setstagemusic(void);
void coinsound(void);


#define SOUND_PLAYERSELECTSCR	0x0e
#define SOUND_VS_SCREEN         0x0f
#define SOUND_VICTORY           0x10
#define SOUND_CONTINUE			0x11
#define SOUND_UNK_SAD			0x12
#define SOUND_GAME_OVER			0x13
#define SOUND_UNK_TADA			0x14
#define SOUND_CHALLENGER		0x15
#define SOUND_DEMOMUSIC         0x16
// 0x17 Unused
// ending music here
#define SOUND_COIN				0x20
#define SOUND_CURSORMOVE		0x21
#define SOUND_PLAYERSELECT		0x22
#define SOUND_CONTTICK			0x23
#define SOUND_UNK_DING			0x24
#define SOUND_PUNCH1			0x25
#define SOUND_PUNCH2			0x26
#define SOUND_PUNCH3			0x27
#define SOUND_IMPACT1			0x28
#define SOUND_IMPACT2			0x29
#define SOUND_IMPACT3			0x2a
#define SOUND_IMPACT4			0x2b
#define SOUND_IMPACT5			0x2c
#define SOUND_IMPACT6			0x2d
#define SOUND_GROUND_THUMP      0x2e
#define	SOUND_IMPACT8			0x2f		// landing on ground from jump 
// 0x30 empty
#define	SOUND_GLASS				0x31
#define	SOUND_BARRELS_BREAK		0x32
#define	SOUND_CAR_BREAK			0x33
// ending music
#define SOUND_SWOOSH1			0x36
#define	SOUND_SWOOSH2			0x37
#define	SOUND_ZAP1				0x38
#define SOUND_ELEPHANT			0x39
#define SOUND_DRUMS_FALL		0x3b
#define	SOUND_SWOOSH3			0x3c
#define	SOUND_PLANE				0x3d
#define SOUND_ZAP2				0x3e
// 0x3f unused
#define	SOUND_OOH				0x40
#define	SOUND_CHUNLI_FUCK		0x42
#define SOUND_KO_MALE           0x43
#define SOUND_KO_FEMALE         0x45
#define	SOUND_GINK				0x46
#define	SOUND_HUA				0x47
#define SOUND_CHUNLI_YUP		0x48
//0x 49 unused
#define SOUND_LAUGH_MALE		0x4a
// 4b unused
#define	SOUND_LAUGH_FEMALE		0x4c
#define SOUND_YODEL				0x4d
#define	SOUND_CHUNLI_WIN		0x4f
#define	SOUND_CROWD_LONG		0x50
#define	SOUND_CROWD_FADE		0x51
#define	SOUND_YOU				0x52
#define	SOUND_WIN				0x53
#define	SOUND_LOSE				0x54
#define SOUND_PERFECT			0x55
#define	SOUND_FIGHT				0x56
#define	SOUND_JAPAN				0x57
#define	SOUND_BRAZIL			0x58
#define	SOUND_USA				0x59
#define	SOUND_CHINA				0x5a
#define	SOUND_USSR				0x5b
#define	SOUND_INDIA				0x5c
#define	SOUND_SPAIN				0x5d
#define	SOUND_THAILAND			0x5e
#define	SOUND_ROUND				0x5f
#define	SOUND_ONE				0x60
#define	SOUND_TWO				0x61
#define	SOUND_THREE				0x62
#define	SOUND_FOUR				0x63
#define	SOUND_FIVE				0x64
#define	SOUND_SIX				0x65
#define	SOUND_SEVEN				0x66
#define	SOUND_EIGHT				0x67
#define	SOUND_NINE				0x68
#define	SOUND_FINAL				0x69
#define	SOUND_SHORYUKEN			0x6a
#define SOUND_HURRICANE			0x6b
#define SOUND_OINK				0x6c
#define SOUND_CANNONBALL		0x6d
#define SOUND_FIRE				0x6e
#define SOUND_FLAME				0x6f
#define	SOUND_SONIC_BOOM		0x70
#define SOUND_IMPACT7			0x71
#define SOUND_HIT_BLOCKED		0x72
#define	SOUND_BIRD_KICK			0x73
#define SOUND_HADOUKEN			0x74
#define SOUND_VEGA_JUMP			0x75
#define SOUND_TIGER				0x76
#define SOUND_UPPERCUT			0x77
// 78 unused
// faster music here
#define SOUND_YOGA				0x85
#define SOUND_ZAP3				0x86
#define SOUND_ZAP4				0x87
#define SOUND_ZAP5				0x88
#define SOUND_ZAP6				0x86
#define SOUND_ZAP1_AGAIN		0x87

#endif /* SF2SOUND */

