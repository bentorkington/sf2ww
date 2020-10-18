/*
 *  gfx_glut.h
 *  GLUTBasics
 *
 *  Created by Ben Torkington on 20/04/10.
 *  Copyright 2010 Ben Torkington. All rights reserved.
 *
 */
#ifndef INC_GFX_GLUT
#define INC_GFX_GLUT

#include "sf2types.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

void gemu_clear_cache(void);
void gemu_cache_scroll1(u16 tile, short palette);
void gemu_cache_scroll2(u16 tile, short palette);
void gemu_cache_scroll3(u16 tile, short palette);
void gemu_cache_object (u16 tile, short palette);

void gfx_glut_init(void);
void gCameraReset(void);
void gfx_glut_drawgame(void);
void gfx_glut_reshape(int width, int height);

void gfx_glut_mousedown(int px, int py);
void gfx_glut_rightmousedown(int px, int py);
void gfx_glut_mouseup(int px, int py);
void gfx_glut_rightmouseup(int px, int py);
void gfx_glut_mousedragged(int px, int py);
void gfx_glut_rightmousedragged(int px, int py);

void drawGLString(GLfloat x, GLfloat y, char *string);
void ResetScrolls(void);

void gemu_readtile(u16 tileid);
void gemu_readtile_scroll3(u16 tileid);
void gemu_readtile_scroll2(u16 tileid);
void gemu_readtile_scroll1(u16 tileid);

void gemu_flip_scroll_enable(int scroll) ;

void gemu_set_cache_clear(void);

void gfx_gemu_drawgame(void);

#endif
