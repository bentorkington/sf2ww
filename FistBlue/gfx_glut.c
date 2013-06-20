/* gfxlib.c functions in here are loose emulations of CPS graphics */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

/* needs ifdef __APPLE__ since they put glut.h somewhere else */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "sf2types.h"
#include "sf2const.h"
#include "sf2macros.h"

#include "structs.h"
#include "player.h"
#include "gstate.h"
#include "lib.h"

#include "particle.h"

extern Game g;

#include "glwimp.h"
#include "gemu.h"
#include "gfxlib.h"
#include "gfx_glut.h"
#include "trackball.h"
#include "effects.h"



extern CPSGFXEMU gemu;
extern GLfloat gWimpScale;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;

extern struct executive_t Exec;
extern struct effectstate es;

int gemu_scroll_enable[4];

FILE * gfxrom;

typedef struct {
	GLdouble x,y,z;
} recVec;

typedef struct {
	recVec viewPos; // View position
	recVec viewDir; // View direction vector
	recVec viewUp; // View up direction
	recVec rotPoint; // Point to rotate about
	GLdouble focalLength; // Focal Length along view direction
	GLdouble aperture; // gCamera aperture
	GLdouble eyeSep; // Eye separation
	GLfloat screenWidth,screenHeight; // current window/screen height and width
} recCamera;

void drawGLText(recCamera cam);


clock_t start, finish ;
double duration, frames, FPS ;
float lightX, theta, radius;

int gframecnt = 0;
int camera=0;
int texture_mode=1;
int enable_lighting=1;
int showTextures=1;

#define DTOR 0.0174532925

#define GFXROM_READFOUR \
buf[0]=getc(gfxrom);	\
buf[1]=getc(gfxrom);	\
buf[2]=getc(gfxrom);	\
buf[3]=getc(gfxrom);

#define PALETTE_MASK_BRIGHTNESS 0xf000
#define PALETTE_MASK_COLOR_R    0x0f00
#define PALETTE_MASK_COLOR_G    0x00f0
#define PALETTE_MASK_COLOR_B    0x000f

#define TRANSPARENT_COLOR_R     0
#define TRANSPARENT_COLOR_G     255
#define TRANSPARENT_COLOR_B     0
#define PALETTE_TRANSPARENT_ID  15

#define ALPHA_TRANS   0x00          /* Alpha value for fully-transparent */
#define ALPHA_OPAQUE  0xff          /* and fully opaque                  */


#define TILE_SIZE_SCR3  1.0f
#define TILE_SIZE_SCR2  (TILE_SIZE_SCR3 / 2.0f)
#define TILE_SIZE_SCR1  (TILE_SIZE_SCR3 / 4.0f)
#define TILE_SIZE_OBJ   (TILE_SIZE_SCR3 / 2.0f)

#define TILE_PIXELS_SCR1    8
#define TILE_PIXELS_SCR2    16
#define TILE_PIXELS_SCR3    32
#define TILE_PIXELS_OBJ     16

/* Tile ROM locations and bytesizes */
#define TILE_OFFSET_OBJECT      0
#define TILE_OFFSET_SCROLLS     0x400000
#define TILE_BYTES_8x8          0x40        // not a bug, the SCR1 tiles are packed ineffieintly
#define TILE_BYTES_16x16        0x80
#define TILE_BYTES_32x32        0x200


/* Tile attribute masks */
#define TILE_MASK_FLIP          0x0060
#define TILE_MASK_PALETTE       0x001f

/* Object masks */
#define TILE_MASK_BLOCK_X   0x0f00
#define TILE_MASK_BLOCK_Y   0xf000
#define TILE_MASK_BLOCK     (TILE_MASK_BLOCK_X | TILE_MASK_BLOCK_Y)
#define TILE_MASK_OFFSET    0x01ff

#define TILE_BRIGHT_TO_FLOAT    61140.0     // divide by this to normalise brightness to 1.0

/* Blank tile IDs */
#define TILE_BLANK_SCR1     0x4000
#define TILE_BLANK_SCR2     0x2800
#define TILE_BLANK_SCR3     0x400

/* conversion from plain x,y coords to how tiles are packed in CPS RAM */
#define SCROLL_DECODE_SCR1(tx,ty) ((((ty) & 0x20) << 8) + ((tx) << 5) + ((ty) & 0x1f))
#define SCROLL_DECODE_SCR2(tx,ty) ((((ty) & 0x30) << 6) + ((tx) * 16) + ((ty) & 0x0f))
#define SCROLL_DECODE_SCR3(tx,ty) ((((ty) & 0x38)<<6) + ((tx) << 3) + ((ty) & 7))

#define TILE_BRIGHT_TO_FLOAT    61140.0
#define TILE_OBJECT_END_TAG     0xff00


#define TEXTURE_CACHE_SIZE      0x10000

recCamera gCamera;
recVec gOrigin;

LBWindow *dummyWindow;
LBView *gameView;
LBView infoView;


GLint gDollyPanStartPoint[2];
GLfloat gTrackBallRotation[4];
GLfloat gWorldRotation[4];
GLboolean gDolly;
GLboolean gPan;
GLboolean gTrackBall;	

int gGameInWindow = 0;
int gShowInfo = 1;
int gShowHelp = 1;


GLint va_scroll1[65 * 65][3];
GLuint vaindex_scroll1[64 * 64][4];


static void draw_scroll1(void);
static void draw_scroll2(void);
static void draw_scroll3(void);
static void draw_object(void);

void (*SCROLL[])(void) = {
	draw_object,
	draw_scroll1,
	draw_scroll2,
	draw_scroll3,
};

static struct texture_cache_t {
	GLuint text_scr1[TEXTURE_CACHE_SIZE][2];
	GLuint text_scr2[TEXTURE_CACHE_SIZE][2];
	GLuint text_scr3[TEXTURE_CACHE_SIZE][2];
	GLuint text_obj [TEXTURE_CACHE_SIZE][2];
} TC;
int gemuCacheClear;

