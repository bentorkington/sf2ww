/*
 *  actions.h
 *  GLUTBasics
 *
 *  Created by Ben Torkington on 21/04/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

/* actions.c */
void proc_actions(void);

void action_start_35(Player *ply);
void ActStartVomit(Player *ply);
void action_1e336(Player *ply);
void action_start_21(void);
void action_start_22(void);

void action_1286e(Object *obj, short d0);
void action_207f0(Object *obj);


void action_15c2e();

void PrintPlayerPic(Player *ply, short side, short fighterid);


void action_1ab8a();
void action_b8aa();
void action_1606c();
void ActBlankaBiteBlood(Player *ply);


void ActStartScreenWobble(void);
void StartDizzyAnim(Player *ply);
void action_1cd3c(Player *ply);
void ActStartGroundDust(Player *ply);


void draw_portraits_prefight(void);		/* 15f9e */
void draw_portraits_postfight(void);
void action_print_chant(void);




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
	char	x0080;	//BattleLoser
	char	x0084;
	Player *x0088;
};


	
struct UserData_Act2f {
	short	OldY;
	FIXED16_16	VelX, VelY, Gravity;
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
	DUAL x0080;
	u16 x0082;
	
};

struct UserData_Act48 {
	char x0080;
	char x0081;
};

