#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;

uniform mat4 uMVPMatrix = mat4(1.f);
uniform mat4 uMVMatrix = mat4(1.f);
uniform mat4 uNormalMatrix = mat4(1.f);

out vec4 vNormal;
out vec4 vPosition;
out vec2 vTexCoords;

void main(){
	vNormal = normalize(uNormalMatrix * vec4(normal, 0.f));
	vPosition = normalize(uMVMatrix * vec4(position, 1.f));
	vTexCoords = texture;
	
	gl_Position = uMVPMatrix * vec4(position, 1.f);
}
