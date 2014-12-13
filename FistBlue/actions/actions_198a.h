/*
 *  actions_198a.h
 *  GLUTBasics
 *
 *  Created by Ben on 16/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

#ifndef INC_ACT198A
#define INC_ACT198A

void actions_198a(void);			/* 249fa */

typedef enum actions_198a {
	BLANKA_FISH = 0,
	KEN_DRUMS,
	RYU_SIGNS,
	ACT_B03,
	ACTB_DRUMFIRE,			// BONUS3 Collisions
	ACTB_DRUMS,				// BONUS2 Collisions
	ACTB_CAR,				// BONUS0 Collisions
	ACTB_GUILE_CRATE,
	ACT_B08,
	ACT_B09,				// BONUS1 Collisions
} ObjG2ID;


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

typedef struct object_g2{
#include "std_object.h"
    FIXED8_8		AclX;
    FIXED8_8		AclY;
    u16				x0044;		
    signed char		UserByte;
    char			Timer2;						/* 0047 */
    char			BoundCheck;					/* 0x48 also a void *??? */
    char			BlockStun;
    char			SufferHB5;					// 0x4a
	char			NextReactMode;				/* 0x4b */
	char			NextReelStrength;				/* 0x4c  was NextReactMode2 */
	char			OnPlatform;					// 0x4d
	char			OnPlatform2;				// 0x4e
	u8				x004f[3];					/* for hitboxes 1,2,3 */
	short			DSOffsetX;
	char			Direction;					/* 0x54 FACING_LEFT=0, FACING_RIGHT=1 */
	u8				ReactMode;					/* 0x55 see RM_* */
	u8				ReactMode2;
	u8				StepSave;					// 0x57
	u8				Difficulty;
	u8				x0059;
	char			OnGround;					// 0x5a
	char			PlatformFallDir;			/* 0x5b direction falling off platform */
	short			DizzyClearCnt, DizzyCount, DizzySpell;
	short			IsBlockingSuper;			// 0062
	char			ThrowStat;
	short			Throw[6];					// XXX 3 x Point
	char			x0070;
	char			ProjectilePushBack;
	// gap
	u8				ThrowTrajectory;			/* 0x0076 */
	char			x0077;
	short			UndealtDamage;
	u16				RewardID;
	short			Damage1;					/* 0x7c */
	short			Damage2;
	
	/* userdata */
	union userdata_obb2 {
		UDcar		UDcar;
		UDbonus1	UDbonus1;
		UDbonus2	UDbonus2;
		UDbonus3	UDbonus3;
		UDactB08	UDactB08;
	} UD;
} Object_G2;

#endif // ACT_198A