const GLfloat flips[4][4][2] = { 
	{{1.0, 1.0},{0.0, 1.0},{0.0, 0.0},{1.0, 0.0}},
	{{0.0, 1.0},{1.0, 1.0},{1.0, 0.0},{0.0, 0.0}},
	{{1.0, 0.0},{0.0, 0.0},{0.0, 1.0},{1.0, 1.0}},
	{{0.0, 0.0},{1.0, 0.0},{1.0, 1.0},{0.0, 1.0}},
};

void gemu_flip_scroll_enable(int scroll) {
	gemu_scroll_enable[scroll] ^= 1;
}

void gfx_glut_reshape(int width, int height) {
	printf("Reshaping to (%d,%d)\n", width, height);
	gCamera.screenWidth  = width;
	gCamera.screenHeight = height;
	glViewport(0, 0, width, height);
}
	
void ResetScrolls(void){
	palette_base_1k();
	scroll1_base_1k(g.CurrentStage);
	scroll2_base_1k(g.CurrentStage);
	scroll3_base_1k(g.CurrentStage);
	GSInitForStage();
	gstate_Scroll1.mode0=0;
	gstate_Scroll1.mode1=0;
	gstate_Scroll2.mode0=0;
	gstate_Scroll2.mode1=0;
	gstate_Scroll3.mode0=0;
	gstate_Scroll3.mode1=0;
	GSInitOffsets();
	glutPostRedisplay();
}

void gemu_set_cache_clear(void) {
	gemuCacheClear = TRUE;
}

void gemu_clear_cache(void) {
	int i;
	for (i=0; i<TEXTURE_CACHE_SIZE; ++i) {
		if (TC.text_scr1[i]) {
			glDeleteTextures(1, TC.text_scr1[i]);
			TC.text_scr1[i][0] = 0;
		}
	}
	for (i=0; i<TEXTURE_CACHE_SIZE; ++i) {
		if (TC.text_scr2[i]) {
			glDeleteTextures(1, TC.text_scr2[i]);
			TC.text_scr2[i][0] = 0;
		}			
	}
	for (i=0; i<TEXTURE_CACHE_SIZE; ++i) {
		if (TC.text_scr3[i]) {
			glDeleteTextures(1, TC.text_scr3[i]);
			TC.text_scr3[i][0] = 0;
		}			
	}
	for (i=0; i<TEXTURE_CACHE_SIZE; ++i) {
		if (TC.text_obj[i][0]) {
			glDeleteTextures(1, &TC.text_obj[i][0]);
			TC.text_obj[i][0] = 0;
		}			
	}
	gemuCacheClear = FALSE;
}

static inline void gemu_color_tile(int pixelSize, short palette, GLubyte *img, GPAL (*scrollPalette)[32][16]) {
    int u,v, master;
	
    for(u=0; u<pixelSize; u++) {
        for(v=0; v<pixelSize; v++) {
            if(gemu.tile[u][v] == PALETTE_TRANSPARENT_ID) {
                /* transparent pixel */
                *img++ = TRANSPARENT_COLOR_R;
                *img++ = TRANSPARENT_COLOR_G;
                *img++ = TRANSPARENT_COLOR_B;
                *img++ = ALPHA_TRANS; /* alpha */
            } else {
                /* convert 4bit RGB + brightness to 8-bit RGBA */
                if(gemu.FadeEnable) {
                    master = (17 * ((*scrollPalette)[palette][ gemu.tile[u][v] ])>>12) / 15;
                } else {
                    master = 17;    /* 4-bit to 8-bit conversion */
                }
                *img++ = master * (((*scrollPalette)[palette][ gemu.tile[u][v] ] & PALETTE_MASK_COLOR_R) >> 8);
                *img++ = master * (((*scrollPalette)[palette][ gemu.tile[u][v] ] & PALETTE_MASK_COLOR_G) >> 4);
                *img++ = master * (((*scrollPalette)[palette][ gemu.tile[u][v] ] & PALETTE_MASK_COLOR_B)     );
                *img++ = ALPHA_OPAQUE; /* alpha */
            }
        }
    }
}

