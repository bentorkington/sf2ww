//
//  OGLView.m
//  Points
//
//  Created by Ben on 26/05/11.
//  Copyright 2011 Ben Torkington. All rights reserved.
//

#import "OGLView.h"
#import "GLUT/glut.h"
#import "trackball.h"

#include "sf2const.h"
#include "sf2types.h"
#include "sf2macros.h"
#include "gstate.h"

#include "structs.h"
#include "particle.h"
#include "player.h"
#include "sm.h"
#include "gfx_glut.h"
//#include "lib.h"
#include "gemu.h"
#include "glwimp.h"
#include "workarounds.h"
#include "pthreads.h"
#include "sf2io.h"	

#include "game.h"
#include "task.h"

#include "redhammer.h"

// You're not the boss of me
#define GL_SILENCE_DEPRECATION 1
#define DEBUG TRUE

extern Game g;
extern CPSGFXEMU gemu;

extern int gGameInWindow;
extern int gShowHelp;
LBView *gGameTitleBar;
LBView *gGameWindowIcon;
LBView *gGameArea;
extern LBView *gameView;
extern LBWindow *dummyWindow;


extern LBView rootView;

void mouse (int button, int state, int x, int y);
void drawGLString(GLfloat x, GLfloat y, char *string);
void GameViewClicked(LBView *view, int button, int state, int x, int y);
void renderDummy(struct view *);

int gtimercount	= 0;
int gsupertaskcnt = 0;
int gdrawallcnt = 0;
extern struct inputs gInputs;

GLfloat gShapeSize = 11.0f;


@implementation OGLView

- (void)prepare
{
    NSLog(@"prepare");
    NSOpenGLContext *glcontext = [self openGLContext];
    [glcontext makeCurrentContext];
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    //glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glEnable(GL_LIGHT0);
    
    GLfloat mata[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat matb[] = {0.9, 0.9, 0.9, 1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mata);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matb);
    
    gCameraReset();
    
    //glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);    
    glFrontFace(GL_CCW);
    
    glColor3f(1.0,1.0,1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);      
    
    glPolygonOffset (1.0, 1.0);
    glEnable(GL_LIGHTING);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_SMOOTH);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_SMOOTH);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    gfx_glut_init();
    load_cps_roms();
    glwimp_init(900, 600);
    
    dummyWindow = createWindow(40, 50, 384, 224);
    dummyWindow->areaView->renderFunc = &renderDummy;
    
    gameView		= addSubView(&rootView, &DrawNull, &ClickNull, 10, 10, 400, 300 + WIN_TITLE_HEIGHT);
    gGameWindowIcon   = addSubView(gameView, &DrawWindowButton, &WindowButtonClicked, 2, 2, WIN_TITLE_HEIGHT - 4, WIN_TITLE_HEIGHT - 4);
    gGameTitleBar     = addSubView(gameView, &DrawTitleBar, &DragBarClicked, WIN_TITLE_HEIGHT + 8, 2, 400 - (WIN_TITLE_HEIGHT + 10), WIN_TITLE_HEIGHT - 4);
    gGameArea         = addSubView(gameView, &DrawNull, &GameViewClicked, 0, WIN_TITLE_HEIGHT, 400, 300);			// XXX
    
    manual_init();
    
    timer = [[NSTimer scheduledTimerWithTimeInterval:1.0 / 60
                                              target:self
                                            selector:@selector(timerCallback)
                                            userInfo:nil
                                             repeats:YES] retain];
}

-(IBAction)resetGame:(id)sender
{
    NSLog(@"todo");
}

void renderDummy(struct view *view) {
    glPushMatrix();
    
    glEnable(GL_TEXTURE_2D);
    gfx_glut_drawgame();
    
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

- (id)initWithCoder:(NSCoder *)c
{
    self = [super initWithCoder:c];
    [self prepare];
    return self;
}

- (void)reshape
{
    baseRect = [self convertRectToBase:[self bounds]]; 
    gfx_glut_reshape(baseRect.size.width, baseRect.size.height);
    [super reshape];
}

- (void)awakeFromNib
{
    [self setNeedsDisplay:YES];
}

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void)timerCallback
{
    task_timer();
    [self setNeedsDisplay:YES];
}

- (void)drawGame:(NSRect)dirtyRect {
    gfx_glut_drawgame();
}

- (void)drawRect:(NSRect)r {
    [self drawGame:r];
    
    glEnable(GL_LIGHTING);
    glColor3f(0.5, 0.5, 0.5);
        
    glFinish();
}

- (NSPoint)convertToOpenGL:(NSPoint)p 
{
    NSPoint q;
    q.y = baseRect.size.height - p.y;
    q.x = p.x;
    return q;
}

- (void)mouseDown:(NSEvent *)event
{
    NSPoint p = [event locationInWindow];
    NSPoint q = [self convertToOpenGL:p];
    gfx_glut_mousedown(q.x, q.y);
}

