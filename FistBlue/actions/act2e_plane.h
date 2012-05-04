/*
 *  act2e_plane.h
 *  MT2
 *
 *  Created by Ben on 5/05/12.
 *  Copyright 2012 Ben Torkington. All rights reserved.
 *
 */

#ifndef _SF2ACT2E
#define _SF2ACT2E



void synth_plane_setup(Object *obj, int city_from, int city_to);
void action_2e(Object *obj);

struct UserData_Act2e {
	u8	city_from;
	u8	city_to;
	
	POINT16 destination;
	//
	short	sound;	// 0096
};


#endif