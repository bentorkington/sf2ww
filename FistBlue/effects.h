/*
 *  effects.h
 *  GLUTBasics
 *
 *  Created by Ben on 18/01/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

void task_blinkers(void);
void task_game(void);
void task_scheduler(void);

void fadenwait1(void);
void fadenwait2(void);
void fadenwait3(void);
void fadenwait4(short argd0);
void fadenwait5(short arg0);


struct effectstate {
	int		FadeCounter;	// 5d3a 
	int		FadeObject;
	int		FadeScroll1;
	int		FadeScroll2;	// 5d46 
	int		FadeScroll3;
	int		x5d4e;
	int		x5d52;
	char	x5d56;
	char	FadeInEffect;	// 5d58 check this 
	u8		FadeBusy;       // 0x5d59   
};	



enum syslib08_arg2 {
    SL08_SCR1_ASCII,
    SL08_OBJ_8X8_ASCII,
    SL08_UNK1,
    SL08_TILETEXT,
    SL08_UNK2,
};

/*
 SL04,
 SL08, NN, -, SL08_SCR1_ASCII,
 SL08, NN, -, SL08_OBJ_8X8_ASCII,
 */
enum syslib04sel {
	TXTLIBA_COPYRIGHT_JAP,
	TXTLIBA_COPYRIGHT_USA,
	TXTLIBA_COPYRIGHT_ETC,
	TXTLIBA_DEMO,
	TXTLIBA_TITLE,						//4
	TXTLIBA_PLAY_DEMO,
	TXTLIBA_RANKING,
	TXTLIBA_CREDIT_EQUALS,
	TXTLIBA_COIN_EQUALS,				//8
	TXTLIBA_INSERT_COIN,
	TXTLIBA_FREE_PLAY,
	TXTLIBA_PUSH_1P_START,
	TXTLIBA_PUSH_1POR2P_START,			//0xc
	TXTLIBA_INSERT_ADDITIONAL,
	TXTLIBA_START1PORADD2P,
	TXTLIBA_PLAYERSELECT,
	TXTLIBA_SANKA,						//0x10
	TXTLIBA_VERSION_JAP,
	TXTLIBA_VERSION_USA,
	TXTLIBA_VERSION_ETC,
	TXTLIBA_BATTLE,
	TXTLIBA_DEVCREDITS1,
	TXTLIBA_DEVCREDITS2,
	TXTLIBA_DEVCREDITS3,
	TXTLIBA_DEVCREDITS4,
	TXTLIBA_DEVCREDITS5,
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

enum syslib10sel {
    SL10_CREDITS            = 0,
    SL10_HIGH_SCORE_TABLE   = 2,
};

#define SL04	0x0400
#define SL08	0x0800
#define SL0C	0x0c00
#define SL10	0x1000
#define SL1C	0x1c00

