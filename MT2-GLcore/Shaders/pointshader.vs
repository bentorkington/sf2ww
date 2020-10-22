#version 330

in vec4 aVertexPosition;
in int tileAttributes[2];

uniform ivec2 scrollPosition;

uniform float pointSize;

flat out int tileIdentifier;
flat out int tileAttribute;

void main() {
    float scale = 0.002;
    vec4 scaleVec = vec4(scale, scale, scale, 1.0);
    vec4 offset = ivec4(212 - scrollPosition.x, -512 + scrollPosition.y, 0, 1);
    
    tileIdentifier = tileAttributes[0];
    tileAttribute = tileAttributes[1];

    gl_Position = vec4((aVertexPosition + offset) * scaleVec);
    gl_PointSize = pointSize;
}

