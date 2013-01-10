#version 330

//Render Type
#define LIGHTED 0
#define INTERFACE 1
#define BILLBOARD 2
#define SKYBOX 3

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;
layout(location = 3) in mat4 MVMatrix;

uniform mat4 uPMatrix = mat4(1.f);
uniform int render_type = 0;

out vec4 vNormal;
out vec4 vPosition;
out vec2 vTexCoords;

void main(){
	if(render_type == LIGHTED || render_type == BILLBOARD){
		vNormal = normalize(transpose(inverse(MVMatrix)) * vec4(normal, 0.f));
		vPosition = MVMatrix * vec4(position, 1.f);
		vTexCoords = texture;
		
		gl_Position = uPMatrix * (MVMatrix * vec4(position, 1.f));
		
	}else if(render_type == INTERFACE){
		vTexCoords = texture;
		gl_Position = MVMatrix*vec4(position, 1.f);
		
	}else if(render_type == SKYBOX){
		vec4 MVPpos = uPMatrix * (MVMatrix * vec4(position, 1.f));
		gl_Position = MVPpos.xyww;
		vTexCoords = texture;
	}
}
