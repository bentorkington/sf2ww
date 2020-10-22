//
//  gfx_glcore.c
//  MT2-GLcore
//
//  Created by Ben Torkington on 19/10/20.
//

#include "gfx_glcore.h"

#include "cps_tile.h"
#include "gemu.h"

extern CPSGFXEMU gemu;
extern struct cps_a_regs cps_a_emu;

extern int gemu_scroll_enable[4];

// Point sprites

GLuint vertices_scroll1[CPS1_OBJ_SIZE][2];
GLuint vertices_scroll2[CPS1_OBJ_SIZE][2];
GLuint vertices_scroll3[CPS1_OBJ_SIZE][2];

GLfloat dummy[] = {
    0.5, 0.5,
    0.5, 0.0,
    0.0, 0.5,
    0.0, 0.0,
};

GLuint dummyInt[] = {
    120, 120,
    120, 250,
    250, 120,
    250, 250,
};

GLushort dummyAttribs[] = {
    0x815f, 0,
    0x2, 0,
    0x3, 0,
    0x4, 0,
};

GLuint scroll_vertex_arrays[3];
GLuint scroll_vertex_buffers[3];
GLuint scroll_attr_buffers[3];

GLuint dummyVertexArray;
GLuint dummyVertexBuffer;
GLuint dummyAttrBuffer;

GLuint vertexLocation;
GLuint attrLocation;

GLuint objectTexture;
GLuint scrollTexture;

int spintile = 0;

struct {
    GLuint tileSize;
    GLuint rowByteStride;
    GLuint tileByteSize;
    GLuint atlasWidth;
    GLuint atlasHeight;
    GLuint pointSize;
    GLuint scrollPosition;
} pl;

GLushort *scrolls[3];
GLushort *palettes[3];

#pragma mark Entry point

void *ehonda_glcore;

#include <unistd.h>
#include <stdlib.h>

