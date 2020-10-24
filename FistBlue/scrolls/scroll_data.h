//
//  scroll_data.h
//  MT2
//
//  Created by Ben Torkington on 24/10/20.
//

#ifndef scroll_data_h
#define scroll_data_h

#include "scroll.h"

void GSDrawScroll1B(ScrollState *gs, u16 *gfx_p, const u16 *a1, CP cp);
void GSDrawScroll2A(ScrollState *gs, u16 *gfx_p, const u16 *tilep, CP cp);

void GSFillScroll3(ScrollState *gs);
void GSFillScroll2(ScrollState *gs);
const u16 *_GSLookupScroll1(ScrollState *gs, CP cp);
const u16 *_GSLookupScroll2(ScrollState *gs, CP cp);
const u16 *_GSLookupScroll3(ScrollState *gs, CP cp);
const u16 *skyscraper_realign(ScrollState *gs, u16 **gfx_p);
const u16 *realign_scr3a(ScrollState *gs, u16 **gfx_p);

#endif /* scroll_data_h */