- (void)rightMouseDown:(NSEvent *)event
{
    NSPoint p = [self convertToOpenGL:[event locationInWindow]];
    gfx_glut_rightmousedown(p.x, p.y);
}

- (void)rightMouseDragged:(NSEvent *)event
{
    NSPoint p = [self convertToOpenGL:[event locationInWindow]];
    gfx_glut_rightmousedragged(p.x, p.y);
    [self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)event
{
    NSPoint p = [self convertToOpenGL:[event locationInWindow]];
    gfx_glut_mousedragged(p.x, p.y);
    [self setNeedsDisplay:YES];
}

- (void)mouseUp:(NSEvent *)event
{
    NSPoint p = [self convertToOpenGL:[event locationInWindow]];
    gfx_glut_mouseup(p.x, p.y);

}

- (void)rightMouseUp:(NSEvent *)event
{
    NSPoint p = [self convertToOpenGL: [event locationInWindow]];
    gfx_glut_rightmouseup(p.x, p.y);
}

- (BOOL)acceptsFirstResponder
{
    return YES;		// YES, yes I do.
}

- (BOOL)resignFirstResponder
{
    [self setNeedsDisplay:YES];
    return YES;
}

- (BOOL)becomeFirstResponder
{
    [self setNeedsDisplay:YES];
    return YES;
}
 
- (void)keyDown:(NSEvent *)theEvent
{
    unichar keypress;
    if ([theEvent isARepeat] == NO) {
        keypress = [[theEvent characters]characterAtIndex:0];

        switch (keypress) {
            case NSUpArrowFunctionKey:    gInputs.p10 |= JOY_UP;        break;
            case NSDownArrowFunctionKey:  gInputs.p10 |= JOY_DOWN;      break;
            case NSLeftArrowFunctionKey:  gInputs.p10 |= JOY_LEFT;      break;
            case NSRightArrowFunctionKey: gInputs.p10 |= JOY_RIGHT;     break;

            case 'q': gInputs.p10 |= BUTTON_A;	       break;
            case 'w': gInputs.p10 |= BUTTON_B;	       break;
            case 'e': gInputs.p10 |= BUTTON_C;	       break;
            case 'a': gInputs.p11 |= BUTTON_D >> 8;	   break;
            case 's': gInputs.p11 |= BUTTON_E >> 8;	   break;
            case 'd': gInputs.p11 |= BUTTON_F >> 8;	   break;
            case '1': gInputs.in0 |= IPT_START1;	   break;
            case '2': gInputs.in0 |= IPT_START2;	   break;
            case '5': gInputs.in0 |= IPT_COIN1;	       break;
            case '6': gInputs.in0 |= IPT_COIN2;	       break;
            case '7': gemu_flip_scroll_enable(0);      break;
            case '8': gemu_flip_scroll_enable(1);      break;
            case '9': gemu_flip_scroll_enable(2);      break;
            case '0': gemu_flip_scroll_enable(3);      break;
            case 'T': print_task_table();              break;
            case 'K': g.Player2.Energy = -1;           break;
            case 'p': g.JPParam ^= JP_FREEZE;          break;
            case 'W': gGameInWindow = 1-gGameInWindow; break;
            case '[': gShowHelp = !gShowHelp;          break;
            default:
                break;
        }
        
    }
}

-(void)keyUp:(NSEvent *)theEvent
{
    unichar keypress = [[theEvent characters]characterAtIndex:0];
    switch (keypress) {
        case 'q':		gInputs.p10 &= ~(BUTTON_A);	     break;
        case 'w':		gInputs.p10 &= ~(BUTTON_B);	     break;
        case 'e':		gInputs.p10 &= ~(BUTTON_C);	     break;
        case 'a':		gInputs.p11 &= ~(BUTTON_D >> 8); break;
        case 's':		gInputs.p11 &= ~(BUTTON_E >> 8); break;
        case 'd':		gInputs.p11 &= ~(BUTTON_F >> 8); break;
        case '1':       gInputs.in0 &= ~IPT_START1;	     break;
        case '2':       gInputs.in0 &= ~IPT_START2;	     break;
        case '5':       gInputs.in0 &= ~IPT_COIN1;	     break;
        case '6':       gInputs.in0 &= ~IPT_COIN2;	     break;
            
        case NSUpArrowFunctionKey:    gInputs.p10 &= ~JOY_UP;    break;
        case NSDownArrowFunctionKey:  gInputs.p10 &= ~JOY_DOWN;  break;
        case NSLeftArrowFunctionKey:  gInputs.p10 &= ~JOY_LEFT;  break;
        case NSRightArrowFunctionKey: gInputs.p10 &= ~JOY_RIGHT; break;
        default:
            break;
    }
}
     
void GameViewClicked(LBView *view, int button, int state, int x, int y) {
}

@end
