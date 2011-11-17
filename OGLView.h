//
//  OGLViewController.h
//  Points
//
//  Created by Ben on 26/05/11.
//  Copyright 2011 Ben Torkington. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <GLUT/glut.h>




@interface OGLView : NSOpenGLView {
	NSTimer *timer;
	int displayList;
	GLfloat trans[3];
	GLfloat pointSize;
}

- (BOOL) acceptsFirstResponder;
- (BOOL) becomeFirstResponder;
@end
