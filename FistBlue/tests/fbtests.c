/*
 *  fbtests.c
 *  MT2
 *
 *  Created by Ben on 30/11/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */
#include <stdio.h>
#include <assert.h>

#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"

#include "gstate.h"
#include "structs.h"

#include "fbtests.h"
#include	"lib.h"


extern Game g;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;


void fistblue_run_tests(void) {
	memclear(&g, sizeof(struct game));	
	
	printf("fistblue_run_tests:\n");
	
#ifdef SF2_ENDIAN_LITTLE
	printf("SF2_ENDIAN_LITTLE\n");
#endif
#ifdef SF2_ENDIAN_BIG
	printf("SF2_ENDIAN_BIG\n");
#endif
	
	// test sf2rand();
	printf("output of sf2rand: ");
	g.randSeed1    = 0x01;       /* initial random seed */
	g.randSeed2    = 0xc2;
	assert(sf2rand() == 0xc7);
	assert(sf2rand() == 0xd8);
	assert(sf2rand() == 0x0d);
	assert(sf2rand() == 0xac);
	printf("PASSED \n");
	
	printf("fixed-precision types in types.h: ");
	FIXED8_8 testorder16;
	FIXED16_16 testorder32;

	assert(sizeof(FIXED8_8) == 2);
	assert(sizeof(FIXED16_16) == 4);
	
	testorder16.full= 0x1234;
	testorder32.full= 0x12345678;
	assert(testorder16.part.integer == 0x12);
	assert(testorder16.part.fraction == 0x34);
	assert(testorder32.part.integer == 0x1234);
	assert(testorder32.part.fraction == 0x5678);
	printf("PASSED \n");
	
	
	
	//exit(0);
}

void fistblue_test_gfx(void) {

//	while (TRUE) {
//		SM_game();
//	}
	
	g.CPS.DispEna		= 0x12da;
	g.Palette1			= 0x11;
	palette_macro(0x11);
	GSInitForStage();
	g.CPS.Scroll2X		= 0;
	g.CPS.Scroll2Y		= 0;
	
	gstate_Scroll3.XPI = 0x0;
	gstate_Scroll3.YPI = 0x700;
	GSSetupScr3(&gstate_Scroll3);	// draw the world map background
	
	fadenwait4();
	//start_effect(LC0_LIGHT_ALL_ENABLE, 0x1);
	//test_fadescreenin();
	printf("screen fading up...\n");
	sf2sleep(60);
	printf("showing ports...\n");
	draw_portraits_prefight();
	sf2sleep(60);
	printf("showing ports post...\n");
	draw_portraits_postfight();
	sf2sleep(60);
	exit(0);
}
