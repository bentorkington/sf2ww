/*
 *  rules.h
 *  GLUTBasics
 *
 *  Created by Ben Torkington on 20/04/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */
void copy_level_table(short d0);
void sub_2b7c(void);
void sub_4720(void);
void proc_round_result(void);
int check_special_ability(Player *ply);
int _check_throw(int airthrow, Player *ply);
int airthrowvalid(Player *ply);
int throwvalid(Player *ply);
void set_initial_positions(void);
void bumpdifficulty(void);
void bumpdifficulty_01(void);
void bumpdifficulty_08(void);
void BumpDiff_NewGame(void);
void bumpdifficulty_08(void);
void bumpdifficulty_02(void);
void bumpdifficulty_03(void);
void bumpdifficulty_04(void);
void bumpdifficulty_05(void);
void bumpdifficulty_06(void);
void bumpdifficulty_10(void);
void bump_difficulty_4816(void);
void BumpDiff_PowerMove(void);	// 46c2
void bumpdifficulty_4576(void);

void set_defeated_true(Player *ply);		
void set_defeated_false(Player *ply);		


void sub_4794(void);
int sub_3fee(Player *ply);
void give_100_points(short side);
short ply_opp_apply_grip_damage(Player *ply, short d2,short subsel_d3, short xoff_d4, short yoff_d5, short sound_d6);			// 3466 player %a6
int ply_opp_has_struggled_free(Player *ply);