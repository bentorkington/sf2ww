//
//  PointSprite.swift
//  CocoaOpenGL-Swift-macOS
//
//  Created by Ben Torkington on 30/09/20.
//

import OpenGL

class PointSprite: Renderable
{
    private var vertexArrayId: GLuint = 0
    private var bufferIds = [GLuint](repeating: 0, count: 5)
    private var size = 4;
    
    private var textures = [GLuint](repeating: 0, count: 2)

    init(program: ShaderProgram) {
        let points: [Float] = [
            0.0, 0.0, 0.1, 0.0, -20.0, 20.0, 20.0, 20.0
        ]
                
        let tilemap: [UInt8] = [
            0, 1, 2, 3, 4, 5, 6, 7,
            0, 1, 2, 3, 4, 5, 6, 7,
            1, 1, 0, 0, 1, 1, 0, 0,
            1, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 1, 1, 0, 0, 1, 1,
            0, 0, 1, 1, 0, 0, 1, 1,
            1, 1, 0, 0, 1, 1, 0, 0,
            1, 1, 0, 0, 1, 1, 0, 0,
        ]
        
        let palette: [UInt8] = [
            255, 255, 255, 255, // white
            192, 192,   0, 255, // yellow
              0, 192, 192, 255, // cyan
              0, 192,   0, 255, // green
            192,   0, 192, 255, // magenta
            192,   0,   0, 255, // red
              0,   0, 192, 255, // blue
              0,   0,   0, 255, // black
            
            128, 128, 128, 255, // white
             96,  96,   0, 255, // yellow
              0,  96,  96, 255, // cyan
              0,  96,   0, 255, // green
             96,   0,  96, 255, // magenta
             96,   0,   0, 255, // red
              0,   0,  96, 255, // blue
              0, 255,   0, 0, // green screen
        ]
        
        let vertexPositionLocation = program.getAttributeLocation("aVertexPosition")!
        
        glGenVertexArrays(1, &vertexArrayId)
        glBindVertexArray(vertexArrayId)
        
        glGenBuffers(1, &bufferIds)
        
        glBindBuffer(GLenum(GL_ARRAY_BUFFER), bufferIds[0])
        glBufferData(GLenum(GL_ARRAY_BUFFER), MemoryLayout<Float>.size * 2, points, GLenum(GL_STATIC_DRAW))
        
        glEnableVertexAttribArray(vertexPositionLocation)
        glVertexAttribPointer(vertexPositionLocation, 3, GLenum(GL_FLOAT), GLboolean(GL_FALSE), 0, nil)
        
        let imageUniformLocation = program.getUniformLocation("u_image")!
        let paletteUniformLocation = program.getUniformLocation("u_palette")!
//
        
        glGenTextures(2, &textures)

        program.use()
        glUniform1i(GLint(imageUniformLocation), 0)
        glUniform1i(GLint(paletteUniformLocation), 1)
        
        glActiveTexture(GLenum(GL_TEXTURE1))

        glBindTexture(GLenum(GL_TEXTURE_2D), textures[0])
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE)
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_WRAP_T), GL_CLAMP_TO_EDGE)
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MIN_FILTER), GL_NEAREST)
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MAG_FILTER), GL_NEAREST)

        glTexImage2D(GLenum(GL_TEXTURE_2D), 0, GL_RGBA, GLsizei(16), GLsizei(1), 0, GLenum(GL_RGBA), GLenum(GL_UNSIGNED_BYTE), palette)

//        glActiveTexture(GLenum(GL_TEXTURE0))
//        glBindTexture(GLenum(GL_TEXTURE_2D), textures[1])
//        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE)
//        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_WRAP_T), GL_CLAMP_TO_EDGE)
//        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MIN_FILTER), GL_NEAREST)
//        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MAG_FILTER), GL_NEAREST)
//        print(glGetError())
//
//        glTexImage2D(GLenum(GL_TEXTURE_2D), 0, GL_RED, GLsizei(8), GLsizei(8), 0, GLenum(GL_RED), GLenum(GL_UNSIGNED_BYTE), tilemap)
        print(glGetError())

    }
    
    deinit {
        glDeleteBuffers(1, &bufferIds)
        glDeleteVertexArrays(1, &vertexArrayId)
    }
    
    func render() {
        glBindVertexArray(vertexArrayId)
        glDrawArrays(GLenum(GL_POINTS), 0, 4)
    }
}
