/*
 *  glutBasics.cpp
 *  GLUTTest
 *
 *  Created by GGS on June 17 2003.
 *  Copyright (c) 2003 Apple. All rights reserved.
 *
 

 */
 
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>

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

#include "trackball.h"

#include "sf2const.h"
#include "sf2types.h"
#include "sf2macros.h"
#include "gstate.h"

#include "task.h"
#include "structs.h"
#include "particle.h"
#include "player.h"
#include "sm.h"
#include "gfx_glut.h"
#include "lib.h"

#include "sf2io.h"
#include "gemu.h"
#include "workarounds.h"


Game g;

extern u16 *DSObjCur_g;
extern void *actlist_383ac;
extern void *actlist_4b03e;

extern struct inputs gInputs;

#define DEBUG

#define DTOR 0.0174532925
#define CROSSPROD(p1,p2,p3) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x
   
#ifndef MIN
	#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define XFACT 0.2
#define YFACT 0.2
#define XFACT_SCR3 0.4
#define YFACT_SCR3 0.4

extern CPSGFXEMU gemu;
short tid;
int scrolltimer=0;
int scroll_cursor=1;
int stage_cursor=0;
int time_wait=12;
GLubyte ubImage[65536];



enum {
	kTextureSize = 256
};

typedef struct {
   GLdouble x,y,z;
} recVec;


struct {
	float x;
	float y;
	int tileid;
} st[4] = {
	{ 0.0, 0.0,  0x04 },
	{ 1.0, 0.0,  0x05 },
	{ 0.0, 1.0,  0x14 },
	{ 1.0, 1.0,  0x15 },
}; 


typedef struct {
	recVec viewPos; // View position
	recVec viewDir; // View direction vector
	recVec viewUp; // View up direction
	recVec rotPoint; // Point to rotate about
	GLdouble focalLength; // Focal Length along view direction
	GLdouble aperture; // gCamera aperture
	GLdouble eyeSep; // Eye separation
	GLint screenWidth,screenHeight; // current window/screen height and width
} recCamera;

void reread_scroll2(void);

GLfloat gShapeSize = 11.0f;

GLint gDollyPanStartPoint[2] = {0, 0};
GLfloat gTrackBallRotation [4] = {0.0, 0.0, 0.0, 0.0};
GLboolean gDolly = GL_FALSE;
GLboolean gPan = GL_FALSE;
GLboolean gTrackBall = GL_FALSE;
GLfloat gWorldRotation [4] = {155.0, 0.0, -1.0, 0.0};

GLboolean gLines = GL_FALSE;
GLboolean gPolygons = GL_TRUE;

short drawscroll2 = 1;

GLfloat xtrans = 0.0;

recCamera gCamera;
recVec gOrigin = {0.0, 0.0, 0.0};

int gLastKey = ' ';

int gMainWindow = 0;

GLuint gPointList = 0;
GLuint gWireList = 0;
GLuint gSolidList = 0;


#pragma mark --- FPS ---

clock_t start, finish ;
double duration, frames, FPS ;


#pragma mark ---- Utilities ----


#pragma mark ---- Drawing ----

void SetLighting(unsigned int mode) {
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {90.0};

	GLfloat position[4] = {0.0,0.0,12.0,0.0};
//	GLfloat position[4] = {7.0,-7.0,12.0,0.0};

	GLfloat ambient[4]  = {0.5,0.5,0.5,1.0};
	GLfloat diffuse[4]  = {1.0,1.0,1.0,1.0};
	GLfloat specular[4] = {1.0,1.0,1.0,1.0};
	
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

	switch (mode) {
		case 0:
			break;
		case 1:
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
			break;
		case 2:
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
			break;
		case 3:
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
			break;
		case 4:
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
			break;
	}
	
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glEnable(GL_LIGHT0);
}

#pragma mark ---- GLUT callbacks ----

void init (void) {
	start = clock();

    manual_init();

	//glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);    
	glFrontFace(GL_CCW);
    

	glColor3f(1.0,1.0,1.0);

	
	gCameraReset ();
	
	glPolygonOffset (1.0, 1.0);
	SetLighting(4);
	//glEnable(GL_LIGHTING);
	//glScalef(1.0, -1.0, 1.0);
}

void reshape (int w, int h) {
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	gCamera.screenWidth = w;
	gCamera.screenHeight = h;
	glutPostRedisplay();
}



void maindisplay(void) {
	finish = clock() ;
	duration += (double)(finish - start) / CLOCKS_PER_SEC ;
	frames ++ ;
	FPS = frames / duration ;
	start = clock() ;

    gfx_glut_drawgame();
	glutSwapBuffers();
}


