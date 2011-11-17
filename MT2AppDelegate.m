//
//  MT2AppDelegate.m
//  MT2
//
//  Created by Ben on 23/06/11.
//  Copyright 2011 Ben Torkington. All rights reserved.
//

#import "MT2AppDelegate.h"
@class OGLView;
@implementation MT2AppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application 
	//NSWindowController *win;
	//win = [[NSWindowController alloc] initWithWindowNibName: @"ParamController"];
	//[win showWindow:self];
}

- (IBAction) resetState:(id)sender
{
	[OGLView resetState];
}

@end
