/*
 *  ai.h
 *  GLUTBasics
 *
 *  Created by Ben on 16/11/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

void AIInitPlayer(Player *ply);

void AIBeginDef(Player *ply);
void AIBeginAgg0(Player *ply);
void AIBeginAgg1(Player *ply);

void AICheckThreats(Player *ply);		// 2c056 per-frame before comp_frontend
void AIPrepareNewState(Player *ply, short a);
void AIInitDefensive(Player *ply);

// XXX these should be in ai.h
struct data_2b7ea {
	const char	x023e[32];
	const unsigned char *x0134[16];	 
};

typedef const struct data_2b7ea AIAggTable;

struct dualptr {
	const u8	***a1;
	const AIAggTable  ***a2;
};

struct defense_strategy {
	char		high_energy[32];
	char		low_energy[32];		/* selector to use if energy is very low */
	const u8	*codes[];		
};


#define AIB_TYPE0		0
#define AIB_TYPE2		2
#define	AIB_TYPE4		4

#define STRAT_STANDSTILL	0   /* if AI1 = 0x81,0xc0 then read AI2 also */
#define STRAT_SHORTWALK		2	// 1
#define STRAT_LONGWALK		4	// 2
#define STRAT_SETBLOCK		6	// 1
#define STRAT_JUMP			8	// 4
#define STRAT_ATTACK	   10	// 3
#define STRAT_KICK         12	// 2
#define STRAT_STUN		   14	// 1

#define AIB_STANDSTILL		0   /* if AI1 = 0x81,0xc0 then read AI2 also */
#define AIB_SHORTWALK		2	// 1
#define AIB_LONGWALK		4	// 2
#define AIB_SETBLOCK		6	// 1
#define AIB_JUMP			8	// 4
#define AIB_ATTACK			10	// 3
#define AIB_KICK			12	// 2
#define AIB_STUN			14	// 1

/* AI Bytecode Defs */
#define AIB_RESTART			0x80
#define AIB_EXIT3			0x82
#define AIB_EXIT4			0x84
#define AIB_EXITRAND		0x86
#define AIB_COLLDIS			0x88
#define AIB_EXIT5_1			0x8a		/* set threat mode */
#define AIB_EXIT5_0			0x8c
#define AIB_EXIT5_2			0x8e
#define AIB_WALLBOUNCE		0x90
#define AIB_B94_NODIZZY		0x92		// YON
#define AIB_LABEL_94		0x94
#define AIB_SET_0216		0x96
#define AIB_MAYBE_AGG1		0x98
#define AIB_GO_AGG1			0x9a
#define AIB_MAYBE_RESTART	0x9c
#define AIB_BA0_DIST_LE		0x9e		// KYON
#define	AIB_LABEL_A0		0xa0
#define AIB_BA4_OPPJUMP		0xa2		// TAN 
#define AIB_LABEL_A4		0xa4
#define	AIB_BA8_OPPX_LE		0xa6		// GTAN
#define	AIB_LABEL_A8		0xa8
#define AIB_BAC_OPPJUMP		0xaa
#define AIB_LABEL_AC		0xac
#define AIB_BB0_NOTWITHIN	0xae		// TIGA
#define AIB_BB2				0xb0
#define AIB_LABEL_B2		0xb2
#define AIB_SETIMMUNE		0xb4




#define AIB_STRL		0x0
#define AIB_STRM		0x2
#define AIB_STRH		0x4

#define AIB_POWERMOVE	0x50
#define AIB_VOLLEY		0x40	// alias for AIB_SHIFT
#define AIB_SHIFT		0x40
#define	AIB_THROW		0x80
#define AIB_WALKAWAY	0x80	