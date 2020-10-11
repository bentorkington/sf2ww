#ifndef INC_SF2LIB
#define INC_SF2LIB

/* lib.c */
#include "player.h"

void task_playground(void);

void sleep2(void);
void init_fight_vars(void);
void newgame(void);
void debughook(int data);
void panic(int data);
void decode_difficulty(void);
void decode_params(void);
void intproc(void);
void read_jumpers(void);
void startup_impatience(void);
void set_waitmode(void);
void fightstuff(void);
void fighttick(void);
void proc_all_actions(void);
void all_actions_sprites(void);
int check_if_new_player(void);
void sf2_interrupt(void);
void startup(void);
short sf2rand(void);
short LBRareChance(void);
void ClearEffectQueue(void);
int QueueEffect(unsigned short arg1, unsigned short arg2);
void player_hitstuni(Player *ply, unsigned char trajectory, unsigned char direction, unsigned char damage, unsigned char subsel, short xoff, short yoff, unsigned char sound);
void memclear(void *c, int len);
void clear_players(void);
void clear_gsrowscroll(void);
void resetstate_B(void);
void LBResetState(void);
void _LBResetState(void);	// XXX rename me
struct object_g2 *pop_1174(void);
/*!
 @abstract deallocate a Group-2 object
 @param obj the Group-2 object to deallocate
 @discussion sf2ua address: 0x29c2
 */
void clearpush_1174(struct object_g2 *obj);
/*!
 @abstract Allocate a projectile object
 @return a projectile object
 */
Object *AllocProjectile(void);
void FreeProjectile(Object *obj);
Object *AllocActor(void);
void FreeActor(Object *obj);
Object2 *pop_5d0c(void);
void clearpush_5d0c(Object2 *obj);
void set_towardsaway(Player *ply);
void LBCheckRoundResult(void);
void process_projectiles(void);
void actions_198a(void);
void actions_530a(void);
void setup_stage_actions(void);
void quirkysound(short data);
void set_ply_directions(Player *ply);
short sub_2fe6(Player *ply, Object *a0, short yoke);
void _bumplevel(void);		/* 2bf2 */
u8 calc_flightpath(Object *obj, int x, int y);
void boss_level_check (void);
void LBInitPlayers(void);
short start_effect(short d0, short d1);
void player_check_dizzy(Player *ply);	/* 377c test player dizzying */
void LBUpdateAITimers(Player *ply);	/* Updates needed by AI */
void startgame(int d2);
void sub_528a();
void bin2bcd(short d0);
void wait_for_ply_PSFinishedParticipating(void);
void sub_529c();
u32 MakePointObj(int x, int y);
void decode_buttons(Player *ply, u16 d0);
short buttonspressed(Player *ply, u16 d1);
short buttonsreleased(Player *ply, u16 d1);

void actionlibrary(void);
void soundsting(short data);		/* 62c4 */



u16 *objcoords_scroll1(Object *obj);
u16 *objcoords_scroll2(Object *obj);
u16 *objcoords_scroll3(Object *obj);
u16 *coords_scroll1(short x, short y);
u16 *coords_scroll2(short x, short y);
u16 *coords_scroll3(short x, short y);

void add_bcd_32(int op, u32 *bcd);
void add_bcd_16(u16 op, u16 *bcd);
void add_bcd_32_16(u32 op, u16 *bcd);
void add_bcd_8(u8 op, u8 *bcd);
void sub_bcd_32(int op, u32 *bcd);
void sub_bcd_32_16(u32 op, u16 *bcd);
void sub_bcd_32_8shift(u32 op, u16 *bcd);
void sub_bcd_16(u16 op, u16 *bcd);
void sub_bcd_8(u8 op, u8 *bcd);
void LBAddPoints(int d0, short d1);		// 55c2


void sub_22a8(void);
void LBDecodeInputs(void);
void LBGetInputs(void);


short	get_scr1x(void);
short	get_scr1y(void);
short	get_scr2x(void);
short	get_scr2y(void);
short	get_scr3x(void);
short	get_scr3y(void);

u32 MakePointObj (int x, int y);


/* start_effect IDs */
#define LC0_DARK_ALL_DISABLE    0x0000
#define LC0_LIGHT_ALL_ENABLE    0x0002

#define LC0_DARK_OBJECT         0x000a
#define LC0_DARK_SCROLL1        0x000c
#define LC0_DARK_SCROLL2        0x000e
#define LC0_DARK_SCROLL3        0x0010
#define LC0_LIGHT_OBJECT        0x0012
#define LC0_LIGHT_SCROLL1       0x0014
#define LC0_LIGHT_SCROLL2       0x0016
#define LC0_LIGHT_SCROLL3       0x0018
#define LC0_DARK_DUNNO          0x001a
#define LC0_DARK_123            0x001e
#define LC0_LIGHT_123           0x0020
#define LC0_DARK_3BL            0x0022
#define LC0_DUPE_LIGHT_OBJECT   0x0024
#define LC0_DUPE_LIGHT_SCROLL2  0x0026
#define LC0_DUPE_LIGHT_SCROLL3  0x0028


#endif /* INC_SF2LIB */

