/* glwimp.c */

/* SwiftBeam 0.1 */


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

#include "sf2const.h"
#include "sf2types.h"
#include "sf2macros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"
#include "particle.h"
#include "player.h"

#include "gfx_glut.h"

#include "glwimp.h"

int gShowHelp;

extern Game g;
extern int gGameInWindow;

LBView *gDragView;
LBView rootView;

int gDragDeltaX;
int gDragDeltaY;
GLfloat gWimpScale = 1.0f;

void render_text_view (GLint window_width, GLint window_height) {
	
}

void drawObj1String(GLint x, GLint y, GLint height, char *string){
	int len, i;
	
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) {
		DrawTileObj(string[i] + SF2_TILE_OBJ_ASCII, 0, x + (i * (height - 2)), y, height, height);
	}
}

void initView (LBView *view, void *renderFunc, void *delegate, int left, int top, int width, int height) {
	view->rect.left   = left;
	view->rect.top    = top;
	view->rect.width  = width;
	view->rect.height = height;
	view->renderFunc  = renderFunc;
	view->delegate    = delegate;
}	

LBView *addSubView(LBView *view, void *renderFunc, void *delegate, int left, int top, int width, int height) {
	LBView *parent = view->subViews;

	if (parent == NULL) {
		view->subViews = calloc(sizeof(LBView), 1);
		initView(view->subViews, renderFunc, delegate, left, top, width, height);
		view->subViews->parent = view;
		return (LBView *)view->subViews;
	} else {
		while (parent->nextView != NULL) {
			parent = parent->nextView;
		}
		parent->nextView = calloc(sizeof(LBView), 1);
		initView(parent->nextView, renderFunc, delegate, left, top, width, height);
		parent->nextView->parent = view;
		return parent->nextView;
	}
}
LBView *addView(LBView *view) {
	LBView *parent = view->nextView;
	
	if (parent == NULL) {
		view->nextView = malloc(sizeof(LBView));
		return (LBView *)view->nextView;
	} else {
		while (parent->nextView != NULL) {
			parent = parent->nextView;
		}
		parent->nextView = malloc(sizeof(LBView));
		return parent->nextView;
	}
}

LBWindow *createWindow(int left, int top, int width, int height) {
	LBView *windowView, *iconView, *titleView, *areaView;
	LBWindow *window;
	
	windowView = addSubView(&rootView, &DrawNull, &ClickNull, left, top, width, height + WIN_TITLE_HEIGHT );
	iconView   = addSubView(windowView, &DrawWindowButton, &WindowButtonClicked, 2, 2, WIN_TITLE_HEIGHT - 4, WIN_TITLE_HEIGHT - 4);
	titleView  = addSubView(windowView, &DrawTitleBar, &DragBarClicked, WIN_TITLE_HEIGHT + 8, 2, 
								   width - (WIN_TITLE_HEIGHT + 8) - 4, 
								   WIN_TITLE_HEIGHT - 4);
	areaView   = addSubView(windowView, &DrawNull, &ClickNull, 0, WIN_TITLE_HEIGHT, width, height);		
	window = calloc(sizeof(LBWindow), 1);
	window->view = windowView;
	window->iconView = iconView;
	window->titleView = titleView;
	window->areaView = areaView;
	
	return window;
}

void destroyWindow(LBWindow *window) {
	
}


#pragma mark Draw Routines

void DrawPlainView(GLint left, GLint top, GLint width, GLint height) {
	
	glColor4f(0.3, 0.3, 0.3, 0.7);
	
	glBegin(GL_QUADS);
	glVertex2i(left,         top         );
	glVertex2i(left + width, top         );
	glVertex2i(left + width, top + height);
	glVertex2i(left,         top + height);
	glEnd();
	
	glColor4f(1.0, 1.0, 0.6, 1.0);
	
	glBegin(GL_LINES);
	glVertex2i(left,         top         );
	glVertex2i(left + width, top         );
	glVertex2i(left + width, top         );
	glVertex2i(left + width, top + height);
	glVertex2i(left + width, top + height);
	glVertex2i(left,         top + height);
	glVertex2i(left,         top + height);
	glVertex2i(left,         top         );
	glEnd();
	
	//glViewport(left, window_height - (top + height), width, height);
	//glTranslatef(0, top, 0.0);
	
	glColor3f (1.0, 1.0, 1.0);
}
void DrawMyFrame(LBView *view) {
	DrawPlainView(view->rect.left, view->rect.top, view->rect.width, view->rect.height);
}
void DrawTileObj(u16 tileid, u16 palette, int left, int top, int width, int height) {	
	glEnable(GL_TEXTURE_2D);
	gemu_cache_object(tileid, palette);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(left,         top         );
	glTexCoord2f(1.0, 0.0);
	glVertex2i(left+width,        top         );
	glTexCoord2f(1.0, 1.0);
	glVertex2i(left+width,        top + height);
	glTexCoord2f(0.0, 1.0);
	glVertex2i(left,         top + height);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glDisable(GL_TEXTURE_2D);
}
void DrawTileScr1(u16 tileid, u16 palette, int left, int top, int width, int height) {	
	glEnable(GL_TEXTURE_2D);
	gemu_cache_scroll1(tileid, palette);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(left,         top         );
	glTexCoord2f(1.0, 0.0);
	glVertex2i(left+width,   top         );
	glTexCoord2f(1.0, 1.0);
	glVertex2i(left+width,   top + height);
	glTexCoord2f(0.0, 1.0);
	glVertex2i(left,         top + height);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glDisable(GL_TEXTURE_2D);
}
void DrawTitleBar(LBView *view) {
	char outString [256] = "";

	sprintf (outString, "GAME WINDOW");
	drawObj1String (0, 1, WIN_TITLE_HEIGHT - 2, outString);
}	
void DrawWindowButton(LBView *view) {
	DrawTileObj(SF2_TILE_OBJ_PEACE, 0, 0, 0, view->rect.width, view->rect.height);	
}
void DrawNull(LBView *view) {
}