void gemu_cache_scroll1(u16 tile, short palette) {
	static GLubyte tempmap[8][8][4];
	if (TC.text_scr1[tile][0] && TC.text_scr1[tile][1] != palette) {
		glDeleteTextures(1, &TC.text_scr1[tile][0]);
		TC.text_scr1[tile][0] = 0;
	}	
	if (TC.text_scr1[tile][0] == 0) {
		gemu_readtile_scroll1(tile);
        gemu_color_tile(TILE_PIXELS_SCR1, palette, (GLubyte *)tempmap, &gemu.PalScroll1);
		glGenTextures(1, &TC.text_scr1[tile][0]);
		if (&TC.text_scr1[tile][0]==0) {
			panic(999);
		}
		TC.text_scr1[tile][1] = palette;
		glBindTexture(GL_TEXTURE_2D, TC.text_scr1[tile][0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempmap);
	} else {
		glBindTexture(GL_TEXTURE_2D, TC.text_scr1[tile][0]);
	}
}
void gemu_cache_scroll2(u16 tile, short palette) {
	static GLubyte tempmap[16][16][4];
	if (TC.text_scr2[tile][0] && TC.text_scr2[tile][1] != palette) {
		glDeleteTextures(1, &TC.text_scr2[tile][0]);
		TC.text_scr2[tile][0] = 0;
	}		
	if (TC.text_scr2[tile][0] == 0) {
		gemu_readtile_scroll2(tile);
        gemu_color_tile(TILE_PIXELS_SCR2, palette, (GLubyte *)tempmap, &gemu.PalScroll2);
		glGenTextures(1, &TC.text_scr2[tile][0]);
		if (&TC.text_scr2[tile][0]==0) {
			panic(999);
		}
		TC.text_scr2[tile][1] = palette;
		glBindTexture(GL_TEXTURE_2D, TC.text_scr2[tile][0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempmap);
	} else {
		glBindTexture(GL_TEXTURE_2D, TC.text_scr2[tile][0]);
	}
}
void gemu_cache_scroll3(u16 tile, short palette) {
	static GLubyte tempmap[32][32][4];
	if (TC.text_scr3[tile][0] && TC.text_scr3[tile][1] != palette) {
		glDeleteTextures(1, &TC.text_scr3[tile][0]);
		TC.text_scr3[tile][0] = 0;
	}	
	if (TC.text_scr3[tile][0] == 0) {
		gemu_readtile_scroll3(tile);
        gemu_color_tile(TILE_PIXELS_SCR3, palette, (GLubyte *)tempmap, &gemu.PalScroll3);
		glGenTextures(1, &TC.text_scr3[tile][0]);
		if (&TC.text_scr3[tile][0]==0) {
			panic(999);
		}
		TC.text_scr3[tile][1] = palette;
		glBindTexture(GL_TEXTURE_2D, TC.text_scr3[tile][0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempmap);
	} else {
		glBindTexture(GL_TEXTURE_2D, TC.text_scr3[tile][0]);
	}
}
void gemu_cache_object(u16 tile, short palette) {
	static GLubyte tempmap[16][16][4];
	if (TC.text_obj[tile][0] && TC.text_obj[tile][1] != palette) {
		glDeleteTextures(1, &TC.text_obj[tile][0]);
		TC.text_obj[tile][0] = 0;
	}
	if (TC.text_obj[tile][0] == 0) {
		gemu_readtile(tile);
        gemu_color_tile(TILE_PIXELS_OBJ, palette, (GLubyte *)tempmap, &gemu.PalObject);
		glGenTextures(1, &TC.text_obj[tile][0]);
		if (&TC.text_obj[tile][0]==0) {
			panic(999);
		}
		TC.text_obj[tile][1] = palette;
		glBindTexture(GL_TEXTURE_2D, TC.text_obj[tile][0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempmap);
	} else {
		glBindTexture(GL_TEXTURE_2D, TC.text_obj[tile][0]);
	}
}

void gfx_glut_init(void) {
	int i;
	
    gfxrom=fopen( "./sf2gfx.bin", "r" );
    if(gfxrom == NULL) {
        printf("fatal: couldn't open graphics ROM in %s", getcwd(NULL, 0));
        exit(EXIT_FAILURE);
    }
    printf("opened sf2gfx.bin\n");
    gemu.FadeEnable = FALSE;
	
	for (i=0; i<4; i++) {
		gemu_scroll_enable[i] = TRUE;
	}
	
	for (i=0; i<CPS1_OTHER_SIZE; i++) {
		gemu.Tilemap_Scroll1[i][0]=0xf;
		gemu.Tilemap_Scroll2[i][0]=0x0;
		gemu.Tilemap_Scroll3[i][0]=0x0;		
		gemu.Tilemap_Scroll1[i][1]=0x0;
		gemu.Tilemap_Scroll2[i][1]=0x0;
		gemu.Tilemap_Scroll3[i][1]=0x0;		
	}
	gWorldRotation[0] = 180.0;
	gWorldRotation[1] = 0.0;
	gWorldRotation[2] = -1.0;
	gWorldRotation[3] = 0.0;
	
	start = clock();

}

const static unsigned char pixbit[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };


void gemu_readtile(u16 tileid) {          /* read a 16x16 tile */
    int u, v;
    unsigned char buf[4];
    int tileaddr = (tileid * TILE_BYTES_16x16) + TILE_OFFSET_OBJECT;
    
    memset(&gemu.tile, 0, sizeof(gemu.tile));   /* Clear the previous tile out */
    
    fseek(gfxrom, tileaddr, SEEK_SET);
    for(u=0; u<16; u++) {
        GFXROM_READFOUR;
        for(v=0; v<8; v++) {
            if (buf[0] & pixbit[v]) { gemu.tile[u][v] += 1; }
            if (buf[1] & pixbit[v]) { gemu.tile[u][v] += 2; }
            if (buf[2] & pixbit[v]) { gemu.tile[u][v] += 4; }
            if (buf[3] & pixbit[v]) { gemu.tile[u][v] += 8; }
        }
        GFXROM_READFOUR;
        for(v=0; v<8; v++) {        
            if (buf[0] & pixbit[v]) { gemu.tile[u][v+8] += 1; }
            if (buf[1] & pixbit[v]) { gemu.tile[u][v+8] += 2; }
            if (buf[2] & pixbit[v]) { gemu.tile[u][v+8] += 4; }
            if (buf[3] & pixbit[v]) { gemu.tile[u][v+8] += 8; }
        }
    }
}
void gemu_readtile_scroll1(u16 tileid) {
    int u, v;
    unsigned char buf[4];
	
    int tileaddr = (tileid * TILE_BYTES_8x8) + TILE_OFFSET_SCROLLS; 
	    
    memset(&gemu.tile, 0, sizeof(gemu.tile));  
    
    fseek(gfxrom, tileaddr, SEEK_SET);
    for(u=0; u<8; u++) {
        GFXROM_READFOUR;
        for(v=0; v<8; v++) {
            if (buf[0] & pixbit[v]) { gemu.tile[u][v] += 1; }
            if (buf[1] & pixbit[v]) { gemu.tile[u][v] += 2; }
            if (buf[2] & pixbit[v]) { gemu.tile[u][v] += 4; }
            if (buf[3] & pixbit[v]) { gemu.tile[u][v] += 8; }
        }
        GFXROM_READFOUR;		/* skip one */
    }
}
void gemu_readtile_scroll2(u16 tileid) {
    int u, v;
    unsigned char buf[4];
	
    int tileaddr = (tileid * TILE_BYTES_16x16) + TILE_OFFSET_SCROLLS;

    memset(&gemu.tile, 0, sizeof(gemu.tile));  
    
    fseek(gfxrom, tileaddr, SEEK_SET);
    for(u=0; u<16; u++) {
        GFXROM_READFOUR;
        for(v=0; v<8; v++) {
            if (buf[0] & pixbit[v]) { gemu.tile[u][v] += 1; }
            if (buf[1] & pixbit[v]) { gemu.tile[u][v] += 2; }
            if (buf[2] & pixbit[v]) { gemu.tile[u][v] += 4; }
            if (buf[3] & pixbit[v]) { gemu.tile[u][v] += 8; }
        }
        GFXROM_READFOUR;
        for(v=0; v<8; v++) {
            if (buf[0] & pixbit[v]) { gemu.tile[u][v+8] += 1; }
            if (buf[1] & pixbit[v]) { gemu.tile[u][v+8] += 2; }
            if (buf[2] & pixbit[v]) { gemu.tile[u][v+8] += 4; }
            if (buf[3] & pixbit[v]) { gemu.tile[u][v+8] += 8; }
        }
    }
}
void gemu_readtile_scroll3(u16 tileid) {
    unsigned char pixbit[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    int u, v;
    unsigned char buf[4];
    if(tileid == 0x400) {tileid = 0x404;}
	
	int tileaddr = (tileid * TILE_BYTES_32x32) + TILE_OFFSET_SCROLLS;
	
    memset(&gemu.tile, 0, sizeof(gemu.tile));  
    
    fseek(gfxrom, tileaddr, SEEK_SET);
    for(u=0; u<32; u++) {
        GFXROM_READFOUR;
        for(v=0; v<8; v++) {
            if (buf[0] & pixbit[v]) { gemu.tile[u][v] += 1; }
            if (buf[1] & pixbit[v]) { gemu.tile[u][v] += 2; }
            if (buf[2] & pixbit[v]) { gemu.tile[u][v] += 4; }
            if (buf[3] & pixbit[v]) { gemu.tile[u][v] += 8; }
        }
        GFXROM_READFOUR;
        for(v=0; v<8; v++) {
            if (buf[0] & pixbit[v]) { gemu.tile[u][v+8] += 1; }
            if (buf[1] & pixbit[v]) { gemu.tile[u][v+8] += 2; }
            if (buf[2] & pixbit[v]) { gemu.tile[u][v+8] += 4; }
            if (buf[3] & pixbit[v]) { gemu.tile[u][v+8] += 8; }
        }
		GFXROM_READFOUR;
        for(v=0; v<8; v++) {
            if (buf[0] & pixbit[v]) { gemu.tile[u][v+16] += 1; }
            if (buf[1] & pixbit[v]) { gemu.tile[u][v+16] += 2; }
            if (buf[2] & pixbit[v]) { gemu.tile[u][v+16] += 4; }
            if (buf[3] & pixbit[v]) { gemu.tile[u][v+16] += 8; }
        }
        GFXROM_READFOUR;
        for(v=0; v<8; v++) {
            if (buf[0] & pixbit[v]) { gemu.tile[u][v+24] += 1; }
            if (buf[1] & pixbit[v]) { gemu.tile[u][v+24] += 2; }
            if (buf[2] & pixbit[v]) { gemu.tile[u][v+24] += 4; }
            if (buf[3] & pixbit[v]) { gemu.tile[u][v+24] += 8; }
        }
    }
}

static inline void draw_gl_tile(int sx, int sy, int flip, float size) {
    glBegin(GL_POLYGON);
    glTexCoord2f(flips[flip][0][0],flips[flip][0][1]);
    glVertex3f(((sx+1) * size), ((sy+1) * size), 0.0f);
    glTexCoord2f(flips[flip][1][0],flips[flip][1][1]);
    glVertex3f((( sx ) * size), ((sy+1) * size), 0.0f);
    glTexCoord2f(flips[flip][2][0],flips[flip][2][1]);
    glVertex3f((( sx ) * size), (( sy ) * size), 0.0f);
    glTexCoord2f(flips[flip][3][0],flips[flip][3][1]);
    glVertex3f(((sx+1) * size), (( sy ) * size), 0.0f);
    glEnd();
}

static void draw_scroll1(void) {
	if (!gemu_scroll_enable[1]) {
		return;
	}
	int x,y, gx, flip;
	int element;
	int modtransx;
	
	modtransx = g.CPS.Scroll1X & 0x7;
	glPushMatrix();
	glTranslatef((-modtransx * TILE_SIZE_SCR1 / 8), 0, -0.1f);
	GLfloat master = (gemu.PalScroll1[0][0] & PALETTE_MASK_BRIGHTNESS) / TILE_BRIGHT_TO_FLOAT;
	glColor3f(master, master, master);
	
	for(y=0;y<32;y++) {
        for(x=0;x<48;x++) {
			gx = x + ((g.CPS.Scroll1X >> 3 ) & 0x1f);
			
            element = SCROLL_DECODE_SCR1(gx, y);
			
			if (gemu.Tilemap_Scroll1[element][0] == TILE_BLANK_SCR1) {
				continue;
			}
			
			gemu_cache_scroll1(gemu.Tilemap_Scroll1[element][0],
							   gemu.Tilemap_Scroll1[element][1] & TILE_MASK_PALETTE);
			
			flip = 0;       // no flipping on SCR1?
            draw_gl_tile(x-24, y-16, flip, TILE_SIZE_SCR1);
        }   
    }  
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}
static void draw_object(void) {

#define DRAWTILE(TILE,PAL,FLIP,sx,sy)										\
gemu_cache_object(TILE, gemu.Tilemap_Object[i][3] & 0x1f);					\
glBegin(GL_POLYGON);														\
glTexCoord2f(flips[flip][0][0],flips[flip][0][1]);							\
glVertex3f(((sx+1) * TILE_SIZE_OBJ), ((sy+1) * TILE_SIZE_OBJ), 0.0f);						\
glTexCoord2f(flips[flip][1][0],flips[flip][1][1]);							\
glVertex3f((( sx ) * TILE_SIZE_OBJ), ((sy+1) * TILE_SIZE_OBJ), 0.0f);						\
glTexCoord2f(flips[flip][2][0],flips[flip][2][1]);							\
glVertex3f((( sx ) * TILE_SIZE_OBJ), (( sy ) * TILE_SIZE_OBJ), 0.0f);						\
glTexCoord2f(flips[flip][3][0],flips[flip][3][1]);							\
glVertex3f(((sx+1) * TILE_SIZE_OBJ), (( sy ) * TILE_SIZE_OBJ), 0.0f);						\
glEnd();
	
	if (!gemu_scroll_enable[0]) {
		return;
	}
	int i,j, blockx, blocky, pal, bys, bxs ;
	GLfloat x,y;
	int tile, flip;
	GLfloat master = (gemu.PalObject[0][0] & PALETTE_MASK_BRIGHTNESS) / TILE_BRIGHT_TO_FLOAT;
	glColor3f(master, 1.0, master);
	
	
	glEnable(GL_TEXTURE_2D);
	for (j=0; j<0x100; j++) {
		if(gemu.Tilemap_Object[j][3] == TILE_OBJECT_END_TAG) {	// End Tag
			break;
		}
	}
	for (i=j; i>=0; i--) {
		tile = gemu.Tilemap_Object[i][2];
		if (tile != 0) {
			pal    = gemu.Tilemap_Object[i][3] & 0x1f;
			flip   = (gemu.Tilemap_Object[i][3] &   0x60) >>  5;
			x      = ((short)gemu.Tilemap_Object[i][0] &  TILE_MASK_OFFSET) / 16.0;
			y      = ((short)gemu.Tilemap_Object[i][1] &  TILE_MASK_OFFSET) / 16.0;			
			x     -= 16.0;
			y	  -=  8.0;

			if (gemu.Tilemap_Object[i][3] & TILE_MASK_BLOCK) {
				// handle blocking
				blockx = ((gemu.Tilemap_Object[i][3] & TILE_MASK_BLOCK_X) >>  8)+1;
				blocky = ((gemu.Tilemap_Object[i][3] & TILE_MASK_BLOCK_Y) >> 12)+1;
				if (flip & 2) {
					// YFLIP
					if (flip & 1) {
						// X and Y FLIP
						for (bys = 0; bys < blocky; bys++) {
							for (bxs = 0; bxs < blockx; bxs++) {
								DRAWTILE(
										 (tile + (blockx - 1) - bxs + (0x10 * (blocky - 1 - bys))),
										 pal,
										 flip,
										 (x + bxs),
										 (y + bys)
										 );
							}
						}
					} else {
						// only YFLIP
						for (bys = 0; bys < blocky; bys++) {
							for (bxs = 0; bxs < blockx; bxs++) {
								DRAWTILE(
										 (tile + bxs + (0x10 * (blocky - 1 -bys))),
										 pal,
										 flip,
										 (x + bxs),
										 (y + bys)
										 );
							}
						}
					}
				} else {
					if (flip & 1) {
						// XFLIP
						for (bys = 0; bys < blocky; bys++) {
							for (bxs = 0; bxs < blockx; bxs++) {
								DRAWTILE(
									(tile + (blockx - 1) - bxs + (0x10 * bys)),
									pal,
									flip,
								    (x + bxs),
									(y + bys)
								);
							}
						}
					} else {
						// NOFLIP
						for (bys = 0; bys < blocky; bys++) {
							for (bxs = 0; bxs < blockx; bxs++) {
								DRAWTILE(
										 (tile + bxs + (0x10 * bys)),
										 pal,
										 flip,
										 (x + bxs),
										 (y + bys)
										 );
							}
						}
					}
				}
			} else {
				// only one tile
				
				DRAWTILE(tile,pal,flip,x,y);				
			}
		}
	}
#undef DRAWTILE
}
static void draw_scroll2(void) {
	int x,y, yloop, flip, tx, ty, tiletx, tilety;
	int scr2x;
	int record;
	float scrollbot, scrolltop;

	if (!gemu_scroll_enable[2]) {
		return;
	}
	/* Draw Scroll2 */
	glPushMatrix();
	scr2x = g.CPS.Scroll2X;;	
	glTranslatef(-(scr2x & 0xf) / 16.0 * TILE_SIZE_SCR2, ((g.CPS.Scroll2Y & 0xf) / 16.0 * TILE_SIZE_SCR2)  , 0);

	tilety = g.CPS.Scroll2Y / 16;
	tiletx = scr2x          / 16;

	GLfloat master = (gemu.PalScroll2[0][0] & PALETTE_MASK_BRIGHTNESS) / TILE_BRIGHT_TO_FLOAT;
	glColor3f(master, master, master);
	
	for(yloop=-1;yloop<16;yloop++) {	
		y = yloop;
		scrolltop = gemu.RowScroll2[ (yloop + 48) * 16 ] / 16.0;
		scrollbot = gemu.RowScroll2[ (yloop + 47) * 16 ] / 16.0;
        for(x=-6;x<39;x++) {
			ty = (y + (48 - tilety)) & 0x3f;
			tx = (x + tiletx) & 0x3f;
            
			record = SCROLL_DECODE_SCR2(tx,ty);
			if (gemu.Tilemap_Scroll2[record][0] == TILE_BLANK_SCR2) {
                // Blank tile in SCR2
				continue;
			}
				
			if(ty == 0) {continue;}
			gemu_cache_scroll2(gemu.Tilemap_Scroll2[record][0],
							   gemu.Tilemap_Scroll2[record][1] & TILE_MASK_PALETTE);
			
			flip = (gemu.Tilemap_Scroll2[record][1] & TILE_MASK_FLIP) >> 5;
			
            draw_gl_tile(x-12, y-8, flip, TILE_SIZE_SCR2);
        }   
    }  
	
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}
static void draw_scroll3(void) {
	int x,y, flip, tx, ty, tilety, tiletx;
	float sx, sy;

	if (!gemu_scroll_enable[3]) {
		return;
	}
	glPushMatrix();
	glTranslatef(-(g.CPS.Scroll3X & 0x1f) / 32.0 * TILE_SIZE_SCR3, (g.CPS.Scroll3Y & 0x1f) / 32.0 * TILE_SIZE_SCR3, 0);
	tilety = g.CPS.Scroll3Y / 32;
	tiletx = g.CPS.Scroll3X / 32;
	GLfloat master = (gemu.PalScroll3[0][0] & PALETTE_MASK_BRIGHTNESS) / TILE_BRIGHT_TO_FLOAT;
	glColor3f(master, master, master);
	
	for(y=-1;y<9;y++) {
        for(x=-1;x<13;x++) {
			ty = (y + (56 - tilety)) & 0x3f;
			tx = (x + tiletx) & 0x3f;
			if (ty == 0) {
				continue;
			}
			short record = SCROLL_DECODE_SCR3(tx, ty);
			if (gemu.Tilemap_Scroll3[record][0] == TILE_BLANK_SCR3) {
				continue;
			}
			gemu_cache_scroll3(gemu.Tilemap_Scroll3[record][0],
							   gemu.Tilemap_Scroll3[record][1] & TILE_MASK_PALETTE);
			
			
            sx = (float)x-6.0;		//(-20 + 12)
			sy = (float)y-4.0;
			flip = gemu.Tilemap_Scroll3[record][1] & TILE_MASK_FLIP;
			flip = flip >>5;
			
            draw_gl_tile(x-6, y-4, flip, TILE_SIZE_SCR3);
        }
    }    
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}	

void gfx_glut_drawgame(void) {
	GLfloat gShapeSize = 11.0f;
	//float angle;
	
	GLdouble xmin, xmax, ymin, ymax;
	// far frustum plane
	GLdouble zFar = -gCamera.viewPos.z + 15;		// was 8
	// near frustum plane clamped at 1.0
	GLdouble zNear = MIN (-gCamera.viewPos.z - gShapeSize * 0.5, 1.0);
	// window aspect ratio
	GLdouble aspect = gCamera.screenWidth / (GLdouble)gCamera.screenHeight;
	
	if (gemuCacheClear) {
		gemu_clear_cache();
	}
	
	//glClearColor(0.2, 0.4, 0.1, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (aspect > 1.0) {
		ymax = zNear * tan (gCamera.aperture * 0.5 * DTOR);
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;
	} else {
		xmax = zNear * tan (gCamera.aperture * 0.5 * DTOR);
		xmin = -xmax;
		ymin = xmin / aspect;
		ymax = xmax / aspect;
	}
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt (gCamera.viewPos.x, gCamera.viewPos.y, gCamera.viewPos.z,
			   gCamera.viewPos.x + gCamera.viewDir.x,
			   gCamera.viewPos.y + gCamera.viewDir.y,
			   gCamera.viewPos.z + gCamera.viewDir.z,
			   gCamera.viewUp.x, gCamera.viewUp.y ,gCamera.viewUp.z);
	
	glRotatef (gTrackBallRotation[0], gTrackBallRotation[1], gTrackBallRotation[2], gTrackBallRotation[3]);
	glRotatef (gWorldRotation[0], gWorldRotation[1], gWorldRotation[2], gWorldRotation[3]);
	
	GLfloat lightPosition[] = {lightX, 1, 3, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	
	gframecnt++;
    finish = clock() ;
	duration += (double)(finish - start) / CLOCKS_PER_SEC ;
	frames ++ ;
	FPS = frames / duration ;
	start = clock() ;
	

	glScalef(0.3, -0.3, 0.3);
	
	glClearColor (0.2f, 0.1f, 0.0f, 1.0f);	// clear the surface
	glClear (GL_COLOR_BUFFER_BIT);
	
	
	if (enable_lighting) {
		glEnable(GL_LIGHTING);
	} else {
		glDisable(GL_LIGHTING);
	}
	
	glEnable(GL_BLEND);
	if (showTextures) {
		glEnable(GL_TEXTURE_2D);
	} else {
		glDisable(GL_TEXTURE_2D);
	}
	
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	
	if (texture_mode) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	} else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	
	
	SCROLL[(g.CPS.DispEna >>   6) & 3]();
	SCROLL[(g.CPS.DispEna >>   8) & 3]();
	SCROLL[(g.CPS.DispEna >>  10) & 3]();
	SCROLL[(g.CPS.DispEna >>  12) & 3]();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_TEXTURE_2D);
	drawGLText(gCamera);
}

void gCameraReset(void) {
	gCamera.aperture = 10;
	gCamera.focalLength = 15;
	gCamera.rotPoint = gOrigin;
	
	gCamera.viewPos.x = 0.0;
	gCamera.viewPos.y = 0.0;
	gCamera.viewPos.z = -gCamera.focalLength;
	gCamera.viewDir.x = -gCamera.viewPos.x; 
	gCamera.viewDir.y = -gCamera.viewPos.y; 
	gCamera.viewDir.z = -gCamera.viewPos.z;
	
	gCamera.viewUp.x = 0;  
	gCamera.viewUp.y = 1; 
	gCamera.viewUp.z = 0;
}

void drawGLText(recCamera cam) {
	char outString [256] = "";
	const char *endstring;
	GLint matrixMode;
	GLint vp[4];
	GLint lineSpacing = 13;
	GLint line = 0;
	GLint startOffest = 7;
	
	
	glGetIntegerv(GL_VIEWPORT, vp);
	glViewport(0, 0, cam.screenWidth, cam.screenHeight);
	
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);

	glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);  glPushMatrix(); glLoadIdentity();
	
	glScalef(2.0f / cam.screenWidth, -2.0f / cam.screenHeight, 1.0f);
	glScalef(gWimpScale, gWimpScale, 1.0f);
	
	glTranslatef(-cam.screenWidth / 2.0f, -cam.screenHeight / 2.0f, 0.0f);
	
	
	// draw 
	glDisable(GL_LIGHTING);
	glColor3f (1.0, 1.0, 1.0);
	if (gShowInfo && camera) {
		
		sprintf (outString, "Camera Position: (%0.1f, %0.1f, %0.1f)", gCamera.viewPos.x, gCamera.viewPos.y, gCamera.viewPos.z);
		drawGLString (10, cam.screenHeight - (lineSpacing * line++) - startOffest, outString);
		sprintf (outString, "Trackball Rotation: (%0.1f, %0.2f, %0.2f, %0.2f)", gTrackBallRotation[0], gTrackBallRotation[1], gTrackBallRotation[2], gTrackBallRotation[3]);
		drawGLString (10, cam.screenHeight - (lineSpacing * line++) - startOffest, outString);
		sprintf (outString, "World Rotation: (%0.1f, %0.2f, %0.2f, %0.2f)", gWorldRotation[0], gWorldRotation[1], gWorldRotation[2], gWorldRotation[3]);
		drawGLString (10, cam.screenHeight - (lineSpacing * line++) - startOffest, outString);
		sprintf (outString, "Aperture: %0.1f", gCamera.aperture);
		drawGLString (10, cam.screenHeight - (lineSpacing * line++) - startOffest, outString);
		sprintf (outString, "Focus Distance: %0.1f", gCamera.focalLength);
		drawGLString (10, cam.screenHeight - (lineSpacing * line++) - startOffest, outString);
	}
	
	
	if (gGameInWindow) {
		DrawFrameAndTitle(gameView, gCamera.screenHeight);		
		DrawFrameAndTitle(dummyWindow->view, gCamera.screenHeight);
	}
	
	
	
	infoView.rect.left   = 180;
	infoView.rect.top    =  20;
	infoView.rect.width  = 580;
	infoView.rect.height = 180;
	if (gShowHelp) {
		glColor4f(0.3, 0.3, 0.3, 0.7);		
		DrawMyFrame(&infoView);
		glTranslatef(infoView.rect.left,infoView.rect.top, 0.0);
		glEnable(GL_SCISSOR_TEST);
		glScissor(infoView.rect.left, cam.screenHeight - (infoView.rect.top + infoView.rect.height), infoView.rect.width, infoView.rect.height);
		
		glColor3f (1.0, 1.0, 1.0);
		line = 1;
		//		sprintf(outString, "%d %d %d %d",
		//				gframecnt,
		//				gsupertaskcnt,
		//				gtimercount,
		//				gdrawallcnt);
		//		drawGLString (10, (lineSpacing * line++) + startOffest, outString);		
		sprintf (outString, "DISPENA %04x RTR %x/%x diff %x LS %02x",
				 g.CPS.DispEna,
				 g.Player1.RoughTimeRemain,
				 g.Player2.RoughTimeRemain,
				 g.Player1.Difficulty,
				 g.libsplatter
				 );
		drawGLString (10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "%4.0f FPS", 
				 FPS);
		drawGLString (10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "%d(%d) %d(%d) %d(%d) %d(%d) %d(%d)",
				 g.mode0,g.timer0,g.mode1,g.timer1,g.mode2,g.timer2, g.mode3, g.timer3, g.mode4, g.timer4);
		drawGLString(10, (lineSpacing * line++) + startOffest, outString);
		endstring = g.Player1.AnimFlags & 0x8000 ? "END" : "   ";
		sprintf (outString, "P1 %x/%x/%x/%x X %04d Y %04d FL %d ST %d", 
				 g.Player1.mode0,
				 g.Player1.mode1,
				 g.Player1.mode2,
				 g.Player1.mode3,
				 g.Player1.X.part.integer,
				 g.Player1.Y.part.integer,
				 g.Player1.Flip,
				 g.Player1.Step
				 );
		drawGLString(10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "P2 %x/%x/%x/%x X %04d Y %04d FL %d ST %d AF %04x %3d", 
				 g.Player2.mode0,
				 g.Player2.mode1,
				 g.Player2.mode2,
				 g.Player2.mode3,
				 g.Player2.X.part.integer,
				 g.Player2.Y.part.integer,
				 g.Player2.Flip,
				 g.Player2.Step,
				 g.Player2.AnimFlags,
				 g.Player2.Timer
				 );
		drawGLString(10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "AI P1 Def %d Agg %d M1 %d M2 %d ST %d P1 0x%02x P2 0x%02x T1 %3d",
				 g.Player1.AIForceDefensive,
				 g.Player1.AIAgressive,
				 g.Player1.AIMode1,
				 g.Player1.AIMode2,
				 g.Player1.AIStrategy,
				 g.Player1.AIParam1,
				 g.Player1.AIParam2,
				 g.Player1.AITimer
				 );
		drawGLString(10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "AI P2 Def %d Agg %d M1 %d M2 %d ST %d P1 0x%02x P2 0x%02x T1 %3d",
				 g.Player2.AIForceDefensive,
				 g.Player2.AIAgressive,
				 g.Player2.AIMode1,
				 g.Player2.AIMode2,
				 g.Player2.AIStrategy,
				 g.Player2.AIParam1,
				 g.Player2.AIParam2,
				 g.Player2.AITimer
				 );
		drawGLString(10, (lineSpacing * line++) + startOffest, outString);
				sprintf	(outString, "SCR1: X:%04x Y:%04x (%04x/%04x) 1E:%04x 20:%04x FADE %x",
						 gstate_Scroll1.XPI,
						 gstate_Scroll1.YPI,
						 gstate_Scroll1.Offset,
						 gstate_Scroll1.OffMask,
						 gstate_Scroll1.x001e,
						 gstate_Scroll1.Index,
						 gemu.PalScroll1[0][0] & PALETTE_MASK_BRIGHTNESS
						 );
				drawGLString(10, (lineSpacing * line++) + startOffest, outString);
				sprintf	(outString, "SCR2: X:%04x Y:%04x (%04x/%04x) 1E:%04x 20:%04x FADE %x",
						 gstate_Scroll2.XPI,
						gstate_Scroll2.YPI,
						 gstate_Scroll2.Offset,
						 gstate_Scroll2.OffMask,
						 gstate_Scroll2.x001e,
						 gstate_Scroll2.Index,
						 gemu.PalScroll2[0][0] & PALETTE_MASK_BRIGHTNESS
						 );
				drawGLString(10, (lineSpacing * line++) + startOffest, outString);
		sprintf	(outString, "SCR3: X:%04x Y:%04x (%04x/%04x) 1E:%04x 20:%04x FADE %x",
				 g.CPS.Scroll3X,
				 g.CPS.Scroll3Y,
				 gstate_Scroll3.Offset,
				 gstate_Scroll3.OffMask,
				 gstate_Scroll3.x001e,
				 gstate_Scroll3.Index,
				 gemu.PalScroll3[0][0] & PALETTE_MASK_BRIGHTNESS
				 );
		drawGLString(10, (lineSpacing * line++) + startOffest, outString);
		sprintf	(outString, "SCRL: X:%04x",gemu.RowScroll2[0]);
		drawGLString(10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "FadeOutComplete %d FadeBusy %d",
						 Exec.FadeOutComplete,
						 es.FadeBusy
						 );
		drawGLString(10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "Coin1 minor:%02d holdoff:%02d bits:0x%02x coinstat:0x%04x",
				 g.coinslot1.count_minor,
				 g.coinslot1.debounce_timer,
				 g.coinslot1.shifted_bits,
				 g.CoinStatus);
		drawGLString(10, (lineSpacing * line++) + startOffest, outString);
		
				 
		
		
		glTranslatef(-infoView.rect.left, -infoView.rect.top, 0.0);
		glDisable(GL_SCISSOR_TEST);
		
	}
	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(matrixMode);
	glViewport(vp[0], vp[1], vp[2], vp[3]);		// restore the saved viewport
}


void drawGLString(GLfloat x, GLfloat y, char *string){
	int len, i;
	
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
	}
}



