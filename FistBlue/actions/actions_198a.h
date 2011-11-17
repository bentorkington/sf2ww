/*
 *  actions_198a.h
 *  GLUTBasics
 *
 *  Created by Ben on 16/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

void actions_198a(void);			/* 249fa */


typedef struct {
	short as,bs;
} CarPlayer;


typedef struct {
	CarPlayer	p1;		// 80-83
	CarPlayer	p2;		// 84-88
	
	char	h0090c;
	char	h0091c;
	char	h0092c;
	char	h0093c;
} UDcar;


typedef struct {
#include "std_object.h"
    signed char		x0040;			/* some palette index, values 1,2,3,5 */
	signed char		x0041;
	void	*x0042;      /* palette? data at 0x90000 set by 82c26 used 82c4c */
    u8		UserByte;
    signed char	Timer2;				/* 0047 prob not a timer in Object2*/
	void	*x0048;
	
	u8 NextReactMode2;		/* 4c */
	char	x004d;
	signed char *x004e;	
	u8		x004f[3];    /* for hitboxes 1,2,3 */
	short   DSOffsetX;
	char    Direction;      
	u8      ReactMode;
	u8      ReactMode2;
	
	char	OnPlatform;
	char	OnPlatform2;
	
	u8      StepSave;		// 0057
	u8      Difficulty;
	u8      x0059;
	char    OnGround;
	char    PlatformFallDir;      /* direction falling off platform */
	short   DizzyClearCnt;
	short   DizzyCount;
	short   DizzySpell;
	short   IsBlockingSuper;		//0062
	char    ThrowStat;
	short   Throw[6];
	char    x0070;
	char    x0071;
	
	u8      ThrowTrajectory;    /* 0x0076 */
	char    x0077;
	short   UndealtDamage;
	u16     RewardID;
	short     Damage1;            /* 0x7c */
	short     Damage2;
	
	/* userdata */
	union userdata_obb2 {
		UDcar UDcar;
	} UD;
} Object_G2;

