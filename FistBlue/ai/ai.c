/*
 *  ai.c
 *  GLUTBasics
 *
 *  Created by Ben on 16/11/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */



#include <stdio.h>

#include "sf2.h"

#include "structs.h"
#include "player.h"

#include "particle.h"

#include "structs.h"
#include "sprite.h"

#include "particle.h"
#include "playerstate.h"
#include "reactmode.h"
#include "rules.h"
#include "actions.h"
#include "collision.h"

#include "lib.h"

#include "computer.h"


#include "ai.h"
#include "aidata.h"


#define debug_print(level, fmt, ...) \
do { if (FISTBLUE_DEBUG_AI >= level) fprintf(stderr, "%s:%d:%s(): " fmt, "AI", \
__LINE__, __func__, __VA_ARGS__); } while (0)

#define DEBUG_AI_STR(level, string) \
do { if (FISTBLUE_DEBUG_AI >= level) fprintf(stderr, "%s:%d:%s():%s ", "AI", \
__LINE__, __func__, string); } while (0)


int debug_ai_recurse;		// used for debugging how deep the AI recurse loop goes
extern struct game g;


static short comp_ply_difficulty_lookup (Player *ply, const u32 data[][32]);	//2bf4e
static void _AICheckUpdate(Player *ply);	
static void _AISearch94NotDizzy(Player *ply);
static void _AISearchA0DistLessOrEqual(Player *ply);
static void _AIDoStrategy(Player *ply,  short d0);
static void _AIStrategyHighBit(Player *ply, u8 d0);
static void _AIFinish(Player *ply);
static void _AISetAttackParams(Player *ply);
static void _AIGotoNextStrategy(Player *ply);		/* 2b8da */
static void sub_2adee(Player *ply);		/* 2adee */
static void _AISearchA4IfOppJump(Player *ply);
static short opponent_within(Player *ply, short d0);
static short _AIClosestThreat(Player *ply);		// 2bcd8 closest threat
static void _AIStratStandStill(Player *ply);
static void _AISetStrategy(Player *ply, char arg_d0);		// 2b93a
static void _AIBeginStrategy(Player *ply);
static void _AINewGoAggressive(Player *ply);
static void _AISearchStrategy (Player *ply, unsigned char d3);
static void _AIExit1(Player *ply);
static void AIAggHigh(Player *ply, short d0);
static void _AISetAgg1(Player *ply, AIAggTable **a2);	// 2b7ea
static void _AIDefHigh(Player *ply, short d0);



#pragma mark Difficulty Lookups

// 0x97522: 10%, 12.5%, 16%, 19%, 22%, ... 100%, 100%

