//
//  scroll_maint.h
//  MT2
//
//  Created by Ben Torkington on 24/10/20.
//

#ifndef scroll_maint_h
#define scroll_maint_h

#include "scroll.h"

enum ScrollMode1Y {
    SCR1Y_NONE,
    SCR1Y_FOLLOW_SCR2 = 2,
};

enum ScrollMode1X {
    /// Follow the perspective calculations from the Rowscroll layer
    SCR1X_PERSPECTIVE,
    /// Scroll horizontally continuously
    SCR1X_SKY = 2,
    SCR1X_SKY2 = 4,
    /// Do nothing
    SCR1X_NONE = 6,
};

enum ScrollMode2Y {
    /// Automatically follow the two players along the stage
    SCR2Y_FIGHT = 0,
    /// Do nothing
    SCR2Y_NONE = 2,
};

#define SCR3X_PERSP 0
#define SCR3X_NONE  2

#define SCR3Y_NONE  0
#define SCR3Y_2Y    2
#define SCR3Y_0_75  4
#define SCR3Y_0_50  6
#define SCR3Y_0_875 8
#define SCR3Y_1_25  10
#define SCR3Y_2Y_MINUS_OFFSET 12

#define SCR2X_FIGHT 0
#define SCR2X_NONE  2

void GSMaintScroll1(ScrollState *gstate);
void GSMaintScroll2(ScrollState *gstate);
void GSMaintScroll3(ScrollState *gstate);


#endif /* scroll_maint_h */
