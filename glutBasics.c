/*
 * 
 *		glutBasics.c
 *		MustardTiger2 GLUT frontend
 *
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
#include "task.h"
#include "structs.h"
#include "gfx_glut.h"
#include "lib.h"
#include "game.h"
#include "sf2io.h"
#include "gemu.h"
#include "workarounds.h"


Game g;

extern struct inputs gInputs;

//#define DEBUG

extern CPSGFXEMU gemu;
int time_wait=12;

typedef struct {
   GLdouble x,y,z;
} recVec;

int gMainWindow = 0;

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

void init (void) {
    manual_init();

	//glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);    
	glFrontFace(GL_CCW);
    
	glColor3f(1.0,1.0,1.0);
	gCameraReset ();
	
	glPolygonOffset (1.0, 1.0);
	SetLighting(4);
	glEnable(GL_LIGHTING);
}

void reshape (int w, int h) {
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	gfx_glut_reshape(w, h);
	glutPostRedisplay();
}
void maindisplay(void) {
    gfx_glut_drawgame();
	glutSwapBuffers();
}
void mouse (int button, int state, int x, int y) {
	switch (button) {
		case GLUT_LEFT_BUTTON:
			switch (state) {
				case GLUT_DOWN:
					gfx_glut_mousedown(x, y);
					break;
				case GLUT_UP:
					gfx_glut_mouseup(x, y);
					break;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			switch (state) {
				case GLUT_DOWN:
					gfx_glut_rightmousedown(x, y);
					break;
				case GLUT_UP:
					gfx_glut_rightmouseup(x, y);
					break;
			}
			break;
	}
}
void mouseMotion(int x, int y) {
	gfx_glut_mousedragged(x, y);
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
        
#ifdef REDHAMMER
        case 'k':
            g.Player2.Energy = -1;      break;
        case 'K':
            g.Player1.Energy = -1;      break;
#endif
			
	}
}
	
void key(unsigned char inkey, int px, int py){
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
void timerFunc(int value) {
    task_timer();
  
    glutPostRedisplay();
    glutTimerFunc(time_wait, timerFunc, 0);
}
int main (int argc, const char * argv[])
{
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
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
	glutMotionFunc(mouseMotion);
	glutTimerFunc(40, timerFunc, 0);
    glutMainLoop();
    return 0;
}
