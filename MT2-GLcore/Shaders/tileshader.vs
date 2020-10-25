#version 330

layout (location = 0) in ivec4 bVertexPosition;
layout (location = 1) in vec2 bTexturePosition;
layout (location = 2) in vec2 bPixelPosition;
layout (location = 3) in int tileAttributes[2];

uniform ivec2 scrollPosition = ivec2(700, 200);

out vec2 texturePosition;
out vec2 pixelPosition;

flat out int tileIdentifier;
flat out int tileAttribute;

void main() {
    float scale = 0.01;
    vec4 scaleVec = vec4(scale, scale, scale, 1.0);
    vec4 offset = ivec4(-256 - scrollPosition.x, -128 + scrollPosition.y, 0, 1);

    texturePosition = bTexturePosition;
    pixelPosition = bPixelPosition;
    
    tileIdentifier = tileAttributes[0];
    tileAttribute = tileAttributes[1];
    
    gl_Position = vec4((bVertexPosition + offset) * scaleVec);
}

