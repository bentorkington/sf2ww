/*
 *  chunli.h
 *  GLUTBasics
 *
 *  Created by Ben on 10/02/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#define CLM(arg0, arg1, arg2, arg3) \
ud->comp_farmove   = arg0;					\
ud->comp_closemove = arg1;					\
ud->comp_volley    = arg2;					\
ud->comp_distance  = arg3;							

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
	u8		mode_birdkick;		//0x0080
	u8		timer_birdkick0;
	char	timer_birdkick1;		
	char	birdkick_potential;
	u8		x0084;		// buttonstrength / 2, only set, never read
	char	x0085;		// multi hit counter
	
	short	x0086;		// only set, never read
	u8		x0090;
	char	didairthrow;	// check, may need to be sync with comp_didairthrow
	u8		x0092;
	u16		newbuttons;		// Thousand foot buttons
	char	mode_lightningkick;		// thousand foot kick state
	char	x0098;		// only set, never read
	ChunLiMove	cm1;	// 0099
	ChunLiMove	cm2;	// 009b
	ChunLiMove	cm3;	// 009d
	
	char	x009f;
	short	movedistance;
	char	closemove;		//00a2
	char	farmove;
	char	x00a4;		// is probably a dual
	char	x00a5;
	
} __attribute__((packed));

struct UserDataComp_ChunLi {
	u8		comp_farmove;		//h0080c	;		// far move
	u8		comp_closemove;		//h0081c;	// close move
	short	comp_distance;		// 0082
	
	u8		comp_select;			// move subsel
	u8		comp_multicount;
	DUAL	h0086;
	short	h0088s;
	DUAL	h008a;
	short	comp_bktimer;			// 008c
	short	comp_bkaccel;
	u8	    comp_volley;
	char	comp_didairthrow;			//0091
	char	x0092;		
};