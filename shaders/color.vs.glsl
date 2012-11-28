#version 330

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

uniform mat4 uMVPMatrix = mat4(1.f);

out vec3 vFragColor;

void main() {
    vFragColor = aVertexColor;
    gl_Position = uMVPMatrix * vec4(aVertexPosition, 1.f);
}