const static u32 data_97522[12][32] = {		
	// 10%, 12.5%, 16%, 19%, 22%, ... 100%, 100%
    { 0xe0000000, 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xe0000000, 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xe0000000, 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xe0000000, 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xe0000000, 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xe0000000, 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xe0000000, 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xe0000000, 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
	/* Bosses */
    { 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
    { 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 
	},
};
/* next address 00097b3a */
const static u32 data_97b3a[12][32] = {
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xfff00000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffe00, 0xffffff00, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffe00000, 0xfff80000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffe00, 0xffffff00, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xfff00000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 
		0xffffe000, 0xfffff000, 0xfffffc00, 0xfffffe00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 
		0xffffe000, 0xfffff000, 0xfffffc00, 0xfffffe00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xffc00000, 0xffe00000, 0xfff80000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 
		0xffffe000, 0xfffff000, 0xfffffc00, 0xfffffe00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xffc00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 
		0xffffe000, 0xfffff000, 0xfffffc00, 0xfffffe00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 
		0xffffe000, 0xfffff000, 0xfffffc00, 0xfffffe00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 
		0xffffe000, 0xfffff000, 0xfffffc00, 0xfffffe00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfff80000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 
		0xffffc000, 0xffffe000, 0xfffff800, 0xfffffc00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfff80000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 
		0xffffc000, 0xffffe000, 0xfffff800, 0xfffffc00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfff80000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 
		0xffffc000, 0xffffe000, 0xfffff800, 0xfffffc00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfff80000, 0xfffc0000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 
		0xffffc000, 0xffffe000, 0xfffff800, 0xfffffc00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 
	},
};
/* next address 00098152 */
const static u32 data_9876a[12][32] = {
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
    { 0xffff0000, 0xffff0000, 0xfffe0000, 0xfffe0000, 0xfffc0000, 0xfffc0000, 0xfff80000, 0xfff80000, 0xfff00000, 0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 0xff800000, 0xff000000, 0xff000000, 
		0xfe000000, 0xfe000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xfc000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf8000000, 0xf0000000, 0xf0000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000, 
	},
};
/* next address 00098d82 */
const static u32 data_98152[12][32] = {
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xffc00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 0xffffe000, 0xfffff000, 
		0xfffff800, 0xfffffc00, 0xffffff00, 0xffffff80, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffff8, 0xfffffff8, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 0xffffe000, 0xfffff000, 
		0xfffff800, 0xfffffc00, 0xffffff00, 0xffffff80, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffff8, 0xfffffff8, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffe000, 0xfffff000, 0xfffff800, 0xfffff800, 0xfffffc00, 
		0xfffffe00, 0xffffff00, 0xffffffc0, 0xffffffe0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xfff00000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffe000, 0xfffff000, 0xfffff000, 0xfffff800, 0xfffff800, 0xfffffc00, 
		0xfffffe00, 0xffffff00, 0xffffffc0, 0xffffffe0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xffc00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff8000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 0xffffe000, 0xfffff000, 
		0xfffff800, 0xfffffc00, 0xffffff00, 0xffffff80, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffff8, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffe00000, 0xfff80000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 0xffffe000, 0xfffff000, 0xfffff000, 0xfffff800, 
		0xfffffc00, 0xfffffe00, 0xffffff80, 0xffffffc0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffc, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff800000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 0xffffc000, 0xfffff000, 
		0xfffff800, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xfff00000, 0xfffc0000, 0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffe000, 0xfffff000, 0xfffff000, 0xfffff800, 0xfffff800, 0xfffffc00, 
		0xfffffe00, 0xffffff00, 0xffffffc0, 0xffffffe0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffff8, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffe00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffff8, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
    { 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 0xfffe0000, 0xffff0000, 0xffff0000, 0xffff8000, 0xffff8000, 0xffffc000, 0xffffc000, 0xffffe000, 
		0xfffff000, 0xfffff800, 0xfffffe00, 0xffffff00, 0xffffffc0, 0xffffffe0, 0xffffffe0, 0xfffffff0, 0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 
	},
};
/* next address 0009876a */
static const char data_2b8a4[16] = { 1,2,1,0,1,2,1,0,2,1,2,1,1,1,2,1 };



#pragma mark ---- AI Init ----
struct dualptr _AILookupStrategy(Player *ply) {		// 2b78e
    // todo: modify this to take a pointer to a struct rather than return a struct.
	struct dualptr retval;
	retval.a1 = dataAIAggressive[ply->FighterID]->a1[ply->OpponentID];
	retval.a2 = dataAIAggressive[ply->FighterID]->a2[ply->OpponentID];
		
	return retval;
}
static void AISetAgg0(Player *ply, const u8 **a1, const AIAggTable **a2) {	// 2b7ce
	/* do something with a1 and a2 returned by _AILookupStrategy */
	
	ply->AIStratAgg0  = a1[ply->RoughTimeRemain];
	ply->AITypeAgg0 = *ply->AIStratAgg0;		/* lookup and store first instruction */
	ply->AIStratIndexAgg0 = 1;
	
	_AISetAgg1(ply, a2);
}
// in original, a2 points to u16[] of offsets to chars
static void _AISetAgg1(Player *ply, const AIAggTable **a2) {		// 2b7ea
	
	const struct data_2b7ea *ptr = a2[ply->RoughTimeRemain];
	ply->x023e = ptr->x023e[RAND32];
	
	
	
	ply->AIStratAgg1 = ptr->x0134[ply->x023e];
	
	ply->AITypeAgg1 = *ply->AIStratAgg1;
	ply->AIStratIndexAgg1 = 1;	
#if FISTBLUE_DEBUG_AI >= 2
	int i;
	
	debug_print(2, "side %d @ 0x%08x 0x%02x ply->AITypeAgg1 = 0x%x\n",ply->Side,ply->AIStratAgg1, ply->x023e,ply->AITypeAgg1);
	for (i=0; i<32; i++) {
		printf("%02x ", ply->AIStratAgg1[i]);
	}
	DEBUG_AI_STR(2, "\n");
#endif
}

void AIInitPlayer(Player *ply) {		//2b780
	struct dualptr DP;
	ply->AIStartAgain=ply->RoughTimeRemain;
	DP=_AILookupStrategy(ply);
#if FISTBLUE_DEBUG_AI >= 2
	int i;

	debug_print(2, "a1: 0x%08x  a2: 0x%08x\n\tDUMP A1> ", DP.a1, DP.a2);
	for (i=0; i<32; i++) {
		printf("%02x ", DP.a1[i]);
	}
	DEBUG_AI_STR(2, "\n");
#endif
	
	AISetAgg0(ply, DP.a1, DP.a2);
}
static short sub_2c120(Player *ply, Player *a0, short *d6) {	
	/* one caller */
	short d0;
	u16 *yokeptr;
	
	if (*d6 != 6 && *d6 != 0x18) {
		return TRUE;
	}
	if (a0->Airborne == AIR_ONGROUND) {
		return FALSE;
	}
	yokeptr = data_995de[a0->FighterID];	
	if(a0->OppXDist > yokeptr[23]){		// beats me
		*d6 = -1;
		return FALSE;
	}
	d0 = ABS(get_scr2x() - a0->XPI) - 0x60;
	if (0xc0 > d0) {
		return TRUE;
	}
	if (a0->VelX.full >= 0) {
		if (0xe0 < d0) {
			*d6 = -1;
			return FALSE;
		}
		*d6 = 0x17;
		return TRUE;
	}
	if (d0 >= 0) {
		*d6 = -1;
		return FALSE;
	}
	*d6=0x17;
	return TRUE;
}

/* 2c0ce  first check if there is a fireball, then if the opponent is attacking us
 return TRUE if so. One caller */

static short sub_2c0ce(Player *ply) {
	short d6;
	
	if (ply->Opponent->Projectile) {
		if(ply->Opponent->Projectile->x0024)  {		// is is a threat?
			d6 = ply->Opponent->Projectile->ActionScript->Yoke;
			if(sub_2fe6(ply, ply->Opponent->Projectile, 
						d6)) {
				ply->YokeAttackingMe = d6;
				return TRUE;
			}
		}
	}
	if (ply->Opponent->IsJumpThreat==0 && ply->Opponent->Attacking==0 ) {
		return FALSE;
	}
	d6=ply->Opponent->ActionScript->Yoke;
	
	if (d6 < 0) {
		return FALSE;
	}
	if (sub_2fe6(ply, (Object *)ply->Opponent, d6)==0) {
		return FALSE;
	}
	if (sub_2c120(ply,ply->Opponent,&d6)==0) {
		return FALSE;
	}
	ply->YokeAttackingMe = d6;
	return FALSE;
}

void AICheckThreats(Player *ply) {		// 2c056 per-frame before comp_frontend
	if(ply->AIThreatCheckMode == 0) {		// check for threats if enabled
		/* 2c09a */
		ply->AITimerThreatCheck = 0;				// no threat, reset timer
		ply->AIThreatFound = FALSE;
		return;
	}
	if(sub_2c0ce(ply) == 0) {
		ply->AITimerThreatCheck = 0;				// no threat, reset timer
		ply->AIThreatFound = FALSE;
		return;
	}	
	if(ply->AITimerThreatCheck) {
		--ply->AITimerThreatCheck;
		ply->AIThreatFound = FALSE;
	} else if (ply->AIThreatFound || ply->AIThreatCheckMode != 2 ||
			   comp_ply_difficulty_lookup(ply, data_98152) ) {
		ply->AIThreatFound=TRUE;
	} else {
		/* delay next threat check for 30f */
		ply->AITimerThreatCheck = 30;		
		/* was a table lookup, but they were always the same */
	}
}

static short comp_ply_difficulty_lookup (Player *ply, const u32 data[][32]) {	//2bf4e
	return data[ply->FighterID][ply->Difficulty] & (1 << RAND32);
}

/* 2b768 reset AI modes */
inline static void _AIResetState(Player *ply) {
	ply->AIMode1=0;
	ply->AIMode2=0;
}

/* 2b740 AIPrepareNewState checked  */
void AIPrepareNewState(Player *ply, short Type) {
	debug_print(2, "side %d type %d\n",ply->Side, Type);
    switch (Type) {	/* a is first char of AI Script, [0,2,4] */
		case 0:
			if(ply->AIForceDefensive || ply->AIStartAgain == ply->RoughTimeRemain) {
				ply->AIMode2 = 0;
				/* don't reset the other state */
				return;
			} else {
				ply->AIStartAgain = 0;
				_AIResetState(ply);
			}
			break;
		case 2:
			_AIResetState(ply);
			break;
		case 4:
			if(comp_ply_difficulty_lookup(ply,data_9876a)) {
				/* inlined case 0 here */
				if(ply->AIForceDefensive || ply->AIStartAgain == ply->RoughTimeRemain) {
					ply->AIMode2 = 0;
				} else {
					ply->AIStartAgain = FALSE;
					_AIResetState(ply);
				}
			} else {
				_AIResetState(ply);
			}
			break;
		FATALDEFAULT;
	}
}

/* 2bf74 */
static short _AIShouldGoAggressive(Player *ply) {
	if (   ply->AIForceDefensive  != FALSE
		|| ply->AIAgressive		  != FALSE
		|| ply->AIAllowAggressive == FALSE
	) {
		return FALSE;
	} else {
		debug_print(1, "side %d TRUE\n", ply->Side);
		return TRUE;
	}
}

/* 2c032 */
static short _AIOpponentClose(Player *ply) {
	short d1 = ply->Size + 64;
	short d0 = ply->XPI - ply->Opponent->XPI + d1;
	
	if(d0 > d1 * 2 || d0 < 0) {
		return FALSE;
	} else {
		return TRUE;
	}
}
static void _AIAggressiveIfClose(Player *ply) {			/* 2bfac checked, one caller */
	if (_AIOpponentClose(ply)==FALSE) {
		/* 2bffe */
		ply->AIAggTimer1 = ply->AIAggTimer0 = ply->AIAllowAggressive = 0;
	} else if (ply->AIAggTimer1) {
		/* 2c00c */
		ply->AIAggTimer1--;
	} else if (ply->AIAllowAggressive) {
		return;
	} else if (ply->AIAggTimer0) {
		/* 2bfcc */
		if(--ply->AIAggTimer0) { return; }
		if (comp_ply_difficulty_lookup(ply, data_97b3a)) {
			ply->AIAllowAggressive = 1;
		}
		else {
			ply->AIAggTimer1 = 30;		/* flattened lookup all same */
			ply->AIAggTimer0 = ply->AIAllowAggressive = 0;
		}
	} else {
		ply->AIAggTimer0 = ply->AITimers[1];
	}
}
static void _AIStrat14(Player *ply) {
	switch (ply->AIMode2) {
		case 0:
			/* 2b640 */
			NEXT(ply->AIMode2);
			ply->CompDoBlockStun = TRUE;
			ply->x027b = ply->AIParam1;
			if (ply->FighterID == FID_VEGA && ply->x027b == 1) {
				ply->x0236 = TRUE;
			} else {
				ply->x0236 = FALSE;
			}
			ply->AIBlockStunTimer = 1 * TICKS_PER_SECOND;		// array all same.
			break;
		case 2:
			// 2b69a
			if (ply->CompDoBlockStun && ply->AIThreatFound) {
				if (ply->x0236) {
					return;
				}
				if (--ply->AIBlockStunTimer) {
					return;
				}
			}
			ply->CompDoBlockStun = FALSE;
			ply->x0236 = FALSE;
			_AIFinish(ply);
			break;
		FATALDEFAULT;
	}
}
static void _AICheckAggressive(Player *ply) {			/* 2b08a */
	if (_AIShouldGoAggressive(ply)) {
		_AINewGoAggressive(ply);
	}
}	
/* 2ccb6 */
static short _AIHasReachedWalkTarget(Player *ply) {
	if (ply->XPI - ply->AIWalkTarget > 4 ||
		ply->XPI - ply->AIWalkTarget < -4) {		
		return 0;
	} else {
		debug_print(3, "TRUE side %d\n", ply->Side);
		return 1;
	}
}
static void sub_2adee(Player *ply) {		/* 2adee not ForceDefensive */
	_AIAggressiveIfClose(ply);
	_AIBeginStrategy(ply);
}

#pragma mark ---- ENTRY ----

/* _AIBeginAgain, recursive, so be careful   2b6ca */
static void _AIBeginAgain(Player *ply) {	
	debug_ai_recurse++;

	if (debug_ai_recurse > 20) {
		printf("AI Recursed too deep!\n");
		panic(0);
	}
	ply->AIMode2 = 0;
	ply->AISigAttack = ply->CompDoBlockStun =
	ply->x0236 = ply->CompDoJump = 0;
	ply->AIWalkDirection |= STEP_STILL;
	if (ply->AIForceDefensive) {
		AIBeginDef(ply);				
	} else if (ply->AIAgressive) {
		AIBeginAgg1(ply);
	} else {
		AIBeginAgg0(ply);
	}
}

/* Entry from computer.c at either AIBeginAgg0, AIBeginAgg1 or AIBeginDef */

/* 2adca per frame aggressive=0 */
void AIBeginAgg0(Player *ply) {	

	if (ply->AIMode1==0) {
		ply->AIMode1 += 2;
		ply->InGroundAttack = FALSE;
		_AICheckUpdate(ply);
		if (ply->AISaveState != 0) {
			ply->AISaveState = 0;		/* throw away the state */
			NEXT(ply->AIMode1);
			_AIBeginStrategy(ply);
			debug_ai_recurse = 0;
			return;
		}
	} 
	sub_2adee(ply);
	debug_ai_recurse = 0;
}

/* 2ae10 per frame aggressive processor */
void AIBeginAgg1(Player *ply) {	
	if (ply->AIMode1) {
		_AIBeginStrategy(ply);
		return;
	} else {
		/* 2ae16 */
		ply->AIMode1 += 2;
		ply->InGroundAttack = 0;
		_AICheckUpdate(ply);
		if (ply->AISaveState != 0) {
			ply->AISaveState = 0;
			ply->AIMode1 += 2;
			sub_2adee(ply);
			debug_ai_recurse = 0;
			return;
		}
		_AIBeginStrategy(ply);
	}
	debug_ai_recurse = 0;
}

/* 2ad4e per frame defensive processor */
void AIBeginDef(Player *ply) {	
	/* AI begin ForceDefensive */
	if (ply->AIMode1==0) {
		ply->AIMode1 += 2;
		_AICheckUpdate(ply);
		if (ply->AISaveState < 0) {
			DEBUG_AI_STR(2, "_AIBeginDef: Restoring saved state\n");
			/* 2ad82 - restore params instead */
			ply->AIStrategy = ply->x0209;
			ply->AIParam1 = ply->x020a;
			ply->AIParam2 = ply->x020b;
			ply->AIParam3 = ply->x020c;
			ply->AIParam4 = ply->x020d;
			ply->AIAgressive = ply->x0218;
			ply->AIMode1 = ply->x0219;
			ply->InGroundAttack = ply->AIMode2 = ply->CompDoBlockStun = ply->x0236 = 0;
			ply->YokeSaved = -1;	/* Will not match */
			_AIBeginAgain(ply);	 
			debug_ai_recurse = 0;
			return;
		}
	}
	_AIBeginStrategy(ply);		/* asm actually calls 2ad64, but that is identical */
	debug_ai_recurse = 0;
}


#pragma mark ---- _AIFinish ----

static void _AIFinish(Player *ply) {		// 2b6c6 was _AINew201
	ply->AIMode1 = 0;
	_AIBeginAgain(ply);
}

static void _AINewGoAggressive(Player *ply) {		// 2bf90
#ifdef FISTBLUE_DEBUG_AI
	debug_print(3, "side %d\n",ply->Side);
#endif
	ply->AIAgressive = 2;
	ply->AIAllowAggressive = ply->AIAggTimer0 = ply->AIAggTimer1 = 0;
	ply->AIStratIndexAgg1 = 1;
	_AIFinish(ply);
}
static void _AINewTimer(Player *ply) {		// 2af28
	if(--ply->AITimer == 0) {
		_AIFinish(ply);
	} else {
		if(_AIShouldGoAggressive(ply)){
			_AINewGoAggressive(ply); 
		}
	}
}
static void _AINewTimerTooClose(Player *ply) {		//2af78
	if (ply->AITooClose > ply->OppXDist) {
		_AIFinish(ply);
		return;
	}
	if(--ply->AITimer == 0) {
		_AIFinish(ply);
		return;
	}
	if (_AIShouldGoAggressive(ply)) {
		_AINewGoAggressive(ply);
	}
}
static void _AINewIfNoFireball(Player *ply) {		//2af3a
	/* 2af3a */
	if (ply->Projectile == NULL) {
		_AIFinish(ply);
	} else {
		if (_AIShouldGoAggressive(ply)) {
			_AINewGoAggressive(ply);
		}
	}
}	
static void _AINewAggIfOppJump(Player *ply) {
	/* 2af4c */
	if (ply->Opponent->Airborne) {
		if (ply->Opponent->YPI <= ply->Opponent->OldOldY.part.integer ||
			ply->Opponent->YPI >= ply->AIOppJumpHeight) {
			if (_AIShouldGoAggressive(ply)) {
				_AINewGoAggressive(ply);
			}
			return;
		} 
	}
	_AIFinish(ply);
}	
static void _AINewIfHeadHittable(Player *ply) {		// 2af96
	if (ply->Opponent->ActionScript->HB_Head) {
		_AIFinish(ply);
	} else if (_AIShouldGoAggressive(ply)) {
		_AINewGoAggressive(ply);
	}
}


#pragma mark ---- AI STRATEGIES ----

// STRAT_STANDSTILL
static void _AIStratStandStill(Player *ply) {		/* 2ae50 standing still*/
	/* checked once */
	switch (ply->AIMode2) {
		case 0:
			/* 2ae68 */
			debug_print(4, "%02x %02x\n", ply->AIParam1, ply->AIParam2);
			NEXT(ply->AIMode2);
			ply->CompDoJump       = 
			ply->AISigAttack      = 
			ply->CompDoBlockStun  =
			ply->x0236            = 0;
			ply->AIWalkDirection |= STEP_STILL;
			
			if(ply->AIParam1 & 0x80) {
				if (ply->AIParam1 & 1) {				/* 2aedc */
					ply->AIMode2    = 8;				//AINewTimerTooClose
					ply->AITooClose = ply->AIParam2;
					ply->AITimer    = (char []){		/* data_2af08 */ 
						120, 120, 120, 120, 120, 120, 120, 120, 100, 100, 100, 100, 100, 100, 100, 100, 
						80,  80,  80,  80,  80,  80,  80,  80,  60,  60,  60,  60,  60,  60,  60,  60, 
					}[RAND32];
					_AINewTimerTooClose(ply);
				} else if (ply->AIParam1 & 0x40) {
					/* 2aeb8 */
					ply->AIMode2 += 4;				//NewAggIfOppJump
					ply->AIOppJumpHeight = ply->AIParam2+0x28;	
					if (ply->Opponent->Airborne != AIR_ONGROUND) {
						_AINewAggIfOppJump(ply);
					} else {
						/* inlined 2af6e */
						if (_AIShouldGoAggressive(ply)) {
							_AINewGoAggressive(ply);
						}
					}
				} else if (ply->AIParam1 & 0x2) {
					/* 2aefe */
					ply->AIMode2 = 10;	// AINewIfHeadHittable
					_AINewIfHeadHittable(ply);
				} else if (ply->Projectile==0) {
					_AIFinish(ply);
				} else {
					NEXT(ply->AIMode2);
					_AINewIfNoFireball(ply);
				}
			} else {

				ply->AITimer = ply->AITimers[ply->AIParam1];
				_AINewTimer(ply);
			}
			break;
			
		case 2:	_AINewTimer(ply);			break;
		case 4:	_AINewIfNoFireball(ply);	break;
		case 6:	_AINewAggIfOppJump(ply);	break;
		case 8: _AINewTimerTooClose(ply);	break;
		case 10: _AINewIfHeadHittable(ply); break;
		FATALDEFAULT;
	}
}	

// STRAT_KICK 0x2b58c
static void _AIStratKick(Player *ply) {
	switch (ply->AIMode2) {
		case 0:
			NEXT(ply->AIMode2);
			ply->AISigAttack = TRUE;
			if (ply->XPI > STAGE_LENGTH || ply->XPI < 0) {
				ply->IsWithinBounds = FALSE;
			}
			else {
				ply->IsWithinBounds = TRUE;
			}
			if (ply->AIForceDefensive==0 && ply->x0216 != 0) {
				if(ply->Opponent->ActionScript->Crouch) {
					ply->CompDoBlock = TRUE;
				}
				else {
					ply->CompDoBlock = FALSE;
				}
			}
			//2b5de
			ply->PunchKick      = PLY_KICKING;
			ply->AIVolley       = FALSE;
			ply->AISigSpecial	= FALSE;
			if (ply->AIParam1 & AIB_THROW) {
				ply->CompDoThrow = TRUE;
			}
			else {
				ply->CompDoThrow = FALSE;
			}
			if (ply->AIParam1 & AIB_VOLLEY) {
				ply->DiceRollCount = ply->AIParam2;	
				ply->AIVolley = TRUE;
			}
			ply->ButtonStrength = ply->AIParam1 & 0xf;
			break;
		case 2:			// 2b626
			if (ply->AISigAttack == FALSE) {
				_AIFinish(ply);
			}
			break;
		FATALDEFAULT;
	}
}

//STRAT_SHORTWALK
static void _AIStratShortWalk(Player *ply) {		/* 2afb0 */
	short temp, d0;
	switch (ply->AIMode2) {
		case 0:
			NEXT(ply->AIMode2); 
			ply->CompDoBlock = FALSE;
			ply->CompDoJump  = FALSE;
			if (ply->AIParam1 & 0x40) {
				if (ply->AIParam1 & AIB_WALKAWAY) {
					ply->AIWalkDirection = FACING_RIGHT;	//away	
				} else {
					ply->AIWalkDirection = FACING_LEFT;		//toward
				}
				if (ply->Flip != FACING_LEFT) {
					ply->AIWalkDirection ^= 1;
				}
				
				if (ply->AIWalkDirection) {
					temp = data_2b09a[ply->AIParam1 & 0xf][RAND32];		//size of = 32bytes
				} else {
					temp = -data_2b09a[ply->AIParam1 & 0xf][RAND32];	
				}
				ply->AIWalkTarget = temp + ply->XPI;
			} else {
				// 2b022
				if (ply->AIParam1 & AIB_WALKAWAY) {
					ply->AIWalkDirection = FACING_RIGHT;	/* away */
				} else {
					ply->AIWalkDirection = FACING_LEFT;	/* toward */
				}
				if (ply->Flip) {
					ply->AIWalkDirection ^= 1;
				}
				if (ply->AIWalkDirection) {
					temp = data_2b094[ply->AIParam1 & 0xf];		/* RIGHT */
				} else {
					temp = -data_2b094[ply->AIParam1 & 0xf];	/* LEFT */
				}
				ply->AIWalkTarget = temp + ply->XPI;
			}
			/* FALLTHRU */
		case 2:
			// 2b05a
			if (_AIHasReachedWalkTarget(ply)) { 
				_AIFinish(ply);
			} else if (g.GPCollDetect && ply->PushDirection != ply->AIWalkDirection) {
				_AIFinish(ply); 
			} else {
				d0 = ply->BoundCheck;
				if (ply->BoundCheck==0) {
					d0 = ply->PlatformFallDir;
					if (d0 == 0) {
						_AICheckAggressive(ply);
						return;
					}
				}
				/* Convert Boundcheck result to compare with AIWalkDirection */
				--d0;
				if (ply->AIWalkDirection != d0) {
					/* Backed into a wall */
					_AICheckAggressive(ply);
				} else {
					_AIFinish(ply);
				}
			}
			break;
		FATALDEFAULT;
	}	
}

//STRAT_ATTACK	0x2b4be
static void _AIStratAttack(Player *ply) {		
	switch (ply->AIMode2) {
		case 0:
			NEXT(ply->AIMode2);
			ply->AISigAttack = TRUE;

			if (ply->XPI > STAGE_LENGTH || ply->XPI < 0) {
				ply->IsWithinBounds = FALSE;
			} else {
				ply->IsWithinBounds = TRUE;
			}
			if (ply->AIForceDefensive == FALSE && ply->x0216) {
				if (ply->Opponent->ActionScript->Crouch) {
					ply->CompDoBlock = TRUE;
				} else {
					ply->CompDoBlock = FALSE;
				}
			}
			_AISetAttackParams(ply);
			break;
		case 2:				//2b582
			// Wait for the attack to finish, 
			if (ply->AISigAttack == FALSE) {
				_AIFinish(ply);
			}
			break;
		FATALDEFAULT;
	}
}

static short _AISetWalkTarget(Player *ply) {		/* 2b25c */
	if(g.ThrowEndHoldoff) {
		set_ply_directions(ply);
	}
	if(ply->Opponent->EnemyDirection != FACING_LEFT) {
		ply->AIWalkTarget = ply->AIJumpSel + ply->Opponent->Size + ply->Opponent->XPI;
	} else {
		ply->AIWalkTarget = ply->AIJumpSel - ply->Opponent->Size + ply->Opponent->XPI;
	}
	
	if (ply->XPI < ply->AIWalkTarget) {
		return FACING_RIGHT;			/* which direction to walk */
	} else {
		return FACING_LEFT;
	}
}

// STRAT_LONGWALK
static void _AIStratLongWalk(Player *ply) { // 2b19a AIStrategy approach / retreat
	int temp,temp2;
	switch (ply->AIMode2) {
		case 0:
			debug_print(4, "%02x %02x\n", ply->AIParam1, ply->AIParam2);

			NEXT(ply->AIMode2);
			ply->CompDoJump  = FALSE;
			ply->CompDoBlock = FALSE;
			if (ply->AIParam2) {
				if (ply->AIParam1 & AIB_WALKAWAY) {
					temp = -ply->AIParam2;
				} else {
					temp = ply->AIParam2;
				}
			} else {
				temp = ply->AIParam1;
			
				if (temp & 0x40) {
					temp = data_2b2b4[ply->AIParam1 & 0x3f][RAND32];	// char
					if (ply->AIParam1 & AIB_WALKAWAY) {
						temp |= !AIB_WALKAWAY;
					}
				}
				//2b1fa
				temp2 = temp;
				temp=data_2b294[temp & 0x7f];
				if (temp2 & 0x80) {
					temp = -temp;
				}
			}
			ply->AIJumpSel       = temp;			//2b210 
			ply->AIWalkDirection = _AISetWalkTarget(ply);
			/* FALLTHRU */
		case 2:
			// 2b21c   
			ply->AIWalkDirection = _AISetWalkTarget(ply);
			if(_AIHasReachedWalkTarget(ply)) {
				_AIFinish(ply);
			} else if (g.GPCollDetect && ply->PushDirection != ply->AIWalkDirection) {
				_AIFinish(ply);
			} else {
				if (ply->BoundCheck==0) {
					if (ply->PlatformFallDir == 0) {
						if (_AIShouldGoAggressive(ply)) {
							_AINewGoAggressive(ply);
						}
						return;
					} else {
						temp = ply->PlatformFallDir-1;
					}
				} else {
					temp = ply->BoundCheck - 1;
				}
				if (temp == ply->AIWalkDirection) {
					_AIFinish(ply);
				} else {
					if (_AIShouldGoAggressive(ply)) {
						_AINewGoAggressive(ply);
					}
				}
			}
			break;
		FATALDEFAULT;
	}
}

// STRAT_SETBLOCK	0x2b4a8
static void _AIStratSetBlock(Player *ply) {
	if (ply->AIParam1) {
		ply->CompDoBlock = FALSE;
	} else {
		ply->CompDoBlock = TRUE;
	}
	_AIFinish(ply);
}

//STRAT_JUMP	0x2b3b4
static void _AIStratJump(Player *ply) {
	short temp5;
	
	switch (ply->AIMode2) {
		case 0:
			NEXT(ply->AIMode2);
			ply->CompDoJump  = TRUE;
			ply->CompDoBlock = FALSE;
			ply->AISigSpecial = ply->CompDoAirThrow = ply->CompDoThrow = ply->AICanJumpAttack = 0;
			ply->AIJumpSel = ply->AIParam1;
			ply->x022a = ply->AIParam2;
			ply->x0228 = ply->AIParam3;
			ply->x0238 = ply->AIParam4;
			if (ply->XPI > 0 && ply->XPI <= STAGE_LENGTH) {
				ply->IsWithinBounds = TRUE;
			} else {
				ply->IsWithinBounds = FALSE;
			}
			temp5 = ply->AIJumpSel;
			ply->AIJumpSel &= 0xf;
			if ((temp5 & 0xf0) == 0) {
				return;
			}
			if (ply->x022a & 0x10) {		/* should we attack during the jump? */
				if (comp_ply_difficulty_lookup(ply, data_97522)==0) {
					return;
				}
			}
			if (ply->x022a & 0x80) {
				ply->CompDoThrow = TRUE;
			}
			ply->AICanJumpAttack = TRUE;
			ply->StandSquat		 = PLY_JUMP;
			ply->ButtonStrength  = ply->x022a & 0xf;
			
			if (ply->AIParam1 & AIB_THROW) {
				ply->CompDoAirThrow = TRUE;
			}
			if (ply->AIParam1 & 0x20) {
				ply->PunchKick = PLY_KICKING;
			} else {
				ply->PunchKick = PLY_PUNCHING;
			}
			break;
		case 2:			//2b48c
			if (ply->Airborne != AIR_ONGROUND) {
				NEXT(ply->AIMode2);
			}
			break;
		case 4:
			if (ply->Airborne == AIR_ONGROUND) {
				ply->CompDoJump = FALSE;
				_AIFinish(ply);
			}
			break;
		FATALDEFAULT;
	}
}

static void _AIBeginStrategy(Player *ply) {		// 2ae32 was _AISwitch204
	switch (ply->AIStrategy) {
		case STRAT_STANDSTILL:			_AIStratStandStill(ply);			break;
		case STRAT_SHORTWALK:			_AIStratShortWalk(ply);				break;
		case STRAT_LONGWALK:			_AIStratLongWalk(ply);				break;
		case STRAT_SETBLOCK:			_AIStratSetBlock(ply);				break;
		case STRAT_JUMP:				_AIStratJump(ply);					break;
		case STRAT_ATTACK:				_AIStratAttack(ply);				break;
		case STRAT_KICK:				_AIStratKick(ply);					break;
		case 14:						_AIStrat14(ply);					break;
		FATALDEFAULT;
	}
}

//2b918
static void _AINextStrategyDef(Player *ply) {		/* checked */
	const unsigned char *Strategies;		// %a0
	short Index;					// %d0
	
	Strategies = ply->AIStratDef;
	Index = ply->AIStratIndexDef++;
	debug_print(4, "side %d Index %d\n", ply->Side, Index);
	if(Strategies[Index] & 0x80) {
		if (Strategies[Index] == 0x80) {
			ply->AISaveState = -1;			/* cause AIBeginDef to restore the state */
		} else {
			_AIDefHigh(ply, Strategies[Index]);
		}
	} else {
		_AISetStrategy(ply, Strategies[Index]);
	}	
}


static void _AICheckUpdate(Player *ply) {	/* 2b8b4 checked */
	if(ply->AIForceDefensive) {
		// 2b910 
		ply->AISaveState = 0;
		ply->TCollDis = 0;
		_AINextStrategyDef(ply);
	} else {
		ply->AISaveState = ply->TCollDis = 0;
		if (ply->AIStartAgain != ply->RoughTimeRemain) {
			ply->AIStartAgain = FALSE;
			AIInitPlayer(ply);
		}
		_AIGotoNextStrategy(ply);
	}
}

#pragma mark ---- AI PARSER ----


void AIInitDefensive(Player *ply) {				// 2b82a
	const struct defense_strategy *DS;
	
#ifdef DEBUG_AI
	if (ply->YokeSaved < 0 || ply->YokeSaved >= 0x10) {
		panic(0x27a);
	}
#endif
	DS = dataAIDefensive[ply->FighterID][(unsigned char)ply->YokeSaved];	/* * to vector list to struct */
	/* range of YokeSaved is 0-15 */

	if (0xff07 & (1 << ply->YokeSaved) && ply->Energy <= data_2b8a4[RAND16] ) {		
		ply->AIStratDef = DS->codes[DS->low_energy[RAND32]];
	} else {
		ply->AIStratDef = DS->codes[DS->high_energy[RAND32]];	
	}
		
	ply->AITypeDef = ply->AIStratDef[0];
	ply->AIStratIndexDef = 1;
}


/* fetch the next character from the script, not only strategy IDs but also 
 parameters */
static char _AIReadByte(Player *ply) {		// 2bf02

	if(ply->AIForceDefensive) {
		return ply->AIStratDef[ply->AIStratIndexDef++];
	} else if (ply->AIAgressive) {
		return ply->AIStratAgg1[ply->AIStratIndexAgg1++];
	} else {
		return ply->AIStratAgg0[ply->AIStratIndexAgg0++];
	}
}

static void _AILoadStrategyParams(Player *ply, unsigned char arg_d0) {	// 2b93e
	debug_print(4, "side %d arg %d\n", ply->Side, arg_d0);
	
	if(arg_d0 == STRAT_STANDSTILL) {
		/* 2b95a */
		ply->AIParam1 = _AIReadByte(ply);
		if (ply->AIParam1 == 0x81 || ply->AIParam1 == 0xc0 ) {
			ply->AIParam2 = _AIReadByte(ply);
		} 	
	} else {
		switch (arg_d0) {
			case 0:		/* can't happen */ panic(0); break;
			case STRAT_SHORTWALK:
			case STRAT_SETBLOCK:
			case STRAT_STUN:
				/* 2b97e */
				ply->AIParam1 = _AIReadByte(ply);
				break;
			case STRAT_LONGWALK:
			case STRAT_KICK:
				/* 2b988 */
				ply->AIParam1 = _AIReadByte(ply);
				ply->AIParam2 = _AIReadByte(ply);
				break;
			case STRAT_JUMP:
				/* 2b9b4 */
				ply->AIParam1 = _AIReadByte(ply);
				ply->AIParam2 = _AIReadByte(ply);
				ply->AIParam3 = _AIReadByte(ply);
				ply->AIParam4 = _AIReadByte(ply);				
				break;
			case STRAT_ATTACK:
				/* 2b99a */
				ply->AIParam1 = _AIReadByte(ply);
				ply->AIParam2 = _AIReadByte(ply);
				ply->AIParam3 = _AIReadByte(ply);
				break;
			default:
				printf("AI Error: Bad token %02x\n", arg_d0);
				panic(0);
		}
	}
}

static void _AISetStrategy(Player *ply, char arg_d0) {		// 2b93a
#if FISTBLUE_DEBUG_AI >= 2
	static const char *strats[]={
		"STANDSTILL",
		"SHORTWALK",
		"LONGWALK",
		"SETBLOCK",
		"JUMP",
		"ATTACK",
		"KICK",
		"STUN",
	};
	
	debug_print (2, "side %d ST 0x%x %s\n",ply->Side, arg_d0, strats[arg_d0/2]);
#endif	
	
	if (ply->AIMode2 != 0) {
		printf("ARGH mode2 is nonzero!\n");
	}
	
	ply->AIStrategy = arg_d0;
	_AILoadStrategyParams(ply, arg_d0);
}
static void _AIGotoNextStrategy(Player *ply) {		/* 2b8da was comp_main_exit */
	/* renamed from comp_ main_ exit */
	const unsigned char *Strategies;
	short Index;
	
	if(ply->AIForceDefensive) {
		_AINextStrategyDef(ply);
	} else {
		/* 2b8e2 */
		if (ply->AIAgressive) {
			Strategies = ply->AIStratAgg1;
			Index = ply->AIStratIndexAgg1++;
		} else {
			Strategies = ply->AIStratAgg0;
			Index = ply->AIStratIndexAgg0++;
		}

		if (Strategies[Index] & 0x80) {
			_AIDoStrategy(ply, Strategies[Index]);
		} else {
			debug_print(3, "side %d @0x%08x[%d]\n",ply->Side,
				   (unsigned int)Strategies, Index);
			_AISetStrategy(ply, Strategies[Index]);
		}
	}
}
static void _AISetAttackParams(Player *ply) {					//2b510
	ply->PunchKick    = 
	ply->CompDoThrow  = 
	ply->AIVolley     = 
	ply->AISigSpecial = 0;
	
	if (ply->AIParam1 & AIB_THROW) {	
		ply->CompDoThrow    = TRUE;
		ply->ButtonStrength = ply->AIParam1 & (AIB_THROW - 1);
	} else {		
		if (ply->AIParam1 < AIB_SHIFT) {				// Ordinary punch attack
			ply->ButtonStrength = ply->AIParam1;			
		} else if (ply->AIParam1 >= AIB_POWERMOVE) {
			ply->PunchKick      = ply->AIParam1 & 0xf;
			ply->ButtonStrength = ply->AIParam2;
			ply->AIMultiCount   = ply->AIParam3;
			ply->AISigSpecial   = 2;	
		} else {
			ply->ButtonStrength = ply->AIParam1 & 0xf;
			ply->DiceRollCount  = ply->AIParam2 - 1;
			ply->AIVolley = TRUE;
		}
	}
}
static void _AISetScriptEnd(Player *ply) {			/* 2b932 */
	ply->AISaveState = -1;
}
static void _AISearchA8IfOppXLessEqual(Player *ply) {				//2bc2e
	DEBUG_AI_STR(4, "AISearchA8IfOppXLessEqual: ");
	if(ply->OppXDist <= (_AIReadByte(ply) & 0xff)) {
		_AISearchStrategy(ply, AIB_LABEL_A8);
	}
}
// 2bc4e
static void _AISearchACIfOppJump(Player *ply) {
	short temp;
	temp = _AIReadByte(ply);
	if (ply->Opponent->Airborne && ply->Opponent->YPI <= ply->Opponent->OldOldY.part.integer) {
		if (ply->OppYDist > ply->Opponent->OldOldY.part.integer) {
			_AIGotoNextStrategy(ply);
			return;
		}
	}
	_AISearchStrategy(ply, AIB_LABEL_AC);
}
// 2bc8a
static void _AIB_WITHIN(Player *ply) {
	int found,distance,selector;
	selector = _AIReadByte(ply);
	distance = _AIReadByte(ply);
	debug_print(4, "_AIB_WITHIN: %d, %d\n", selector, distance);
	
	switch (selector) {
		case 0:
			/* 2bcac 2a */
			if (_AIClosestThreat(ply) > distance) {
				found = TRUE;
			} else {
				found = FALSE;
			}
			break;
		case 2:				/* 2bd12 */
			found = opponent_within(ply, distance);
			break;
		case 4:				/* 2bd1c */
			if (ply->Opponent->Airborne == AIR_ONGROUND ||
				ply->Opponent->YPI <= ply->Opponent->OldOldY.part.integer) {
				found = FALSE;
			} else if (ply->OppYDist > ply->Opponent->OldOldY.part.integer) {
				found = TRUE;
			} else {
				found = FALSE;
			}
			break;
		case 6:
			/* 2bd3a */
			if(ply->Opponent->Airborne == AIR_ONGROUND ||
			   ply->Opponent->YPI > ply->Opponent->OldOldY.part.integer) {
				found = FALSE;
			} else if (ply->OppYDist > ply->Opponent->OldOldY.part.integer) {
				found = TRUE;
			} else {
				found = FALSE;
			}
			break;
			
		FATALDEFAULT;
	}
	if (found == FALSE) {
		_AISearchStrategy(ply, AIB_BB2);
	}
}

/* 2ba20 process strategy with high bit set, one caller */
static void _AIDefHigh(Player *ply, short d0){
	int temp;
	switch (d0 & 0x7f) {
		case 0:		case 2:		case 4:		case 6:				/* 80,82,84,86 */
		case 0xa:	case 0xc:	case 0xe:	
		case 0x18:	case 0x1a:			/* 8a,8c,96,98 */
		case 0x1c:												/* 9a */
			_AISetScriptEnd(ply);		// not actually script end, just tags, set AISaveState=-1
			debug_print(4, "LABEL %x\n", d0);
			break;
		case 8:			// 2baf4, 0x88	Disable Collisions
			ply->TCollDis = 1;
			_AIGotoNextStrategy(ply);
			break;
		case 0x10:		// 2bb24	0x8e Wallbounce
			temp = _AIReadByte(ply);
			if(temp == 8) {
				ply->AIWallBounce = TRUE;
				ply->AIStrategy   = temp;
				_AILoadStrategyParams(ply, temp);
			} else {
				if (temp < 0) {
					_AIDoStrategy(ply, temp);
				} else {
					_AISearch94NotDizzy(ply);
					_AIGotoNextStrategy(ply);		// refactored out of 2bb40
				}
			}					
			break;
		case 0x12:			// 0x90 search94notdizzy
			_AISearch94NotDizzy(ply);
			_AIGotoNextStrategy(ply);
			break;
			
		case 0x16:			
			ply->x0216 = TRUE;
			_AIGotoNextStrategy(ply);
			break;
		case 0x1e:			/* 2bbd8 */
			_AISearchA0DistLessOrEqual(ply);
			_AIGotoNextStrategy(ply);
			break;
			
		
		case 0x14:		case 0x20:		case 0x24:		case 0x28:
		case 0x2c:		case 0x32:
			_AIGotoNextStrategy(ply);
			break;
			
		case 0x22:		/* a0 */
			_AISearchA4IfOppJump(ply);
			_AIGotoNextStrategy(ply);
			break;
		case 0x26:		/* a4 searchtaga4 */
			_AISearchA8IfOppXLessEqual(ply);
			_AIGotoNextStrategy(ply);
			break;
		case 0x2a:		/* a8 searchtaga8 */
			_AISearchACIfOppJump(ply);
			_AIGotoNextStrategy(ply);
			break;
		case 0x2e:		/* ac searchtagac */
			_AIB_WITHIN(ply);
			_AIGotoNextStrategy(ply);
			break;
		case 0x30:			//2bd5a  ae
			_AISearchStrategy(ply, AIB_LABEL_B2);
			_AIGotoNextStrategy(ply);
			break;
		case 0x34:			//2bd6a	b2  Immune (a)
			ply->CompImmune = _AIReadByte(ply);
			_AIGotoNextStrategy(ply);
			break;
		FATALDEFAULT;
	}
}

// ForceDefensive High-Bit
static void _AIStrategyHighBit(Player *ply, u8 d0) {	//2b92a
	if(d0 != 0x80) {
		_AIDefHigh(ply, d0);
	} else {
		/* end of list */
		_AISetScriptEnd(ply);
	}
}

#pragma mark ---- AIExits ----
/* all callers should _AIGotoNextStrategy afterward */

static void _AIExit1(Player *ply) {	/* 2b8d2 */
	ply->x0216 = FALSE;			/* u8 */
	AIInitPlayer(ply);
}

static void _AIRestartAggressive(Player *ply) {	/* 2ba62 _AIExit2 */
	if (ply->AIAgressive) {
		ply->AIStratIndexAgg1 = 1;
	} else {
		ply->AIStratIndexAgg0 = 1;
	}	
}
static void _AIExit3(Player *ply) {		/* 2ba7c */
	if (ply->AIAgressive != 0) {
		_AIExit1(ply);				/* re-init */
	} else {
	debug_print(2, "side %d going aggressive\n",ply->Side);

	ply->AIAggTimer0 = ply->AIAllowAggressive = ply->AIAggTimer1 = 0;
	ply->AIAgressive      = 2;
	ply->AIMode1          = 0;
	ply->AIMode2          = 0;
	ply->AISaveState      = 1;			
	ply->AIStratIndexAgg1 = 1;		// Rewind to beginning
	}
}
static void _AIExit4(Player *ply) {	/* 2baae */
	if(ply->AIAgressive == 0) {
		_AIRestartAggressive(ply);
	} else {
		ply->AIAggTimer0 = ply->AIAllowAggressive = ply->AIAggTimer1 = 0;
		ply->AIAgressive = 0;
		ply->AIMode1 = ply->AIMode2 = 0;
		ply->AISaveState = 1;
		if(ply->AIStartAgain) { ply->AIStratIndexAgg0 = 1; }
		ply->AIStartAgain = FALSE;
	}
}	
static void _AIExit5(Player *ply) {	/* 2bb14 */
	ply->AITimerThreatCheck = 0;
	ply->x0276 = _AIReadByte(ply);		// unused
	debug_print(2, "0x%02x\n",ply->x0276);
}

#pragma mark ========


static void _AIDoStrategy(Player *ply,  short d0) {			//2b9d6 high bit strategy
	if (ply->AIForceDefensive) {
		_AIStrategyHighBit(ply, d0);
	} else {
		AIAggHigh(ply, d0 & 0x7f);
	}
}

void AIPanic(const unsigned char *a2, short d2) {
	int i;
	printf("AIPanic 0x%08x %d\n", (unsigned int)a2, d2);
	for (i = 0; i<d2; i++) {
		printf("%02x ", a2[i]);
	}
	printf("\n===>\n");
	for (i = d2; i<=(d2 + 8); i++) {
		printf("%02x ", a2[i]);
	}
	printf("\n");
	panic(0);
}
	
static short _AISkipOverParams(Player *ply, u8 d0, const unsigned char *a2, short *d2) {
	/* 2bdce return value actually ignored, seems void, but we'll keep it for now */
	
	if (ply->AIForceDefensive && (d0 == 0x80)) {
		return g.libsplatter;
	} else if (d0 & 0x80) {
		switch (d0 & 0x7f) {
			case 0:		case 2:		case 4:		case 6:
				return g.libsplatter;	
			case 8:		case 0x10:	case 0x12:	case 0x14:	
			case 0x16:	case 0x18:	case 0x1a:	case 0x1c:
			case 0x20:	case 0x24:	case 0x28:	case 0x2c:
			case 0x30:	case 0x32:
				return d0 & 0x7f;
				break;
			case 0xa:	case 0xc:	case 0xe:	case 0x1e:
			case 0x22:	case 0x26:	case 0x2a:	case 0x2e:
			case 0x34:
				*d2+=1;
				return a2[*d2];
				break;
			FATALDEFAULT;
		}
	} else if (d0 == 0) {
		/* 2bdfe */
		d0 = a2[(*d2)++];
		
		if (d0 == 0x81 || d0 == 0xc0) {
			return a2[(*d2)++];	
		} else {
			return d0;
		}
	} else {
		/* skip over params */
		switch (d0) {
			case 0:
			case 2:
			case 6:
			case 0xe:
				/* 2be1a */
				*d2 += 1;
				break;
			case 4:
			case 0xc:
				/* 2be1e */
				*d2 += 2;
				break;
			case 8: /* 2be32 */
				*d2 += 4;
				break;
			case 0xa:
				/* 2be26 */
				*d2 += 3;
				break;
			default:
				AIPanic(a2, *d2);
		}
	}
	return a2[*d2-1];
}
static void _AISearchStrategy (Player *ply, unsigned char d3){	//2bda0, 2bd76
	const u8 *code;
	short index;
	
	// Select the right code pointer and index value
	if(ply->AIForceDefensive) {	/* 2bd76 */
		code = ply->AIStratDef;
		index = ply->AIStratIndexDef; 
	} else if (ply->AIAgressive) {
		code = ply->AIStratAgg1;
		index = ply->AIStratIndexAgg1;
	} else {
		code = ply->AIStratAgg0;
		index = ply->AIStratIndexAgg0;
	}
	
	while ( code[index++] != d3 ) {
		_AISkipOverParams(ply, code[index-1], code, &index);		// skip params
	}
	
	/* 2bdb0 write index value back */
	if(ply->AIForceDefensive) {
		ply->AIStratIndexDef = index;
	} else if (ply->AIAgressive) {
		ply->AIStratIndexAgg1 = index;
	} else {
		ply->AIStratIndexAgg0 = index;
	}	
}
//2bb40
static void _AISearch94NotDizzy(Player *ply) {
	if (ply->Opponent->DizzyStun == FALSE && ply->Opponent->DizzyFall == FALSE) {
		_AISearchStrategy(ply, 0x80 + 0x14);
	}
	// always returns to _AIGotoNextStrategy in caller
}	
// 2bbd8 checked 
static void _AISearchA0DistLessOrEqual(Player *ply) {
	short dist = _AIReadByte(ply);
	debug_print(2, "side %d distance 0x%x \n", ply->Side, dist);
	if (ply->OppXDist >= dist) {
		_AISearchStrategy(ply, 0xa0);
	}
	// always return to _AIGotoNextStrategy in caller
}

/*
 static short sub_2bf42(char *a2, short *d2) {		// inlined 
 return a2[*d2++] & 0xff;
 }
 */

static short sub_2bcbc(Player *ply) {		/* 2bcbc */
	if (ply->AIForceDefensive == FALSE) {
		return 1;
	} else {
		if (0x00002007 && 1 << ply->YokeSaved) {
			return 0;
		}
	}
	return 1;
}
static short opponent_within(Player *ply, short d0) {
	if (ply->OppXDist > d0) {
		return TRUE;
	}
	return FALSE;
}
static short _AIClosestThreat(Player *ply) {		// 2bcd8 closest threat
	short d1,d2;
	
	if (sub_2bcbc(ply)) {		// too stupid to notice fireballs?
		return ply->OppXDist;
	} else {
		/* inlined 2bcf0 */
		if(ply->Opponent->Projectile) {
			d1 = ABS(ply->Opponent->Projectile->YPI - ply->YPI);
			d2 = 0;
		} else {
			d2 = -1;
		}
		if (d2<0) {
			return ply->OppXDist;
		}
		if(ply->OppXDist < d1) {
			return ply->OppXDist;
		}
		return d1;
	}
}
static void _AIGoToAgg1(Player *ply) {			//2bb84
	struct dualptr DP;
	ply->AIAggTimer0 = ply->AIAllowAggressive = ply->AIAggTimer1 = 0;
	ply->AIAgressive = 2;
	_AIResetState(ply);
	ply->AISaveState = 1;
	DP=_AILookupStrategy(ply);		
	_AISetAgg1(ply, DP.a2);
	// return to caller, who carries on to _AIGotoNextStrategy
}		

// 2bbfa
static void _AISearchA4IfOppJump(Player *ply) {
    short temp;
	temp = _AIReadByte(ply) & 0xff;
	if(ply->Opponent->Airborne && ply->Opponent->YPI >= ply->Opponent->OldOldY.part.integer) {
		if (ply->OppYDist > temp) {		/* u16 */
			return;
		}
	}
	_AISearchStrategy(ply, 0xa4);

}	

static void AIAggHigh(Player *ply, short d0) {
	/* big switch at 2b9ea 
	 AIStrat & 0x80 = TRUE,
	 AIAgressive == 0,
	 AIForceDefensive == 0,
	 */
	int temp;
	
	switch (d0) {
		case 0:				/* AIB_RESTART */
			_AIRestartAggressive(ply); _AIGotoNextStrategy(ply); break;
		case 2:
			_AIExit3(ply); _AIGotoNextStrategy(ply); break;
		case 4:				/* 2baae */
			_AIExit4(ply); _AIGotoNextStrategy(ply); break;
		case 6:				/* 2bae6 */
			if(comp_ply_difficulty_lookup(ply, data_97522)) {
				_AIExit3(ply);
			} else {
				_AIExit4(ply);
			}
			_AIGotoNextStrategy(ply);
			break;
		case 8:			/* 2baf4 */
			ply->TCollDis = TRUE;
			_AIGotoNextStrategy(ply);
			break;
			
		case 0xa:			/* 2bafe */
			ply->AIThreatCheckMode = 1; _AIExit5(ply); _AIGotoNextStrategy(ply); break;
		case 0xc:			/* 2bb06 Enable threat assessment */
			ply->AIThreatCheckMode = 0; _AIExit5(ply); _AIGotoNextStrategy(ply); break;
		case 0xe:
			ply->AIThreatCheckMode = 2; _AIExit5(ply); _AIGotoNextStrategy(ply); break;
			
		case 0x10:
			temp = _AIReadByte(ply);
			if(temp == 8) {
				ply->AIWallBounce = TRUE;
				ply->AIStrategy = temp;
				_AILoadStrategyParams(ply, temp);	// does not _AIGotoNextStrategy()
			} else {
				if (temp & 0x80) {
					_AIDoStrategy(ply, temp);		// recursion
				} else {
					_AISearch94NotDizzy(ply);
					_AIGotoNextStrategy(ply);
				}
			}		
			break;
		case 0x12:
			_AISearch94NotDizzy(ply); _AIGotoNextStrategy(ply); break;
		case 0x16:
			ply->x0216 = TRUE;		  _AIGotoNextStrategy(ply); break;
		case 0x18:
			if(ply->AIAgressive==0) {
				if(comp_ply_difficulty_lookup(ply, data_97522)) {
					_AIGoToAgg1(ply);
				}
			}
			_AIGotoNextStrategy(ply);
			break;
		case 0x1a:
			if(ply->AIAgressive==0) {
				ply->AIStartAgain = TRUE;
				_AIGoToAgg1(ply);
			}
			_AIGotoNextStrategy(ply);
			break;
		case 0x1c:
			if (ply->AIAgressive==0) {				/* 2bbc0 */
				if (comp_ply_difficulty_lookup(ply, data_97522)) {
					ply->AIStartAgain = TRUE;
				}
			}
			_AIGotoNextStrategy(ply);
			break;
		case 0x1e:
			_AISearchA0DistLessOrEqual(ply); _AIGotoNextStrategy(ply); break;
		case 0x22:
			_AISearchA4IfOppJump(ply);       _AIGotoNextStrategy(ply); break;
		case 0x26:
			_AISearchA8IfOppXLessEqual(ply); _AIGotoNextStrategy(ply); break;

		case 0x14:
		case 0x20:
		case 0x24:
		case 0x28:
		case 0x2c:
		case 0x32:
			_AIGotoNextStrategy(ply);
			break;
			
		case 0x2a:	
			_AISearchACIfOppJump(ply);      _AIGotoNextStrategy(ply); break;
		case 0x2e:
			_AIB_WITHIN(ply);   _AIGotoNextStrategy(ply); break;
		case 0x30:
			_AISearchStrategy(ply, AIB_LABEL_B2);
			_AIGotoNextStrategy(ply);
			break;
		case 0x34:
			ply->CompImmune = _AIReadByte(ply);
			_AIGotoNextStrategy(ply);
			break;
		FATALDEFAULT;
	}
}


