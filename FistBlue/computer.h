/*
 *  computer.h
 *  GLUTBasics
 *
 *  Created by Ben on 15/10/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */

void comp_set_jump(Player *ply);				/* 2ceda */

void comp_recovery_complete(Player *ply);		/* 2b700 */
void comp_exit_plstat_crouch(Player *ply);		/* 0x2c496 */
void comp_exit_land(Player *ply);
void exit_to_compdisp1(Player *ply);			/* 2c464 */
void exit_comp_plstatfe_normal(Player *ply); 	/* 2c436 */
void exit_comp_normal(Player *ply);				/* 0x2c408 */


/* Main Entry */
void computer_per_frame(Player *ply);

void set_comp_difficulty(Player *ply);
void sub_2c516(Player *ply);

void comp_plstat_normal(Player *ply);  /* 2c2a4 */

short comp_diceroll(Player *ply);   	// 2c34a

void comp_setjumping_main(Player *ply);	/* 2c534 */

short comp_setnextaction(Player *ply);		/* 2c5de */
void comp_do_next_action(Player *ply);		/* 2c668 */


#define COMP_CHK_TACTICS if(temp=comp_check_newtactics(ply)){comp_changetactics(ply, temp);return;}
#define COMP_CHK_BLOCKSTUN if(check_compDoBlockStun(ply))
#define COMP_CHK_27BZ if(check_ply_27b(ply)==0)

struct data_2b7ea {
	const char	x023e[32];
	const unsigned char *x0134[16];	 
};

typedef const struct data_2b7ea AIAggTable;



struct dualptr {
	const u8	*a1;
	const AIAggTable  *a2;
};

struct defense_strategy {
	char		high_energy[32];
	char		low_energy[32];		/* selector to use if energy is very low */
	const u8	*codes[];		
};



