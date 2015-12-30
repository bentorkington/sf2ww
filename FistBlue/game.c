/*
 *  game.c
 *  MT2
 *
 *  Created by Ben on 3/09/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

#include "sf2types.h"
#include "sf2const.h"

#include "game.h"
#include "player.h"
#include "structs.h"

#ifdef FISTBLUE_TESTS
#include "fbtests.h"
#endif

#include "lib.h"
#include "pthreads.h"
#include "demo.h"
#include "task.h"
#include "aitests.h"
#include "sm.h"

struct game g;

void manual_init(void) {
#ifdef FISTBLUE_TESTS
	fistblue_run_tests();
    redhammer_run_tests();
#endif
	memclear(&g, sizeof(struct game));
	LBResetState();
	g.CurrentDifficulty = 0x7;
	
	RHInitThreads();
	
	g.CPS.DispEna = 0x12c0;
    startup();
}
