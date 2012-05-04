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

void fistblue_run_tests(void) {
	int i;
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
