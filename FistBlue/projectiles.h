/*
 *  projectiles.h
 *  GLUTBasics
 *
 *  Created by Ben on 16/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */
void process_projectiles(void);			/* 22aca */

enum en_projectiles {
	SF2_PROJ_HADOUKEN = 0,
	SF2_PROJ_YOGAFIRE,
	SF2_PROJ_YOGAFLAME,
	SF2_PROJ_SONICBOOM,
	SF2_PROJ_TIGER,
	SF2_PROJ_X05,
};
typedef enum en_projectiles FBProjectileID;