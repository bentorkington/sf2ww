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
#include "fbtests.h"

#include "lib.h"
#include "pthreads.h"
#include "demo.h"
#include "task.h"

struct game g;
//struct player_t Player1;
//struct player_t Player2;


void manual_init(void) {
	//fistblue_run_tests();
	memclear(&g, sizeof(struct game));
	LBResetState();
	g.CurrentDifficulty = 0x7;
	
	RHInitThreads();
	
	startup();
	if (0) {
		create_task(task_attractSequence, 4, 0, 0, 0);
	} else {
		startgame(ONLY_P1);
		//g.Player1.Human     = TRUE;
		//g.Player2.Human    = TRUE;
		g.Player1.FighterID = FID_RYU;
		g.Player2.FighterID = FID_GUILE;
		g.CurrentStage = STAGE_USA_GUILE;
		g.randSeed1    = 0x01;       /* initial random seed */
		g.randSeed2    = 0xc2;
	}
}
