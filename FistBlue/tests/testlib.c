/*
 *  testlib.c
 *  MT2
 *
 *  Created by Ben on 2/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */

#include "testlib.h"
#include "sf2const.h"
#include "sf2types.h"
#include "sf2macros.h"
#include "gemu.h"
#include "sf2io.h"
#include "structs.h"
#include "effects.h"

#include	"lib.h"
#include	"gfxlib.h"

//extern CPSGFXEMU gemu;

extern struct effectstate es;
//extern Game g;

void test_dimscreenout(void) {		// 84ccc
	es.FadeBusy = TRUE;
	QueueEffect(0x0c00, 2);
	while (es.FadeBusy) {
		sf2sleep(1);
	}
}
void test_fadescreenin(void) {
	es.FadeBusy = TRUE;
	QueueEffect(0x0c10, 2);
	while (es.FadeBusy) {
		sf2sleep(1);
	}
}
