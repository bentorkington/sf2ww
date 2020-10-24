//
//  gfx_glcore.h
//  MT2-GLcore
//
//  Created by Ben Torkington on 19/10/20.
//

#ifndef gfx_glcore_h
#define gfx_glcore_h

#include <stdio.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h>
#endif

void render_glcore(void);
void init_glcore(unsigned int shaderProgram, unsigned int shader2, GLuint objectTexture, GLuint scrollTexture);

#endif /* gfx_glcore_h */
