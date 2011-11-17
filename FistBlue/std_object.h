    signed char		exists;
    signed char		flag1;
    signed char		mode0;
    signed char		mode1;					/* Ply_Stat */
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
    const struct action	*ActionScript;
    char			LocalTimer;				//001e	 another Timer 
    char			x001f;
    signed char		Sel;					// 20
    signed char		SubSel;					// 21
    short			Layer;					// 22
    char			x0024;					/* Is threat */
    char			Scroll;
    struct player_t	*Owner;					/* this is a 16-bit ptr on CPS */
    u16				x0028;					/* Unused */
    short			Energy;
    short			EnergyDash;
    u16				x002e;
    short			*Path;					/* 0030 XXX special type for path */
    const struct hitboxes *HitBoxes;				/* 0034 */
    signed char		*MoreBoxes;				/* 0038 */
    FIXED8_8		VelX;					// 3c
    FIXED8_8		VelY;					// 3e
 