void mouse (int button, int state, int x, int y) {
    gfx_glut_mousedown(x, y);
}
void special(int key, int px, int py) {
	switch (key) {
		case GLUT_KEY_UP:
			gInputs.p10 |= 0x08;
			break;
		case GLUT_KEY_DOWN:
			gInputs.p10 |= 0x04;
			break;
		case GLUT_KEY_LEFT:
			gInputs.p10 |= 0x02;
			break;
		case GLUT_KEY_RIGHT:
			gInputs.p10 |= 0x01;
			break;
		default:
			break;
	}
}
void specialup(int key, int px, int py) {
	switch (key) {
		case GLUT_KEY_UP:
			gInputs.p10 &= 0xf7;
			break;
		case GLUT_KEY_DOWN:
			gInputs.p10 &= 0xfb;
			break;
		case GLUT_KEY_LEFT:
			gInputs.p10 &= 0xfd;
			break;
		case GLUT_KEY_RIGHT:
			gInputs.p10 &= 0xfe;
			break;
		default:
			break;
	}
}

void keyup(unsigned char inkey, int px, int py) {
	gLastKey = inkey;
	switch (inkey) {
		case 'q':		gInputs.p10 &=  0xef;	break;
		case 'w':		gInputs.p10 &=  0xdf;	break;
		case 'e':		gInputs.p10 &=  0xbf;	break;
		case 'a':		gInputs.p11 &= 0xfe;   break;
		case 's':		gInputs.p11 &= 0xfd;   break;
		case 'd':		gInputs.p11 &= 0xfb;   break;
		case '1':
			gInputs.in0 &= ~IPT_START1;	break;
		case '2':
			gInputs.in0 &= ~IPT_START2;	break;
		case '5':
			gInputs.in0 &= ~IPT_COIN1;	break;
		case '6':
			gInputs.in0 &= ~IPT_COIN2;	break;
			
	}
}
	
void key(unsigned char inkey, int px, int py){
  gLastKey = inkey;
	switch (inkey) {
		case 27:
			exit(0);
			break;
		case 'q':		gInputs.p10 |=  0x10;	break;
		case 'w':		gInputs.p10 |=  0x20;	break;
		case 'e':		gInputs.p10 |=  0x40;	break;
		case 'a':		gInputs.p11 |= 0x1;   break;
		case 's':		gInputs.p11 |= 0x2;   break;
		case 'd':		gInputs.p11 |= 0x4;   break;
		case '1':
			gInputs.in0 |= IPT_START1;	break;
		case '2':
			gInputs.in0 |= IPT_START2;	break;
		case '5':
			gInputs.in0 |= IPT_COIN1;	break;
		case '6':
			gInputs.in0 |= IPT_COIN2;	break;
			
	}
}

void spaceballmotion (int x, int y, int z) {
	long deadZone = 105;
	float scale = -gCamera.viewPos.z * 0.00000001f;
	if (abs (x) > deadZone) {
		GLfloat panX = abs (x) * x * scale;
		gCamera.viewPos.x += panX;
	}
	if (abs (y) > deadZone) {
		GLfloat panY = abs (y) * y * scale;
		gCamera.viewPos.y -= panY;
	}
	if (abs (z) > deadZone) {
		GLfloat dolly = abs (z) * z * scale;
		gCamera.viewPos.z += dolly;
		if (gCamera.viewPos.z == 0.0) // do not let z = 0.0
			gCamera.viewPos.z = 0.0001;
	}
	glutPostRedisplay();
}

void spaceballrotate (int rx, int ry, int rz) {
	long deadZone = 60;
	float rotation[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	// handle rotations about each respective axis
	if (abs(rx) > deadZone) {
		rotation[0] = abs (rx) * -rx * 0.0000008f;
		rotation[1] = 1.0f;
		rotation[2] = 0.0f;
		rotation[3] = 0.0f;
		addToRotationTrackball (rotation, gWorldRotation);
	}
	if (abs(ry) > deadZone) {
		rotation[0] = abs (ry) * ry * 0.0000008f;
		rotation[1] = 0.0f;
		rotation[2] = 1.0f;
		rotation[3] = 0.0f;
		addToRotationTrackball (rotation, gWorldRotation);
	}
	if (abs(rz) > deadZone) {
		rotation[0] = abs (rz) * -rz * 0.0000008f;
		rotation[1] = 0.0f;
		rotation[2] = 0.0f;
		rotation[3] = 1.0f;
		addToRotationTrackball (rotation, gWorldRotation);
	}
	glutPostRedisplay();
}

void timerFunc(int value) {
    task_timer();
  
    glutPostRedisplay();
    glutTimerFunc(time_wait, timerFunc, 0);
}


#pragma mark ---- main ----

int main (int argc, const char * argv[])
{
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // non-stereo for main window
	glutInitWindowPosition (300, 50);
	glutInitWindowSize (900, 600);
	gMainWindow = glutCreateWindow("sf2GL");

    init();					// standard GL init
    gfx_glut_init();
	
	glutIgnoreKeyRepeat(TRUE);

    glutReshapeFunc (reshape);
    glutDisplayFunc (maindisplay);
	glutKeyboardFunc (key);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc (special);
	glutSpecialUpFunc (specialup);
	glutMouseFunc (mouse);
	glutTimerFunc(40, timerFunc, 0);
	glutSpaceballMotionFunc(spaceballmotion);
	glutSpaceballRotateFunc(spaceballrotate);
    glutMainLoop();
    return 0;
}





