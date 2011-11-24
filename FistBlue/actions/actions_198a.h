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
	
	char	boxes[4];		// hmm, overlap with 0x80?
	
	char	h008cc, h008dc, h008ec, h008fc;
	
	char	PeopleOnMe;		// 90 bitmask, people on me
	char	HitsTaken;		// 91
	char	h0092c;
	char	h0093c;
} UDcar;		// (bonus0)


// the Barrels
typedef struct {
	char	h0084c;
	
	char	h008cc;
	char	h008dc;
	char	h008ec;
	char	h008fc;
	
	FIXED16_16	Velocity, Accel;	// 90, 94
	short	h0098s;			// distance to P1
	short	h009as;			// distance to P2
	FIXED16_16 H009c;
	
} UDbonus1;


// the Drums
typedef struct {
	char	h0084c;
	
	char	h008cc;
	char	h008dc;
	char	h008ec;
	char	h008fc;
	
	char	h0092c;
	void	*H0092;
	void	*H0094;
	char	h0096c;
	char	h009ac;

} UDbonus2;

typedef struct {
	short h00b0s;
} UDbonus3;

typedef struct {
	char	h0080c;
} UDactB08;

typedef struct {
#include "std_object.h"
    FIXED8_8 AclX;
    FIXED8_8 AclY;
    u16 x0044;		
    u8		UserByte;
    signed char	Timer2;				/* 0047 prob not a timer in Object2*/
	char BoundCheck;
	char BlockStun;
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
		UDcar		UDcar;
		UDbonus1	UDbonus1;
		UDbonus2	UDbonus2;
		UDbonus3	UDbonus3;
		UDactB08	UDactB08;
	} UD;
} Object_G2;

