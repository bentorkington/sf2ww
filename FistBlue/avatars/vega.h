/*
 *  vega.h
 *  GLUTBasics
 *
 *  Created by Ben on 13/02/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

struct vegathrow {
	char	move;
	char	success;
};

struct UserData_Vega {
	u8	x0080;
	
	char	x0090;
	short	x0092;
	short	x0094;
	char	x0096;
} __attribute__((packed));

void PLCBCompAttackVega(Player *ply);
