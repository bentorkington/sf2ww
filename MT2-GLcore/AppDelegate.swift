//
//  AppDelegate.swift
//  MT2-GLcore
//
//  Created by Ben Torkington on 18/10/20.
//

import Cocoa
import OpenGL
import GLKit

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {

    @IBOutlet var window: NSWindow!
    @IBOutlet weak var view: MyNSOpenGLView!

    private var timer: Timer!
    private var ticks: UInt64 = AppDelegate.getTicks()

    private var codeRom: [UInt8]?
    private var gfxRom: [UInt8]?
    private var sprites: [UInt8]?
    private var tiles: [UInt8]?

    private var scrollTextures = [GLuint](repeating: 0, count: 3)

    private var game: Game?

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        print("didFinishLaunching")
        // read ROMs
        guard let codeUrl = Bundle.main.url(forResource: "allroms", withExtension: "bin") else { return }
        guard let gfxUrl = Bundle.main.url(forResource: "sf2gfx", withExtension: "bin") else { return }
        
        // GFX data:
        // Total size 0x600000
        
        do {
            let codeData: Data = try Data(contentsOf: codeUrl)
            let gfxData: Data = try Data(contentsOf: gfxUrl)
            
            codeRom = [UInt8](codeData)
            gfxRom = [UInt8](gfxData)

            sprites = [UInt8](gfxData[..<0x480000])
            tiles = [UInt8](gfxData[0x400000...])
        } catch {
            print ("Can't read ROMs")
            return
        }
        
        glGenTextures(3, &scrollTextures)
        
        glBindTexture(GLenum(GL_TEXTURE_2D), scrollTextures[0])
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE)
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_WRAP_T), GL_CLAMP_TO_EDGE)
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MIN_FILTER), GL_NEAREST)
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MAG_FILTER), GL_NEAREST)
        glTexImage2D(
            GLenum(GL_TEXTURE_2D),
            0,
            GL_RED,
            GLsizei(2048),
            GLsizei(2304),
            0,
            GLenum(GL_RED),
            GLenum(GL_UNSIGNED_BYTE),
            sprites
        )
        
        glBindTexture(GLenum(GL_TEXTURE_2D), scrollTextures[1])
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE)
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_WRAP_T), GL_CLAMP_TO_EDGE)
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MIN_FILTER), GL_NEAREST)
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MAG_FILTER), GL_NEAREST)
        glTexImage2D(
            GLenum(GL_TEXTURE_2D),
            0,
            GL_RED,
            GLsizei(2048),
            GLsizei(1024),
            0,
            GLenum(GL_RED),
            GLenum(GL_UNSIGNED_BYTE),
            tiles
        )

        glBindTexture(GLenum(GL_TEXTURE_2D), scrollTextures[0])

//        glBindTexture(GLenum(GL_TEXTURE_2D), scrollTextures[0])

        /* format of texture SCR1
         
         ,-(0.0, 0.0)                 ,-(1.0, 0.0)
         1248----1248---  ... 1248----
         ||||
         |||`--------------- '8' bits for eight pixels
         ||`---------------- '4' bits for eight pixels
         
         */
        
        // Do some GL setup.
        glClearColor(0.0, 0.0, 0.0, 1.0)
        glClearDepth(1.0)
        //glDisable(GLenum(GL_BLEND))
        glBlendFunc(GLenum(GL_SRC_ALPHA), GLenum(GL_ONE_MINUS_SRC_ALPHA))
        glEnable(GLenum(GL_BLEND))
        
        
        glEnable(GLenum(GL_PROGRAM_POINT_SIZE))
//        glEnable(GLenum(GL_DEPTH_TEST))
//        glDepthFunc(GLenum(GL_LEQUAL))
//        glEnable(GLenum(GL_CULL_FACE))
//        glFrontFace(GLenum(GL_CCW))
//        glCullFace(GLenum(GL_BACK))
        
        game = Game(withTextures: scrollTextures)
        
        timer = Timer(timeInterval: 1.0 / 60.0, repeats: true, block: timerFireMethod)
        RunLoop.current.add(timer, forMode: RunLoop.Mode.default)
        
        print("didFinishLaunching done")
    }
    
    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func timerFireMethod(_ sender: Timer!) {
        glClear(GLbitfield(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
        game?.render(view.projectionMatrix)
        //        scene.render(view.projectionMatrix)
        glFlush()
        view.flush()

        // Cycle the scene.
        let newTicks = AppDelegate.getTicks()
        let secondsElapsed = Float(newTicks - ticks) / 1000.0
        ticks = newTicks
        //scene.cycle(secondsElapsed)
        game?.tick(secondsElapsed)
    }

    private class func getTicks() -> UInt64 {
        var t = timeval()
        gettimeofday(&t, nil)
        return UInt64(t.tv_sec * 1000) + UInt64(t.tv_usec / 1000)
    }
}

