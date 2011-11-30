/* gfxlib.c functions in here are loose emulations of CPS graphics */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

#include "particle.h"

extern Game g;

#include "glwimp.h"
#include "gemu.h"
#include "gfxlib.h"
#include "gfx_glut.h"
#include "trackball.h"


extern CPSGFXEMU gemu;
extern GLfloat gWimpScale;
extern GState gstate_Scroll1;
extern GState gstate_Scroll2;
extern GState gstate_Scroll3;

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

#define XFACT 0.2
#define YFACT 0.2
#define XFACT_SCR3 0.4
#define YFACT_SCR3 0.4
#define DTOR 0.0174532925

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

struct texture_cache_t {
	GLuint text_scr1[0x10000];
	GLuint text_scr2[0x10000];
	GLuint text_scr3[0x10000];
	GLuint text_obj[0x10000][2];
} TC;
int gemuCacheClear;

extern GLubyte ubImage[];

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
	for (i=0; i<0x10000; ++i) {
		if (TC.text_scr1[i]) {
			glDeleteTextures(1, &TC.text_scr1[i]);
			TC.text_scr1[i] = 0;
		}			
	}
	for (i=0; i<0x10000; ++i) {
		if (TC.text_scr2[i]) {
			glDeleteTextures(1, &TC.text_scr2[i]);
			TC.text_scr2[i] = 0;
		}			
	}
	for (i=0; i<0x10000; ++i) {
		if (TC.text_scr3[i]) {
			glDeleteTextures(1, &TC.text_scr3[i]);
			TC.text_scr3[i] = 0;
		}			
	}
	for (i=0; i<0x10000; ++i) {
		if (TC.text_obj[i][0]) {
			glDeleteTextures(1, &TC.text_obj[i][0]);
			TC.text_obj[i][0] = 0;
		}			
	}
	gemuCacheClear = FALSE;
}
	
	
	

