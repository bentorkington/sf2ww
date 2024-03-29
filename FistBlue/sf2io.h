/*
 *  io.h
 *  MT2
 *
 *  Created by Ben on 4/12/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */


#ifndef _SF2_IO
#define _SF2_IO
// Jumpers

//JPParam
#define JP_DBGWAITMODE	0x01
#define JP_DBGNOCOLLIDE 0x02
#define JP_FREEPLAY		0x04
#define JP_FREEZE		0x08		// live
#define JP_FLIP			0x10		// live
#define JP_DEMOSOUND	0x20		// live
#define JP_ALLOWCONT	0x40
#define JP_DEBUG2		0x80		// live

//JPDifficulty
#define JP_DIFF1		0x01
#define JP_DIFF2		0x02
#define JP_DIFF4		0x04
#define JP_DIFFMASK		(JP_DIFF1|JP_DIFF2|JP_DIFF4)
#define JP_JAPANJUMP	0x08

//JPCost
#define JP_DBGSLEEP		0x01
#define JP_DBGNOTICK	0x08
#define JP_DBGTACTICS	0x10
#define JP_CONTINUECOIN	0x40

#define JP_COSTMASK1	0x07
#define JP_COSTMASK2	0x38


/* CPS IN0 */
#define IPT_COIN1	0x01
#define IPT_COIN2	0x02
#define IPT_SERVICE	0x04
#define IPT_START1	0x10
#define IPT_START2	0x20
#define IPT_SERVICE_NOTOGGLE	0x40


/* other IO */
//ContrP[12].full
#define JOY_UP        0x8
#define JOY_DOWN      0x4
#define JOY_LEFT      0x2
#define JOY_RIGHT     0x1

#define JOY_LRD			(         JOY_DOWN | JOY_LEFT | JOY_RIGHT)		// 0x7
#define JOY_LRU			(JOY_UP |            JOY_LEFT | JOY_RIGHT)		// 0xb
#define JOY_MOVEMASK	(JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT)		// 0xf

#define BUTTON_A	 0x10
#define BUTTON_B	 0x20
#define BUTTON_C	 0x40
#define BUTTON_D	0x100
#define BUTTON_E	0x200
#define BUTTON_F	0x400

#define BUTTON_PUNCHES (BUTTON_A | BUTTON_B | BUTTON_C)
#define BUTTON_KICKS   (BUTTON_D | BUTTON_E | BUTTON_F)
#ifndef BUTTON_MASK
#define BUTTON_MASK	( BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_D | BUTTON_E | BUTTON_F )
#endif

#endif
