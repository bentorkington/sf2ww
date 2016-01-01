/* particle.h */
#ifndef INC_PARTICLE
#define INC_PARTICLE

#include "gstate.h"

#define	GFX_LAYER1 4
#define GFX_LAYER2 6
#define GFX_LAYER3 8

typedef struct action Action;

enum HitBoxID {
	HB_HEAD,
	HB_BODY,
	HB_FOOT,
	HB_WEAK,
	HB_ACTIVE,
	HB_PUSH,
};



struct particle {
	signed char		exists;
    signed char		flag1;
    signed char		mode0;
    signed char		mode1; /* Ply_Stat */
    signed char		mode2;
    signed char		mode3;
    FIXED16_16		X;
    FIXED16_16		Y;
    short			ZDepth;					/* is on ground, X coord should be parallax corrected */
    signed char		Pool;					/* 0x10 */
    signed char		Step;
    signed char		Flip;					// FACING_LEFT=0 (no flip) FACING_RIGHT=1 (flipped) 
    char			Draw1;	  				/* flag, should draw with Draw2 color instead */
    u16				AnimFlags;				/* 0x0014 maybe split into two? */
	FIXED8_8		Draw2;					// 0x16
    short			Timer;                  // 0x18 
    struct action	*ActionScript;
    char			LocalTimer;				//001e	 another Timer 
    char			SubTimer;
    signed char		Sel;					// 20
    signed char		SubSel;					// 21
    short			Layer;					// 22
    char			x0024;					/* 24 is Projectile */
    char			Scroll;
    struct player_t			*Owner;					/* this is a 16-bit ptr on CPS */
    u16				x0028;					/* Unused */
    short			Energy;
    short			EnergyDash;
    u16				x002e;
    short			*Path;					/* 0030 XXX special type for path */
    const struct hitboxes *HitBoxes;				/* 0034 */
    signed char		*MoreBoxes;				/* 0038 */
    FIXED8_8		VelX;					// 3c
    FIXED8_8		VelY;					// 3e
};

typedef u32 RHImagePtr;

struct fb_action {
    u16		Delay;			/* aka chr ctr */
    u16		Flags;
    RHImagePtr Image;
    u8		HB_Head, HB_Body, HB_Foot, HB_Weak, HB_Active, HB_Push;	/* 8-0xd */
    u8		Shadow;     /* 0xe */
    u8		Priority;	/* f  */
    u8		Catch;      /* 10 CATCH index for data_93440[] @ply_thrown */
    u8		Block;		/* 0 for non block, 1 for standing block, 2 for crouching */
    u8		Crouch;     /* exit status stand/crouch 0x12 AI uses this to determine if should block */
    /* mugenguild dude has this as weak box mode, determines behaviour when chars
     weak box is hit */
    u8		FlipBits;   /* 0x13 */
    /* mugenguild dude says this is 0 for stand, 1 for crouch */
    char	YOffset;    /* 0x14 */
    u8		DamageMod;  /* 0x15 */
    u8		ExtraSprite;/* extra sprite selector*/
    char	Yoke;		/* YOKE, 0x17 neutral jumps, 06 forward/backward jumps, 0xff walking */
} __attribute__((packed));

typedef struct fb_action FBAction;

struct action {
    u16		Delay;			/* aka chr ctr */
    u8		Loop;			/* aka chr type */
    u8		Next;
	const struct image *Image;
    u8		HB_Head, HB_Body, HB_Foot, HB_Weak, HB_Active, HB_Push;	/* 8-0xd */
    u8		Shadow;     /* 0xe */
    u8		Priority;	/* f  */
    u8		Catch;      /* 10 CATCH index for data_93440[] @ply_thrown */
    u8		Block;		/* 0 for non block, 1 for standing block, 2 for crouching */
    u8		Crouch;     /* exit status stand/crouch 0x12 AI uses this to determine if should block */
					/* mugenguild dude has this as weak box mode, determines behaviour when chars
					   weak box is hit */
    u8		FlipBits;   /* 0x13 */
					/* mugenguild dude says this is 0 for stand, 1 for crouch */
    char	YOffset;    /* 0x14 */
	u8		DamageMod;  /* 0x15 */
    u8		ExtraSprite;/* extra sprite selector*/
    char	Yoke;		/* YOKE, 0x17 neutral jumps, 06 forward/backward jumps, 0xff walking */
};

struct fb_simpleaction {
    short Delay;
    short Flags;
    RHImagePtr Image;
} __attribute__((packed));

typedef struct fb_simpleaction FBSimpleAction;

typedef struct image Image;

struct image {
    u16		TileCount;
    u16		Attr;
    u16		Dimensions;
	short	OffsetX;
	short	OffsetY;
	u16		Tiles[];		// C99
};

typedef struct image FBImage;

struct image2 {
	u16		Width;
	u16		Height;
	u16		Palette;
	u16		Tiles[];		// C99
};

typedef struct image2 SimpleImage;

struct actionhdr {
    u8  Type;
    u8  SubSel;
    u8  Sel;
    u8  Step;
    u8  Scroll;
    u8  UserByte;
    u16 ZDepth;
    u16 X;
    u16 Y;
};

