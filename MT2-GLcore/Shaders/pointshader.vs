#version 330

in vec4 aVertexPosition;

void main() {
    gl_Position = vec4(aVertexPosition);
    gl_PointSize = 512.0;
}

