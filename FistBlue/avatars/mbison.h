/*
 *  mbison.h
 *  GLUTBasics
 *
 *  Created by Ben on 27/01/11.
 *  Copyright 2011 Ben Torkington. All rights reserved.
 *
 */

void sub_3623e(Player *ply);		// 3623e cape processing


#define MBM(arg0, arg1, arg2, arg3) \
ud->x0080 = arg0;					\
ud->x0081 = arg1;					\
ud->x0084 = arg2;					\
ud->x0082 = arg3;					\



void pl_cb_setstatus2_mbison(Player *ply, short status, int argd0);
void pl_cb_setstatus3_mbison(Player *ply, short status);
void pl_cb_setstatus1_mbison(Player *ply, short status);


void PLCBCompAttackMBison(Player *ply);
int PLCBCompJumpMBison(Player *ply);

void sub_3623e(Player *ply);

struct UserData_MBison {
	u8		x0080;			//far move
	u8		x0081;			//closemove
	short	x0082;			//distance
	u8		x0084;		
	char	x0085;
	short	x0086;
	short	x0088;
	char	x008a;
	char	x008b;
	short	x008c;
	short	x008e;
	char	x0090;
	u8	x0092;
} __attribute__((packed));

