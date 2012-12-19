#version 330

in vec3 vNormal;
in vec2 vTexCoords;

uniform mat4 uMVPMatrix = mat4(1.f);

out vec4 fFragColor;

float dotProduct(vec4 v1, vec4 v2){
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

float norm(vec4 v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

vec4 normalize(vec4 v){
	float vec_norm = norm(v);
	return vec4(v.x/vec_norm, v.y/vec_norm, v.z/vec_norm, v.w/vec_norm);
}

void main() {
	vec4 light = vec4(1.f, -1.f, 0.f, 0.f);
	vec4 transfomedNormal = uMVPMatrix * vec4(vNormal, 0.f);
	float simpleLightProcess = dotProduct(normalize(transfomedNormal), normalize(light));
	fFragColor = vec4(0.7f - simpleLightProcess, 1.f - simpleLightProcess, 1.2f - simpleLightProcess, 1.f);
}