struct hitbox_active {		/* Attack hitbox is 0xc long */
    signed char X, Y, width, height;
    u8		Damage;      /* 0x4 */
    char	Shove;       /* 0x5 future victim 0x4a */
    u8		Sound;
    u8		Attack;	/* 0x7 ATck EX 0=standard normal
				 1=normal sweep
				 2=jumping normal
				 3=special
				 4=norm with priority  */
    u8 Strength;		/* 0=light,1=med 2=hard */
	
    u8 Random1;     
    u8 Random2;     /* 0xa */
    u8 ReactMode;   /* 0xb */
};

#define EMPTY_HITBOX		{0,0,0,0}
#define EMPTY_HITBOXACT		{0,0,0,0,0,0,0,0,0,0,0,0}

struct hitbox {		/* Attack hitbox is 0xc long */
    signed char X;			// DX
    signed char Y;			// DY
    signed char width;		// SX
    signed char height;		// SY
};

struct hitboxes {
	const struct hitbox			*head;
	const struct hitbox			*body;
	const struct hitbox			*foot;
	const struct hitbox			*weak;
	const struct hitbox_active	*active;
	const struct hitbox			*push;
};

typedef struct hitbox HitBox;
typedef struct hitbox_active HitBoxAct;


typedef struct {
#include "std_object.h"
    FIXED8_8		AclX;
    FIXED8_8		AclY;
    u16				x0044;		
    signed char		UserByte;
    char			Timer2;						/* 0047 */
    char			BoundCheck;					/* 0x48 also a void *??? */
    char			BlockStun;
    char			SufferHB5;					// 0x4a
	char			NextReactMode;				/* 0x4b */
	char			NextReelStrength;			/* 0x4c was NextReactMode2  0=light 1=med 2=heavy */
	char			OnPlatform;					// 0x4d
	char			OnPlatform2;				// 0x4e
	u8				x004f[3];					/* for hitboxes 1,2,3 */
	short			DSOffsetX;
	char			Direction;			/* 0x54 FACING_LEFT=0, FACING_RIGHT=1 */
	u8				ReactMode;			/* 0x55 see RM_* */
	u8				ReactMode2;         // 0x56 0=light 1=med 2=heavy
	u8				StepSave;			// 0x57
	u8				Difficulty;
	u8				x0059;
	char			OnGround;			// 0x5a
	char			PlatformFallDir;			/* 0x5b direction falling off platform */
	short			DizzyClearCnt, DizzyCount, DizzySpell;
	short			IsBlockingSuper;	// 0062
	char			ThrowStat;
	short			Throw[6];			// XXX 3 x Point
	char			x0070;
	char			ProjectilePushBack;
	// gap
	u8				ThrowTrajectory;    /* 0x0076 */
	char			x0077;
	short			UndealtDamage;
	u16				RewardID;
	short			Damage1;            /* 0x7c */
	short			Damage2;
	u8				UserData[0x80];
} Object;

typedef struct {
#include "std_object.h"
    signed char		x0040;			/* some palette index, values 1,2,3,5 */
	signed char		x0041;
	void			*x0042;			/* palette? data at 0x90000 set by 82c26 used 82c4c */
    u8				UserByte;
    signed char		Timer2;				/* 0047 prob not a timer in Object2*/
	void			*x0048;
	
	// rest of this could be rubbish
	
	u8				NextReelStrength;		/* 4c */
	char			x004d;
	const signed char		*x004e;
	short			DSOffsetX;
	char			Direction;      
	u8				ReactMode;
	u8				ReactMode2;
	
	char			OnPlatform;
	char			OnPlatform2;
	
	u8				StepSave;		// 0057
	u8      Difficulty;
	u8      x0059;
	char    OnGround;				/* 0x5a TRUE or FALSE */
	char    PlatformFallDir;		/* FACING_LEFT, FACING_RIGHT direction falling off platform */
	short   DizzyClearCnt;
	short   DizzyCount;
	short   DizzySpell;
	short   IsBlockingSuper;		//0062
	char    ThrowStat;
	short   Throw[6];
	char    x0070;
	char    ProjectilePushBack;
	
	u8      ThrowTrajectory;    /* 0x0076 */
	char    x0077;
	short   UndealtDamage;
	u16     RewardID;
	short     Damage1;            /* 0x7c */
	short     Damage2;
} Object2;


//typedef struct simpleaction CASimpleFrame;

typedef struct action CAFrame;

/* particle.c */

#ifdef REDHAMMER_EXTROM
void RHSetActionList(Object *obj, const void *listaddr, short sel);
void RHSetAction(Object *obj, const FBAction *act);
void RHActionTick(Object *obj);
#endif

void setaction_list(Object *obj, const Action **list, short sel);
void setaction_direct(Object *obj, const Action *act);
void actiontick(Object *obj);
void check_rect_queue_draw(Object *obj);
void check_onscreen_queue(Object *obj);
GState *get_graphics_context(Object *obj);
void update_motion(Object *obj);
void CATrajectory(Object *obj);
void enqueue_and_layer(Object *obj);
int check_ground_collision(Object *ply);
void update_obj_path(Object *ply);
Object *alloc_action_by_type(short type);
void CAApplyVelocity(Object *obj);
void die_if_offscreen(Object *obj);	// 248c
void sub_25f8(Object *obj);



#endif