void init_glcore(unsigned int shaderProgram, GLuint text1, GLuint text2) {
    objectTexture = text1;
    scrollTexture = text2;
    
    ehonda_glcore = malloc(0x20000);
    FILE *ehondadata = fopen("/Users/ben/ehondagfx_le.dat", "r");
    
    long bytesread = fread(ehonda_glcore, 1, 0x20000, ehondadata);
    printf("%ld bytes read\n", bytesread);
    fclose(ehondadata);
    
    pl.tileSize = glGetUniformLocation(shaderProgram, "tileSize");
    pl.rowByteStride = glGetUniformLocation(shaderProgram, "rowByteStride");
    pl.tileByteSize = glGetUniformLocation(shaderProgram, "tileByteSize");
    pl.atlasWidth = glGetUniformLocation(shaderProgram, "atlasWidth");
    pl.atlasHeight = glGetUniformLocation(shaderProgram, "atlasHeight");
    pl.pointSize = glGetUniformLocation(shaderProgram, "pointSize");
    pl.scrollPosition = glGetUniformLocation(shaderProgram, "scrollPosition");

    for (int i=0; i<4; i++) {
        gemu_scroll_enable[i] = 1;
    }
    
    for (int i = 0; i < CPS1_OTHER_SIZE ; i++) {
        gemu.Tilemap_Scroll1[i][0] = TILE_BLANK_SCR1;
        gemu.Tilemap_Scroll2[i][0] = TILE_BLANK_SCR2;
        gemu.Tilemap_Scroll3[i][0] = TILE_BLANK_SCR3;
        gemu.Tilemap_Scroll1[i][1] = 0;
        gemu.Tilemap_Scroll2[i][1] = 0;
        gemu.Tilemap_Scroll3[i][1] = 0;
    }
    
    for (int i = 0; i < CPS1_OTHER_SIZE; i++) {
        // yxxx xxxy yyyy
        int scr1y = (i & 0x1f) + ((i & 0x800) >> 6);
        int scr1x = (i & 0x7e0) >> 5;
        
        vertices_scroll1[i][0] = scr1x * TILE_PIXELS_SCR1 + TILE_PIXELS_SCR1 / 2;
        vertices_scroll1[i][1] = 512 - (scr1y * TILE_PIXELS_SCR1 + TILE_PIXELS_SCR1 / 2);
        
//        int tile = TILE_BLANK_SCR1 + scr1x + (63 - scr1y) * 64;
//        printf("%04x (%04d,%04d)\n", tile, vertices_scroll1[i][0], vertices_scroll1[i][1]);
//        gemu.Tilemap_Scroll1[i][0] = tile;
        
        // yyxx xxxx yyyy
        int scr2y = (i & 0xf) + ((i & 0xc00) >> 6);
        int scr2x = (i & 0x3f0) >> 4;

        vertices_scroll2[i][0] = scr2x * TILE_PIXELS_SCR2 + TILE_PIXELS_SCR2 / 2;
        vertices_scroll2[i][1] = 1024 - (scr2y * TILE_PIXELS_SCR2 + TILE_PIXELS_SCR2 / 2);

        // yyyx xxxx xyyy
        int scr3y = (i & 0x7) + ((i & 0xe00) >> 6);
        int scr3x = (i & 0x1f8) >> 3;

        vertices_scroll3[i][0] = scr3x * TILE_PIXELS_SCR3 + TILE_PIXELS_SCR3 / 2;
        vertices_scroll3[i][1] = 2048 - (scr3y * TILE_PIXELS_SCR3 + TILE_PIXELS_SCR3 / 2);
    }
    
    // set up the dummy layer
    
    vertexLocation = glGetAttribLocation(shaderProgram, "aVertexPosition");
    attrLocation = glGetAttribLocation(shaderProgram, "tileAttributes");

    printf("before dummy glError is %d\n", glGetError());

    glGenVertexArrays(1, &dummyVertexArray);
    glGenBuffers(1, &dummyVertexBuffer);
    glGenBuffers(1, &dummyAttrBuffer);
    
    glBindVertexArray(dummyVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, dummyVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * 2 * 4, dummyInt, GL_STATIC_DRAW);
    glVertexAttribIPointer(vertexLocation, 2, GL_UNSIGNED_INT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, dummyAttrBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * 2 * 4, dummyAttribs, GL_STATIC_DRAW);
    glVertexAttribIPointer(attrLocation, 2, GL_UNSIGNED_SHORT, 0, 0);

    glEnableVertexAttribArray(vertexLocation);
    glEnableVertexAttribArray(attrLocation);
    
    
    printf("after dummy glError is %d\n", glGetError());

    // set up the Scroll 1 -3 layers
    
    glGenVertexArrays(3, scroll_vertex_arrays);
    glGenBuffers(3, scroll_vertex_buffers);
    glGenBuffers(3, scroll_attr_buffers);

    // Scroll 1
    glBindVertexArray(scroll_vertex_arrays[0]);
    glBindBuffer(GL_ARRAY_BUFFER, scroll_vertex_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * 2 * CPS1_OTHER_SIZE, vertices_scroll1, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vertexLocation);

    glVertexAttribPointer(vertexLocation, 2, GL_UNSIGNED_INT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, scroll_attr_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * 2 * CPS1_OTHER_SIZE, gemu.Tilemap_Scroll1, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(attrLocation);
    glVertexAttribIPointer(attrLocation, 2, GL_UNSIGNED_SHORT, 0, 0);
    
    // Scroll 2

    glBindVertexArray(scroll_vertex_arrays[1]);
    glBindBuffer(GL_ARRAY_BUFFER, scroll_vertex_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * 2 * CPS1_OTHER_SIZE, vertices_scroll2, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vertexLocation);

    glVertexAttribPointer(vertexLocation, 2, GL_UNSIGNED_INT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, scroll_attr_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * 2 * CPS1_OTHER_SIZE, gemu.Tilemap_Scroll2, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(attrLocation);
    glVertexAttribIPointer(attrLocation, 2, GL_UNSIGNED_SHORT, 0, 0);

    // Scroll 3

    glBindVertexArray(scroll_vertex_arrays[2]);
    glBindBuffer(GL_ARRAY_BUFFER, scroll_vertex_buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * 2 * CPS1_OTHER_SIZE, vertices_scroll3, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vertexLocation);

    glVertexAttribPointer(vertexLocation, 2, GL_UNSIGNED_INT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, scroll_attr_buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * 2 * CPS1_OTHER_SIZE, gemu.Tilemap_Scroll3, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(attrLocation);
    glVertexAttribIPointer(attrLocation, 2, GL_UNSIGNED_SHORT, 0, 0);

    // done!
    
    glBindVertexArray(0);
    
    int error = glGetError();
    printf("after init glError is %d\n", error);
    
    if(1) {
        scrolls[0] = &ehonda_glcore[0xc000];
        scrolls[1] = &ehonda_glcore[0x4000];
        scrolls[2] = &ehonda_glcore[0x8000];
        palettes[0] = &ehonda_glcore[0x400];
        palettes[1] = &ehonda_glcore[0x800];
        palettes[2] = &ehonda_glcore[0xc00];
        
        cps_a_emu.scroll1x = 0x1c6;
        cps_a_emu.scroll2x = 0x108 + 0xc0;
        cps_a_emu.scroll3x = 0x1c4;
    } else {
        scrolls[0] = (GLushort *)gemu.Tilemap_Scroll1;
        scrolls[1] = (GLushort *)gemu.Tilemap_Scroll2;
        scrolls[2] = (GLushort *)gemu.Tilemap_Scroll3;
        palettes[0] = (GLushort *)gemu.PalScroll1;
        palettes[1] = (GLushort *)gemu.PalScroll2;
        palettes[2] = (GLushort *)gemu.PalScroll3;
    }
}

void render_dummy(void) {
    glUniform1i(pl.tileSize, 16);
    glUniform1f(pl.pointSize, 16.0);
    glUniform1i(pl.rowByteStride, 8);
    glUniform1i(pl.tileByteSize, 0x80);
    glUniform1i(pl.atlasWidth, 2048);
    glUniform1i(pl.atlasHeight, 2304);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, objectTexture);
    
    glBindVertexArray(dummyVertexArray);
    glDrawArrays(GL_POINTS, 0, 4);
}


void set_up_sprite_shader() {
    glUniform1i(pl.tileSize, 16);
    glUniform1f(pl.pointSize, 16.0);
    glUniform1i(pl.rowByteStride, 8);
    glUniform1i(pl.tileByteSize, 0x80);
    glUniform1i(pl.atlasWidth, 2048);
    glUniform1i(pl.atlasHeight, 2304);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, objectTexture);
}

void set_up_scroll1_shader() {
    glUniform1i(pl.tileSize, 8);
    glUniform1f(pl.pointSize, 8.0);
    glUniform1i(pl.rowByteStride, 8);
    glUniform1i(pl.tileByteSize, 0x40);
    glUniform1i(pl.atlasWidth, 2048);
    glUniform1i(pl.atlasHeight, 1024);
    glUniform2i(pl.scrollPosition, cps_a_emu.scroll1x % 512, cps_a_emu.scroll1y % 512);
    
    //printf ("%d, %d\n", gemu.Scroll1X % 512, gemu.Scroll1Y % 512);
    glActiveTexture(GL_TEXTURE1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 32, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, palettes[0]);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, scrollTexture);
}