#pragma mark Event Callbacks

int WindowButtonClicked(LBView *view) {
	gGameInWindow = FALSE;
	return TRUE;
}
void mouseZoom(int x, int y) {
	printf("mouseZoom\n");
	gWimpScale = ((y - gDragDeltaY) / 10.0) + 1.0;
	glutPostRedisplay();
}
int RootViewClick(LBView *view, int button, int state, int x, int y, int scrx, int scry) {
	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) {
		glutMotionFunc(mouseZoom);
		gDragDeltaX = scrx;
		gDragDeltaY = scry;			
	} else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP)) {
		glutMotionFunc(NULL);
	}
	//printf("Clicked in the root view!\n");
	return TRUE;
}
int ClickNull(LBView *view) { return TRUE; }
void mouseDrag(int x, int y) {
	printf("mouseDrag\n");
	gDragView->rect.left = x - gDragDeltaX;
	gDragView->rect.top = y - gDragDeltaY;
	glutPostRedisplay();
}
int DragBarClicked(LBView *view, int button, int state, int x, int y, int scrx, int scry) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		glutMotionFunc(mouseDrag);
		printf("drag begins\n");
		gDragDeltaX = scrx - (view->parent->rect.left);
		gDragDeltaY = scry - (view->parent->rect.top);
		gDragView = view->parent;
	} else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		printf("drag ended \n");
		glutMotionFunc(NULL);
	}
	return TRUE;
}
int checkMouse(LBView *view, int button, int state, int x, int y, int scrx, int scry) {
	LBView *sv = view->subViews;
	while (sv != NULL) {
		if (x > sv->rect.left && x < (sv->rect.left + sv->rect.width) &&
			y > (sv->rect.top)  && y < (sv->rect.top + sv->rect.height)) {
			//now check recursively any subviews
			if (checkMouse(sv, button, state, x - sv->rect.left, y - sv->rect.top, scrx, scry)){
				// printf("a subView responded\n");
				return TRUE;
			} else {
				// no subview responded, so we do
				if (sv->delegate != NULL) {
					return sv->delegate(sv, button, state, x, y, scrx, scry);	
				}			
				
			}
		}
		sv = sv->nextView;
	}		
	
	if (x > view->rect.left && x < (view->rect.left + view->rect.width) &&
		y > view->rect.top && y < (view->rect.top + view->rect.height)) {
		//printf("Clicked in the view! %d %d\n", x, y);
		
		if (view->delegate != NULL) {
			return view->delegate(view, button, state, x, y, scrx, scry);	
		}
		
	}
	return FALSE;
}


void glwimp_root_resize (int screenWidth, int screenHeight) {
	rootView.rect.width = screenWidth;
	rootView.rect.height = screenHeight;
}
void glwimp_init(int screenWidth, int screenHeight) {
	initView(&rootView, &DrawNull, &RootViewClick, 0, 0, screenWidth, screenHeight );
	
}


void DrawFrameAndTitle(LBView *view, GLint screenHeight) {
	LBView *subView;
	
	glColor4f(1.0, 1.0, 1.0, 0.7);
	
	glBegin(GL_QUADS);
	glVertex2i(view->rect.left,                    view->rect.top         );
	glVertex2i(view->rect.left + view->rect.width, view->rect.top         );
	glVertex2i(view->rect.left + view->rect.width, view->rect.top + WIN_TITLE_HEIGHT);
	glVertex2i(view->rect.left,                    view->rect.top + WIN_TITLE_HEIGHT);
	glEnd();
	
	glColor4f(1.0, 1.0, 0.6, 1.0);
	
	glBegin(GL_LINES);
	glVertex2i(view->rect.left,         view->rect.top         );
	glVertex2i(view->rect.left + view->rect.width, view->rect.top         );
	glVertex2i(view->rect.left + view->rect.width, view->rect.top         );
	glVertex2i(view->rect.left + view->rect.width, view->rect.top + view->rect.height);
	glVertex2i(view->rect.left + view->rect.width, view->rect.top + view->rect.height);
	glVertex2i(view->rect.left,         view->rect.top + view->rect.height);
	glVertex2i(view->rect.left,         view->rect.top + view->rect.height);
	glVertex2i(view->rect.left,         view->rect.top         );
	glEnd();
	
	subView = view->subViews;
	glColor3f (1.0, 1.0, 1.0);

	while (subView != NULL) {
		glTranslatef(subView->rect.left + view->rect.left, subView->rect.top + view->rect.top, 0);
		glEnable(GL_SCISSOR_TEST);

		glScissor(subView->rect.left + view->rect.left, 
				  screenHeight - (subView->rect.top + view->rect.top + subView->rect.height), 
				  subView->rect.width,
				  subView->rect.height);
		
		//glClearColor(0.0, 1.0, 0.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
		glColor3f(1.0, 1.0, 1.0);
		
		subView->renderFunc(subView);
		glTranslatef(-(subView->rect.left + view->rect.left), -(subView->rect.top + view->rect.top), 0);
		glDisable(GL_SCISSOR_TEST);

		subView = subView->nextView;
	}
	
	
	//glViewport(left, window_height - (top + height), width, height);
	//glTranslatef(0, top, 0.0);
	
}