void gemu_cache_scroll1(u16 tile, short palette) {
	static GLuint tempmap[8][8][4];
	if (TC.text_scr1[tile] == 0) {
		gemu_readtile_scroll1(tile);
		gemu_colortile_scroll1(palette, tempmap);
		glGenTextures(1, &TC.text_scr1[tile]);
		if (&TC.text_scr1[tile]==0) {
			panic(999);
		}
		glBindTexture(GL_TEXTURE_2D, TC.text_scr1[tile]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempmap);
	} else {
		glBindTexture(GL_TEXTURE_2D, TC.text_scr1[tile]);
	}
}
void gemu_cache_scroll2(u16 tile, short palette) {
	static GLubyte tempmap[16][16][4];
	if (TC.text_scr2[tile] == 0) {
		gemu_readtile_scroll2(tile);
		gemu_colortile_scroll2(palette, tempmap[0][0]);
		glGenTextures(1, &TC.text_scr2[tile]);
		if (&TC.text_scr2[tile]==0) {
			panic(999);
		}
		glBindTexture(GL_TEXTURE_2D, TC.text_scr2[tile]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempmap);
	} else {
		glBindTexture(GL_TEXTURE_2D, TC.text_scr2[tile]);
	}
}
void gemu_cache_scroll3(u16 tile, short palette) {
	static GLuint tempmap[32][32][4];
	if (TC.text_scr3[tile] == 0) {
		gemu_readtile_scroll3(tile);
		gemu_colortile_scroll3(palette, tempmap);
		glGenTextures(1, &TC.text_scr3[tile]);
		if (&TC.text_scr3[tile]==0) {
			panic(999);
		}
		glBindTexture(GL_TEXTURE_2D, TC.text_scr3[tile]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempmap);
	} else {
		glBindTexture(GL_TEXTURE_2D, TC.text_scr3[tile]);
	}
}
void gemu_cache_object(u16 tile, short palette) {
	static GLuint tempmap[16][16][4];
	if (TC.text_obj[tile][0] == tile && TC.text_obj[tile][1] != palette) {
		glDeleteTextures(1, &TC.text_obj[tile][0]);
		TC.text_obj[tile][0] = 0;
	}
	if (TC.text_obj[tile][0] == 0) {
		gemu_readtile(tile);
		gemu_colortile2(palette, tempmap);
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

// Color a 16x16 tile, pack into a GLubyte array for use as a GL texture
void gemu_colortile2(short palette, GLubyte *img) {
    int u,v, master;
	
    for(u=0; u<16; u++) {
        for(v=0; v<16; v++) {
            if(gemu.tile[u][v] == 0xf) {
                /* transparent pixel */
                *img++ = 0;   /* red */
                *img++ = 255; /* green */
                *img++ = 0;   /* blue */
                *img++ = ALPHA_TRANS; /* alpha */
            } else {                
                /* convert 4bit RGB + brightness to 8-bit RGBA */
                if(gemu.FadeEnable) {
                    master = (17 * (gemu.PalObject[palette][ gemu.tile[u][v] ])>>12) / 15;
                } else {
                    master = 17;    /* 4-bit to 8-bit conversion */
                }
                APPEND_RGBA(gemu.PalObject);
            }        
        }
    }
}  


void gemu_colortile_scroll2(short palette, GLubyte *img) {
    int u,v, master;
	
    for(u=0; u<16; u++) {
        for(v=0; v<16; v++) {
            if(gemu.tile[u][v] == 0xf) {
                /* transparent pixel */
                *img++ = 0;   /* red */
                *img++ = 255; /* green */
                *img++ = 0;   /* blue */
                *img++ = ALPHA_TRANS; /* alpha */
            } else {                
                /* convert 4bit RGB + brightness to 8-bit RGBA */
                if(gemu.FadeEnable) {
                    master = (17 * (gemu.PalScroll2[palette][ gemu.tile[u][v] ])>>12) / 15;
                } else {
                    master = 17;    /* 4-bit to 8-bit conversion */
                }
                APPEND_RGBA(gemu.PalScroll2);
            }        
        }
    }
}
void gemu_colortile_scroll3(short palette, GLubyte *img) {
    int u,v, master;
	
    for(u=0; u<32; u++) {
        for(v=0; v<32; v++) {
            if(gemu.tile[u][v] == 15) {
                /* transparent pixel */
                *img++ = 0;   /* red */
                *img++ = 255; /* green */
                *img++ = 0;   /* blue */
                *img++ = ALPHA_TRANS; /* alpha */
            } else {                
                /* convert 4bit RGB + brightness to 8-bit RGBA */
                if(gemu.FadeEnable) {
                    master = (17 * (gemu.PalObject[palette][ gemu.tile[u][v] ])>>12) / 15;
                } else {
                    master = 17;    /* 4-bit to 8-bit conversion */
                }
                APPEND_RGBA(gemu.PalScroll3);
            }        
        }
    }
}        
void gemu_colortile_scroll1(short palette, GLubyte *img) {
    int u,v, master;
	
    for(u=0; u<8; u++) {
        for(v=0; v<8; v++) {
            if(gemu.tile[u][v] == 0xf) {
                /* transparent pixel */
                *img++ = 0;   /* red */
                *img++ = 255; /* green */
                *img++ = 0;   /* blue */
                *img++ = ALPHA_TRANS; /* alpha */
            } else {                
                /* convert 4bit RGB + brightness to 8-bit RGBA */
                if(gemu.FadeEnable) {
                    master = (17 * (gemu.PalScroll1[palette][ gemu.tile[u][v] ])>>12) / 15;
                } else {
                    master = 17;    /* 4-bit to 8-bit conversion */
                }
                APPEND_RGBA(gemu.PalScroll1);
            }
        }
    }
}        


void gfx_glut_init(void) {
	int i;
	
    gfxrom=fopen( "sf2gfx.bin", "r" );
    if(gfxrom == NULL) {
        printf("fatal: couldn't open graphics ROM in ");
		system("/bin/pwd");
        exit(0);
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
	gWorldRotation[0] = 155.0;
	gWorldRotation[1] = 0.0;
	gWorldRotation[2] = -1.0;
	gWorldRotation[3] = 0.0;
	
	start = clock();

}
void gemu_readtile(u16 tileid) {          /* read a 16x16 tile */
    unsigned char pixbit[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    int u, v;
    unsigned char buf[4];
    int tileaddr = tileid * 0x80;           /* xx use OBJECT_TILE_SIZE */
    
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
    unsigned char pixbit[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    int u, v;
    unsigned char buf[4];
	//if(tileid == 0x2800) {tileid = 0x2804;}
	
    int tileaddr = (tileid * 0x40) + 0x400000;           /* xx use OBJECT_TILE_SIZE */
	    
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
    unsigned char pixbit[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    int u, v;
    unsigned char buf[4];
	//if(tileid == 0x2800) {tileid = 0x2804;}
	
    int tileaddr = (tileid * 0x80) + 0x400000;           /* xx use OBJECT_TILE_SIZE */

    
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
	
	int tileaddr = (tileid * 0x200) + 0x400000;           /* xx use OBJECT_TILE_SIZE */
	
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

static void draw_scroll1(void) {
	if (!gemu_scroll_enable[1]) {
		return;
	}
	int x,y, gx, flip;
	short sx, sy;
	int element;
	int modtransx;
	
	modtransx = g.CPS.Scroll1X & 0x7;
	glPushMatrix();
	glTranslatef((-modtransx * XFACT * 1 / 8) - 4, 0, 0);
	GLfloat master = (gemu.PalScroll1[0][0] & 0xf000) / 61140.0;
	glColor3f(master, master, master);
	
	for(y=0;y<32;y++) {
        for(x=0;x<64;x++) {
			gx = x + ((g.CPS.Scroll1X >> 3 ) & 0x1f);
			
			element = ((y & 0x20) << 8) + (gx << 5) + (y & 0x1f);
			
			if (gemu.Tilemap_Scroll1[element][0] == 0x4000) {
				continue;
			}
			
			gemu_cache_scroll1(gemu.Tilemap_Scroll1[element][0],
							   gemu.Tilemap_Scroll1[element][1] & 0x1f);
			
			flip = 0;
            
			sx = x-1.0; sy=y-12;
			
            glBegin(GL_POLYGON);
            glTexCoord2f(flips[flip][0][0],flips[flip][0][1]);
			glVertex3f((sx * XFACT)+XFACT/2.0f, (sy * YFACT) + YFACT / 2.0f, -4.1f);
			glTexCoord2f(flips[flip][1][0],flips[flip][1][1]);
            glVertex3f((sx * XFACT)-XFACT/2.0f, (sy * YFACT) + YFACT / 2.0f, -4.1f);        
			glTexCoord2f(flips[flip][2][0],flips[flip][2][1]);
            glVertex3f((sx * XFACT)-XFACT/2.0f, (sy * YFACT) - YFACT / 2.0f, -4.1f);
            glTexCoord2f(flips[flip][3][0],flips[flip][3][1]);
            glVertex3f((sx * XFACT)+XFACT/2.0f, (sy * YFACT) - YFACT / 2.0f, -4.1f);
            glEnd();
			
        }   
    }  
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}
static void draw_object(void) {
	if (!gemu_scroll_enable[0]) {
		return;
	}
	int i,j;
	GLfloat x,y;
	int tile, flip;
	GLfloat master = (gemu.PalObject[0][0] & 0xf000) / 61140.0;
	glColor3f(master, 1.0, master);
	
	
	glEnable(GL_TEXTURE_2D);
	for (j=0; j<0x100; j++) {
		if(gemu.Tilemap_Object[j][3] == 0xff00) {	// End Tag
			break;
		}
	}
	for (i=j; i>=0; i--) {
		
		tile = gemu.Tilemap_Object[i][2];
		if (tile != 0) {
			
			
			flip = (gemu.Tilemap_Object[i][3] & 0x60) >> 5;
			x    = (short)gemu.Tilemap_Object[i][0] / 16.0;
			y    = ((short)gemu.Tilemap_Object[i][1] &  0x1ff) / 16.0;
			
			y -=  7.0;
			x -= 20.0;
			
			gemu_cache_object(tile, gemu.Tilemap_Object[i][3] & 0x1f);
			
			glBegin(GL_POLYGON);
			glTexCoord2f(flips[flip][0][0],flips[flip][0][1]);
			glVertex3f((x * XFACT)+XFACT/2.0f, (y * YFACT) + YFACT / 2.0f, -0.01f);
			glTexCoord2f(flips[flip][1][0],flips[flip][1][1]);
			glVertex3f((x * XFACT)-XFACT/2.0f, (y * YFACT) + YFACT / 2.0f, -0.01f);        
			glTexCoord2f(flips[flip][2][0],flips[flip][2][1]);
			glVertex3f((x * XFACT)-XFACT/2.0f, (y * YFACT) - YFACT / 2.0f, -0.01f);
			glTexCoord2f(flips[flip][3][0],flips[flip][3][1]);
			glVertex3f((x * XFACT)+XFACT/2.0f, (y * YFACT) - YFACT / 2.0f, -0.01f);
			glEnd();
		}
	}
	
}
static void draw_scroll2(void) {
	int x,y, yloop, flip;
	if (!gemu_scroll_enable[2]) {
		return;
	}
	/* Draw Scroll2 */
	glPushMatrix();
	glTranslatef(-g.CPS.Scroll2X / 128.0, g.CPS.Scroll2Y / 128.0, 0);
	
	GLfloat master = (gemu.PalScroll2[0][0] & 0xf000) / 61140.0;
	glColor3f(master, master, master);
	
	for(yloop=0;yloop<64;yloop++) {				//32
		y = yloop;
        for(x=0;x<64;x++) {
            short sx, sy;
			
			if (gemu.Tilemap_Scroll2[((y & 0x30) << 6) + (x * 16) + (y & 0x0f)][0] == 0x2800) {
				continue;
			}
			
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA,
            //             GL_UNSIGNED_BYTE, gemu.tiles_scroll2[x][y]);
            
			//glBindTexture(GL_TEXTURE_2D, gemu.textures_scroll2[x][y]);
			
			gemu_cache_scroll2(gemu.Tilemap_Scroll2[((y & 0x30) << 6) + (x * 16) + (y & 0x0f)][0],
							   gemu.Tilemap_Scroll2[((y & 0x30) << 6) + (x * 16) + (y & 0x0f)][1] & 0x1f);
			
			flip = gemu.Tilemap_Scroll2[((y & 0x30) << 6) + (x * 16) + (y & 0x0f)][1] & 0x60;
			flip = flip >> 5;
			
			if (yloop <32) {
				sx = x-64; sy = y-24;
			} else {
				sx = x; sy=y-40;
			}
			//glColor4f(1.0, 0.0, 0.0, 0.25);
            glBegin(GL_POLYGON);
            //glTexCoord2f(1.0, 1.0);
            glTexCoord2f(flips[flip][0][0],flips[flip][0][1]);
			glVertex3f((sx * XFACT)+XFACT/2.0f, (sy * YFACT) + YFACT / 2.0f, -0.1f);
			glTexCoord2f(flips[flip][1][0],flips[flip][1][1]);
            glVertex3f((sx * XFACT)-XFACT/2.0f, (sy * YFACT) + YFACT / 2.0f, -0.1f);        
			glTexCoord2f(flips[flip][2][0],flips[flip][2][1]);
            glVertex3f((sx * XFACT)-XFACT/2.0f, (sy * YFACT) - YFACT / 2.0f, -0.1f);
            glTexCoord2f(flips[flip][3][0],flips[flip][3][1]);
            glVertex3f((sx * XFACT)+XFACT/2.0f, (sy * YFACT) - YFACT / 2.0f, -0.1f);
            glEnd();
        }   
    }  
	
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}
static void draw_scroll3(void) {
	int x,y, flip;
	if (!gemu_scroll_enable[3]) {
		return;
	}
	glPushMatrix();
	glTranslatef(-g.CPS.Scroll3X / 128.0, ((g.CPS.Scroll3Y & 0x7ff) / 128.0) - 15, 0);
//	glTranslatef(-g.CPS.Scroll3X / 128.0, ((g.CPS.Scroll3Y & 0x7ff) / 128.0) - 22.5, 0);
	
	GLfloat master = (gemu.PalScroll3[0][0] & 0xf000) / 61140.0;
	glColor3f(master, master, master);
	
	//glScalef(0.2, 0.2, 0.2);
	for(y=0;y<64;y++) {
        for(x=0;x<64;x++) {
            short sx, sy;
			short record = ((y & 0x38)<<5) + (x << 3) + (y & 7);
			
			if (gemu.Tilemap_Scroll3[record][0] == 0x400) {
				continue;
				//gemu.Tilemap_Scroll3[record][0] = 4;
			} 			
			gemu_cache_scroll3(gemu.Tilemap_Scroll3[record][0],
							   gemu.Tilemap_Scroll3[record][1] & 0x1f);
			
			
            sx = x-12.0; 
			sy=y-3.5;
			flip = gemu.Tilemap_Scroll3[record][1] & 0x60;
			flip = flip >>5;
			
			// glColor4f(1.0, 0.0, 0.0, 0.25);
            glBegin(GL_POLYGON);
			glTexCoord2f(flips[flip][0][0],flips[flip][0][1]);
            glVertex3f((sx * XFACT_SCR3)+XFACT_SCR3/2.0f, (sy * YFACT_SCR3) + YFACT_SCR3 / 2.0f, -0.35f);
			glTexCoord2f(flips[flip][1][0],flips[flip][1][1]);
            glVertex3f((sx * XFACT_SCR3)-XFACT_SCR3/2.0f, (sy * YFACT_SCR3) + YFACT_SCR3 / 2.0f, -0.35f);        
			glTexCoord2f(flips[flip][2][0],flips[flip][2][1]);
            glVertex3f((sx * XFACT_SCR3)-XFACT_SCR3/2.0f, (sy * YFACT_SCR3) - YFACT_SCR3 / 2.0f, -0.35f);
			glTexCoord2f(flips[flip][3][0],flips[flip][3][1]);
            glVertex3f((sx * XFACT_SCR3)+XFACT_SCR3/2.0f, (sy * YFACT_SCR3) - YFACT_SCR3 / 2.0f, -0.35f);
            glEnd();
        }   
    }    
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}	

void gfx_glut_drawgame(void) {
	GLfloat gShapeSize = 11.0f;
	float angle;
	
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
	
	glClearColor(0.2, 0.4, 0.1, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
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
	
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);	// clear the surface
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
						 gemu.PalScroll1[0][0] & 0xf000
						 );
				drawGLString(10, (lineSpacing * line++) + startOffest, outString);
				sprintf	(outString, "SCR2: X:%04x Y:%04x (%04x/%04x) 1E:%04x 20:%04x FADE %x",
						 gstate_Scroll2.XPI,
						gstate_Scroll2.YPI,
						 gstate_Scroll2.Offset,
						 gstate_Scroll2.OffMask,
						 gstate_Scroll2.x001e,
						 gstate_Scroll2.Index,
						 gemu.PalScroll2[0][0] & 0xf000
						 );
				drawGLString(10, (lineSpacing * line++) + startOffest, outString);
				sprintf	(outString, "SCR3: X:%04x Y:%04x (%04x/%04x) 1E:%04x 20:%04x FADE %x",
						 g.CPS.Scroll3X,
						 g.CPS.Scroll3Y,
						 gstate_Scroll3.Offset,
						 gstate_Scroll3.OffMask,
						 gstate_Scroll3.x001e,
						 gstate_Scroll3.Index,
						 gemu.PalScroll3[0][0] & 0xf000
						 );
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