void set_up_scroll2_shader() {
    glUniform1i(pl.tileSize, 16);
    glUniform1f(pl.pointSize, 16.0);

    glUniform1i(pl.rowByteStride, 8);
    glUniform1i(pl.tileByteSize, 0x80);
    glUniform1i(pl.atlasWidth, 2048);
    glUniform1i(pl.atlasHeight, 1024);
    glUniform2i(pl.scrollPosition, cps_a_emu.scroll2x % 1024, cps_a_emu.scroll2y % 1024);

    glActiveTexture(GL_TEXTURE1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 32, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, palettes[1]);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, scrollTexture);
}

void set_up_scroll3_shader() {
    glUniform1i(pl.tileSize, 32);
    glUniform1f(pl.pointSize, 32.0);
    glUniform1i(pl.rowByteStride, 16);
    glUniform1i(pl.tileByteSize, 0x200);
    glUniform1i(pl.atlasWidth, 2048);
    glUniform1i(pl.atlasHeight, 1024);
    glUniform2i(pl.scrollPosition, cps_a_emu.scroll3x % 2048, cps_a_emu.scroll3y % 2048);

    glActiveTexture(GL_TEXTURE1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 32, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, palettes[2]);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, scrollTexture);
}

void render_glcore(void) {
//    render_dummy();
    
    // scroll 1
    glBindBuffer(GL_ARRAY_BUFFER, scroll_attr_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * 2 * CPS1_OTHER_SIZE, scrolls[0], GL_DYNAMIC_DRAW);
    set_up_scroll1_shader();
    glBindVertexArray(scroll_vertex_arrays[0]);
    glDrawArrays(GL_POINTS, 0, CPS1_OTHER_SIZE);
    
    // scroll 2
    glBindBuffer(GL_ARRAY_BUFFER, scroll_attr_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * 2 * CPS1_OTHER_SIZE, scrolls[1], GL_DYNAMIC_DRAW);
    set_up_scroll2_shader();
    glBindVertexArray(scroll_vertex_arrays[1]);
    glDrawArrays(GL_POINTS, 0, CPS1_OTHER_SIZE);
    
    // scroll 3
    glBindBuffer(GL_ARRAY_BUFFER, scroll_attr_buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLushort) * 2 * CPS1_OTHER_SIZE, scrolls[2], GL_DYNAMIC_DRAW);
    set_up_scroll3_shader();
    glBindVertexArray(scroll_vertex_arrays[2]);
    glDrawArrays(GL_POINTS, 0, CPS1_OTHER_SIZE);
}
