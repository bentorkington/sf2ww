//
//  MT2AppDelegate.h
//  MT2
//
//  Created by Ben on 23/06/11.
//  Copyright 2011 Ben Torkington. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface MT2AppDelegate : NSObject  {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;
- (IBAction)resetState:(id)sender;


@end
