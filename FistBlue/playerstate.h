/* playerstate.h */

#ifndef INC_PLAYERSTATE
#define INC_PLAYERSTATE


/* playerstate.c */
void PSEntry(Player *ply);
void human_per_frame(Player *ply);
void proc_plstat_normal(Player *ply);
void proc_plstat_crouch(Player *ply);
void proc_plstat_attacking(Player *ply);
void proc_plstat_powermove(Player *ply);
void RMTumble(Player *ply);
void proc_plstat_taking_hit(Player *ply);
void plstat_do_nextaction(Player *ply);
void proc_plstat_jumping(Player *ply);
void proc_plstat_standingup(Player *ply);
void jump_physics(Player *ply);
void proc_plstat_blockstun(Player *ply);
void sub_28d3c(Player *ply);
void sub_28d78(Player *ply);
void sub_28d7e(Player *ply);
void PSTurnAroundCrouch(Player *ply);
void crouchmove_or_jump(Player *ply);
void standattack_or_jump(Player *ply);
void sub_28e64(Player *ply);
void sub_29280(Player *ply);
void check_dizzy(Player *ply);	/* 292a0 */
void random_dizzyspell(Player *ply);
void PSDizzyState(Player *ply);
int is_facing_enemy(Player *ply);
short check_platform_end(Player *ply);
int is_pushing_up(Player *ply);
int is_holding_down(Player *ply);
void turn_around(Player *ply);
void crouch(Player *ply);
void set_standblock(Player *ply);
void set_crouchblock(Player *ply);
void set_falling_from_platform(Player *ply);
void set_jumping(Player *ply);
void react_to_result(Player *ply);
void downandout(Player *ply);
void ply_exit_stand(Player *ply);
void ply_exit_crouch(Player *ply);
void ply_exit_air(Player *ply);
void player_postamble(Player *ply);
short check_round_result(void);			/* 2cd06 */
void PSStateRoundOver(Player *ply);
void PSPushBack(Player *ply);		/* handle pushback */
void proc_plstat_victory(Player *ply);		/* plstat_victory */
void ApplyPhysicsRules(void);		/* 28340 */
void _RMRecoverTumble(Player *ply) ;     /* 292f4 recovering from a knockover */
void set_attacking(Player *ply);
short PSSetNextAction(Player *ply);		// 288c0
void exit_jumping2(Player *ply);

#define NEXTACT_ROUNDOVER	2
#define NEXTACT_TURNAROUND  0x4
#define NEXTACT_CROUCHDOWN  0x6
#define NEXTACT_JUMP		0x8
#define NEXTACT_STANDUP	    0xa
#define NEXTACT_FALLING     0xc

#endif
