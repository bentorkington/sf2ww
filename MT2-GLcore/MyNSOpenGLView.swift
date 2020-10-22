//
//  MyNSOpenGLView.swift
//  MT2-GLcore
//
//  Created by Ben Torkington on 18/10/20.
//

import Cocoa
import GLUT

class MyNSOpenGLView: NSOpenGLView
{
    var projectionMatrix = Matrix4()

    private var trackingArea: NSTrackingArea?
    
    override func awakeFromNib()
    {
        let attr = [
            NSOpenGLPixelFormatAttribute(NSOpenGLPFAOpenGLProfile),
            NSOpenGLPixelFormatAttribute(NSOpenGLProfileVersion3_2Core),
            NSOpenGLPixelFormatAttribute(NSOpenGLPFAColorSize), 24,
            NSOpenGLPixelFormatAttribute(NSOpenGLPFAAlphaSize), 8,
            NSOpenGLPixelFormatAttribute(NSOpenGLPFADoubleBuffer),
            NSOpenGLPixelFormatAttribute(NSOpenGLPFADepthSize), 32,
            0
        ]

        let format = NSOpenGLPixelFormat(attributes: attr)
        let context = NSOpenGLContext(format: format!, share: nil)

        self.openGLContext = context
        self.openGLContext?.makeCurrentContext()
    }
    
    override func reshape() {
        let frame = self.frame

        // Update the viewport.
        glViewport(0, 0, GLsizei(frame.size.width), GLsizei(frame.size.height))

        // Create projection matrix.
        let aspectRatio = Float(frame.size.width) / Float(frame.size.height)
        projectionMatrix = Matrix4.perspectiveMatrix(fov: Float.pi / 4.0, aspect: aspectRatio, near: 0.1, far: 200.0)

        // Remove existing tracking area if necessary.
        if trackingArea != nil {
            removeTrackingArea(trackingArea!)
        }

        // Create new tracking area.
        let options: NSTrackingArea.Options = [NSTrackingArea.Options.mouseMoved, NSTrackingArea.Options.activeWhenFirstResponder]
        trackingArea = NSTrackingArea(rect: frame, options: options, owner: self, userInfo: nil)
    }
    
    override var acceptsFirstResponder: Bool {
        return true
    }

    override func keyDown(with theEvent: NSEvent) {
        // Close the window when the escape key is pressed.
        if theEvent.keyCode == 0x35 {
            window?.close()
        }
    }

    func flush()
    {
        self.openGLContext?.flushBuffer()
    }
}