void mouseDolly(int pointx, int pointy) {
	if (gDolly) {
		GLfloat dolly = (gDollyPanStartPoint[1] - pointy) * -gCamera.viewPos.z / 200.0f;
		GLfloat eyeRelative = gCamera.eyeSep / gCamera.focalLength;
		gCamera.focalLength += gCamera.focalLength / gCamera.viewPos.z * dolly; 
		if (gCamera.focalLength < 1.0)
			gCamera.focalLength = 1.0;
		gCamera.eyeSep = gCamera.focalLength * eyeRelative;
		gCamera.viewPos.z += dolly;
		if (gCamera.viewPos.z == 0.0) // do not let z = 0.0
			gCamera.viewPos.z = 0.0001;
		gDollyPanStartPoint[0] = pointx;
		gDollyPanStartPoint[1] = pointy;
	}
}
void mousePan(int pointx, int pointy) {
	if (gPan) {
		GLint panX = (gDollyPanStartPoint[0] - pointx) / (900.0f / -gCamera.viewPos.z);
		GLint panY = (gDollyPanStartPoint[1] - pointy) / (900.0f / -gCamera.viewPos.z);
		gCamera.viewPos.x -= panX;
		gCamera.viewPos.y -= panY;
		gDollyPanStartPoint[0] = pointx;
		gDollyPanStartPoint[1] = pointy;
	}
}
void mouseTrackball(int pointx, int pointy) {
	if (gTrackBall) {
		rollToTrackball (pointx, pointy, gTrackBallRotation);
	}
}

