/*
 *  actions.h
 *  GLUTBasics
 *
 *  Created by Ben Torkington on 21/04/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

/* actions.c */
#include "actions_198a.h"

void proc_actions(void);

void action_start_35(Player *ply);
void ActStartVomit(Player *ply);
void action_1e336(Player *ply);
void action_start_21(void);
void action_start_22(void);

void action_1286e(Object *obj, short d0);
void action_207f0(Object *obj);


void action_15c2e(void);

void PrintPlayerPic(Player *ply, short side, short fighterid);


void action_1ab8a(void);
void action_b8aa(void);
void action_1606c(void);
void ActBlankaBiteBlood(Player *ply);

void action_132fe(Object_G2 *obj_a2, int arg_d6);       // Car collision

void ActStartScreenWobble(void);
void StartDizzyAnim(Player *ply);
void action_1cd3c(Player *ply);
void ActStartGroundDust(Player *ply);


void draw_portraits_prefight(void);		/* 15f9e */
void draw_portraits_postfight(void);
void action_print_chant(void);

void sub_221ea(Object_G2 *obj_a2);
void sub_221bc(Object_G2 *obj_a2);


#define ACT117C(id,function)    \
	case id:					\
		function(obj);			\
		break;					\


// may need to use  __attribute__((packed))



struct UserData_Act09 {
	short	h0080s;
	char	h0082c;
	char	h0083c;
};

struct UserData_Act11 {
	u32	h0080l;
	u32 h0084l;
};
struct UserData_Act12 {
	char h0080c;
};

struct UserData_Act0B {
	short	h0080s;
	short	h0082s;
};

struct UserData_Act0E {
	short	h0080s;	
};

struct UserData_Act1d {
	char	h0080c;	//BattleLoser
	char	h0084c;
	Player *x0088;
};
	
struct UserData_Act2f {
	short	OldY;
	FIXED16_16	VelX, VelY, Gravity;
};

struct UserData_Act32 {
    const short *x0080;
};

struct UserData_Act35 {
	short	x0080;	
	short	x0082;
	const char    *x0084;
};

struct UserData_Act36 {		/* ScreenWobble */
	short Save_Scroll1Y;
	short Save_Scroll2Y;
	short Save_Scroll3Y;
};

struct UserData_Act3b {
    u16 x0080;
    u16 x0082;
};
struct UserData_Act44 {
	char x0080;
	//char x0081;
};

struct UserData_Act48 {
	char x0080;
	char x0081;
};

