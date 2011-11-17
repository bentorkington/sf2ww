/*
 *  chunli.h
 *  GLUTBasics
 *
 *  Created by Ben on 10/02/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#define CLM(arg0, arg1, arg2, arg3) \
ud->x0080 = arg0;					\
ud->x0081 = arg1;					\
ud->x0090 = arg2;					\
ud->x0082 = arg3;							
// check if this is a word write



struct chunlimove {
	u8	a;
	u8	b;
};
typedef struct chunlimove ChunLiMove;


void pl_cb_setstatus1_chunli(Player *player, short status);
void pl_cb_setstatus2_chunli(Player *player, short status, int argd0);
void pl_cb_setstatus3_chunli(Player *player, short status);

int PLCBPowerChunLi(Player *ply);
int PLCBStandChunLi(Player *ply);
int PLCBCrouchChunLi(Player *ply);
int PLCBJumpChunLi(Player *ply);
void PSCBAttackChunLi(Player *ply);

void PLCBCompAttackChunLi(Player *ply);
void PSCBVictoryChunLi(Player *ply);

struct UserData_ChunLi {
	u8	x0080;
	u8	x0081;
	short	x0082;		// XXX used as word for OppXDist, as byte for BirdKickTimer
	char	x0083;
	u8		x0084;		// buttonstrength / 2
	char	x0085;
	short	x0086;
	short	x0088;
	char	x008a;
	char	x008b;
	short	x008c;
	short	x008e;
	u8	x0090;
	char	x0091;
	u8	x0092;
	u16		x0094;		// Thousand foot buttons
	char	x0096;		// thousand foot kick state
	char	x0098;
	ChunLiMove	cm1;	// 0099
	ChunLiMove	cm2;	// 009b
	ChunLiMove	cm3;	// 009d
	
	char	x009f;
	short	x00a0;
	char	x00a2;
	char	x00a3;
	char	x00a4;
	char	x00a5;
	
} __attribute__((packed));

