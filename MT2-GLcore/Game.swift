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
    private var tileProgram: ShaderProgram
    
    private var lightPosition = [Float](repeating: 0.0, count: NUM_LIGHTS * 3)
    private var lightColor: [Float] = [1.0, 0.0, 0.0,
                                       0.0, 1.0, 0.0,
                                       0.0, 0.0, 1.0]
    private var lightRotation: Float = 0.0

//    private var normalmap: Texture?
    private var renderable: PointSprite

    private var cameraRotation: Float = 0.0
    private var cameraPosition: [Float] = [0.0, 0.0, 4.0]
    
    private var textures: [GLuint]
    
    init(withTextures textures: [GLuint]) {
        self.textures = textures
        print (textures[1])
        // Create the program, attach shaders, and link.
        program = ShaderProgram()
        program.attachShader("pointshader.vs", withType: GL_VERTEX_SHADER)
        program.attachShader("pointshader.fs", withType: GL_FRAGMENT_SHADER)
        program.link()
        
        tileProgram = ShaderProgram();
        tileProgram.attachShader("tileshader.vs", withType: GL_VERTEX_SHADER)
        tileProgram.attachShader("tileshader.fs", withType: GL_FRAGMENT_SHADER)
        tileProgram.link()
        
        renderable = PointSprite(program: program)
        
        init_glcore(program.program, tileProgram.program, textures[0], textures[1])
        load_cps_roms()
        manual_init()
    }

    func render(_ projectionMatrix: Matrix4) {
//        let translationMatrix = Matrix4.translationMatrix(x: -cameraPosition[0], y: -cameraPosition[1], z: -cameraPosition[2])
//        let rotationMatrix = Matrix4.rotationMatrix(angle: cameraRotation, x: 0.0, y: -1.0, z: 0.0)
//        let modelviewMatrix = translationMatrix * rotationMatrix

        // Enable the program and set uniform variables.
//        program.use()
        
//        glBlendFunc(GLenum(GL_SRC_ALPHA), GLenum(GL_ONE_MINUS_SRC_ALPHA))
//        glEnable(GLenum(GL_BLEND))
        
//        glUniformMatrix4fv(GLint(programProjectionMatrixLocation), 1, GLboolean(GL_FALSE), projectionMatrix.matrix)
//        glUniformMatrix4fv(GLint(programModelviewMatrixLocation), 1, GLboolean(GL_FALSE), modelviewMatrix.matrix)
//        glUniform3fv(GLint(programCameraPositionLocation), 1, cameraPosition)
//        glUniform3fv(GLint(programLightPositionLocation), GLint(NUM_LIGHTS), lightPosition)
//        glUniform3fv(GLint(programLightColorLocation), GLint(NUM_LIGHTS), lightColor)
        
        
        // Render the object.
        //renderable.render()
        
        render_glcore()

        // Disable the program.
        glUseProgram(0)
        
    }

    func tick(_ secondsElapsed: Float) {
        //task_timer()
//        gfx_glut_drawgame()
    }
}
