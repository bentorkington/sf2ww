    signed char		exists;
    signed char		flag1;					// is Visible
    signed char		mode0;					// 0x02
    signed char		mode1;					/* Ply_Stat for Players */
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
    const struct action	*ActionScript;		// 0X1a full 32 bit pointer on CPS
    char			LocalTimer;				// 0x1e	 another Timer 
    char			SubTimer;
    signed char		Sel;					// 20
    signed char		SubSel;					// 21
    short			Layer;					// 22
    char			x0024;					/* Is threat */
    char			Scroll;					// 0x25
    struct player_t	*Owner;					/* 0x26 this is a 16-bit ptr on CPS */
    u16				x0028;					/* Unused */
    short			Energy;					// 2a
    short			EnergyDash;				// 2c
    char			x002e;
	char			x002f;
    const VECT16	*Path;					/* 0030 */
    const struct hitboxes *HitBoxes;		/* 0034 */
    const signed char     *MoreBoxes;				/* 0038 bullshit */
    FIXED8_8		VelX;					// 3c
    FIXED8_8		VelY;					// 3e
			// up to 0x40