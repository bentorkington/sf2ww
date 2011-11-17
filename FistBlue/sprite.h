/* sprite.h */

#ifndef INC_SPRITE
#define INC_SPRITE

/* sprite.c */
void DSDrawShadows(void);
void draw_shadow(Player *ply, Object *obj);
void draw_layer1_grp1(void);
void draw_layer1_grp2(void);
void draw_layer3_grp1(void);
void draw_layer3_grp2(void);
void draw_layer3_grp3(void);
void draw_layer3_grp4(void);
void draw_layer3_grp5(void);
void draw_layer3_grp6(void);
void draw_layer2_grp1(void);
void draw_layer2_grp2(void);
void draw_layer2_grp3(void);
void sub_7e4dc(void);
void DSDrawAll_176(void);
void sub_7e544(void);
void DSDrawAllMain(void);
void drawsprite(Object *obj);
void dbg_draw_hitboxes(Player *ply);

void DSDrawAll_Hira(void);

const short *sub_7f224(u16 dim);



#endif /* INC_SPRITE */