void gfx_glut_mousedown(int pointx, int pointy) {
	if (gDolly) { // if we are currently dollying, end dolly
		mouseDolly(pointx, pointy);
		gDolly = GL_FALSE;
		gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
	} else if (gPan) {
		mousePan(pointx, pointy);
		gPan = GL_FALSE;
		gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
	}
	startTrackball (pointx, pointy, 0, 0, gCamera.screenWidth, gCamera.screenHeight);
	gTrackBall = GL_TRUE;
}	

void gfx_glut_rightmousedown(int px, int py) {
	if (gTrackBall) {// if we are currently trackballing, end trackball
		gTrackBall = GL_FALSE;
		rollToTrackball (px, py, gTrackBallRotation);
		if (gTrackBallRotation[0] != 0.0)
			addToRotationTrackball (gTrackBallRotation, gWorldRotation);
		gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
	} else if (gPan) {
		mousePan(px, py);
		gPan = GL_FALSE;
		gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
	}
	gDollyPanStartPoint[0] = px;
	gDollyPanStartPoint[1] = py;
	gDolly = GL_TRUE;
}

void gfx_glut_mousedragged(int px, int py) {
	mouseTrackball(px, py);
}
void gfx_glut_rightmousedragged(int px, int py) {
	mouseDolly(px, py);
}
void gfx_glut_mouseup(int px, int py) {
	gTrackBall = GL_FALSE;
	rollToTrackball (px, py, gTrackBallRotation);
	if (gTrackBallRotation[0] != 0.0)
		addToRotationTrackball (gTrackBallRotation, gWorldRotation);
	gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;	
}
void gfx_glut_rightmouseup(int px, int py) {
	mouseDolly(px, py);
	gDolly = GL_FALSE;
	gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
}
