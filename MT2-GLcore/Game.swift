//
//  Game.swift
//  MT2-GLcore
//
//  Created by Ben Torkington on 18/10/20.
//

import Foundation
#if os(macOS)
import OpenGL
#else
import OpenGLES
#endif

let NUM_LIGHTS = 3

class Game {
    private var program: ShaderProgram
    
    private var programTileIdLocation: GLuint
    private var programTileSizeLocation: GLuint
    private var programRowByteStride: GLuint
    private var programTileByteSize: GLuint
    private var programAtlasWidth: GLuint
    private var programAtlasHeight: GLuint
    
    private var lightPosition = [Float](repeating: 0.0, count: NUM_LIGHTS * 3)
    private var lightColor: [Float] = [1.0, 0.0, 0.0,
                                       0.0, 1.0, 0.0,
                                       0.0, 0.0, 1.0]
    private var lightRotation: Float = 0.0

//    private var normalmap: Texture?
//    private var renderable: Renderable

    private var cameraRotation: Float = 0.0
    private var cameraPosition: [Float] = [0.0, 0.0, 4.0]
    
    init() {
        // Create the program, attach shaders, and link.
        program = ShaderProgram()
        program.attachShader("pointshader.vs", withType: GL_VERTEX_SHADER)
        program.attachShader("pointshader.fs", withType: GL_FRAGMENT_SHADER)
        program.link()

        // Get uniform locations.
//        programProjectionMatrixLocation = program.getUniformLocation("projectionMatrix")!
//        programModelviewMatrixLocation = program.getUniformLocation("modelviewMatrix")!
//        programCameraPositionLocation = program.getUniformLocation("cameraPosition")!
//        programLightPositionLocation = program.getUniformLocation("lightPosition")!
//        programLightColorLocation = program.getUniformLocation("lightColor")!
        
        programTileIdLocation = program.getUniformLocation("tileId")!
        programTileSizeLocation = program.getUniformLocation("tileSize")!
        programRowByteStride = program.getUniformLocation("rowByteStride")!
        programTileByteSize = program.getUniformLocation("tileByteSize")!
        programAtlasWidth = program.getUniformLocation("atlasWidth")!
        programAtlasHeight = program.getUniformLocation("atlasHeight")!
        
//        renderable = PointSprite(program: program)
        
        gfx_glut_init()
        load_cps_roms()
        manual_init()

        //print(addThings(4, 5))

    }

    func render(_ projectionMatrix: Matrix4) {
        let translationMatrix = Matrix4.translationMatrix(x: -cameraPosition[0], y: -cameraPosition[1], z: -cameraPosition[2])
        let rotationMatrix = Matrix4.rotationMatrix(angle: cameraRotation, x: 0.0, y: -1.0, z: 0.0)
        let modelviewMatrix = translationMatrix * rotationMatrix

        // Enable the program and set uniform variables.
        program.use()
        
//        glBlendFunc(GLenum(GL_SRC_ALPHA), GLenum(GL_ONE_MINUS_SRC_ALPHA))
//        glEnable(GLenum(GL_BLEND))
        
//        glUniformMatrix4fv(GLint(programProjectionMatrixLocation), 1, GLboolean(GL_FALSE), projectionMatrix.matrix)
//        glUniformMatrix4fv(GLint(programModelviewMatrixLocation), 1, GLboolean(GL_FALSE), modelviewMatrix.matrix)
//        glUniform3fv(GLint(programCameraPositionLocation), 1, cameraPosition)
//        glUniform3fv(GLint(programLightPositionLocation), GLint(NUM_LIGHTS), lightPosition)
//        glUniform3fv(GLint(programLightColorLocation), GLint(NUM_LIGHTS), lightColor)

        /* Blank tile IDs
        #define TILE_BLANK_SCR1     0x4000
        #define TILE_BLANK_SCR2     0x2800
        #define TILE_BLANK_SCR3     0x400 */
        
        glUniform1i(GLint(programTileIdLocation), 0x815f)
        glUniform1i(GLint(programTileSizeLocation), 16)
        glUniform1i(GLint(programRowByteStride), 8)     //SR1, 2, sprites: 8, SCR3: 16
        glUniform1i(GLint(programTileByteSize), 0x80) // SCR1: 0x40, SCR2/sprite: 0x80, SCR3: 0x200
        glUniform1i(GLint(programAtlasWidth), 2048)
        glUniform1i(GLint(programAtlasHeight), 2304)
        
        
        // Render the object.
//        renderable.render()

        // Disable the program.
        glUseProgram(0)
        
    }

    func tick(_ secondsElapsed: Float) {
        task_timer()
        //print(" \(gemu.Scroll1Y) \(gemu.Scroll2Y) \(gemu.Scroll3Y)")
        gfx_glut_drawgame()
    }
}
