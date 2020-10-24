
#version 330

layout (location = 0) in vec3 bVertexPosition;
layout (location = 1) in vec2 bTexturePosition;
layout (location = 2) in vec2 bPixelPosition;
layout (location = 3) in int tileAttributes[2];

out vec2 texturePosition;
out vec2 pixelPosition;
flat out int tileIdentifier;
flat out int tileAttribute;

void main() {
    texturePosition = bTexturePosition;
    pixelPosition = bPixelPosition;
    
    tileIdentifier = tileAttributes[0];
    tileAttribute = tileAttributes[1];
//    gl_PointSize = 8.0;
    gl_Position = vec4(bVertexPosition, 1.0);
}

