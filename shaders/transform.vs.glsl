#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;
layout(location = 3) in mat4 MVMatrix;

uniform mat4 uPMatrix = mat4(1.f);
uniform int is_skyBox = 0;

out vec4 vNormal;
out vec4 vPosition;
out vec2 vTexCoords;

void main(){
	if(is_skyBox == 0){
		vNormal = normalize(transpose(inverse(MVMatrix)) * vec4(normal, 0.f));
		vPosition = normalize(MVMatrix * vec4(position, 1.f));
		vTexCoords = texture;
		
		gl_Position = uPMatrix * (MVMatrix * vec4(position, 1.f));
	}else{
		vec4 MVPpos = uPMatrix * (MVMatrix * vec4(position, 1.f));
		gl_Position = MVPpos.xyww;
		vTexCoords = texture;
	}
}
