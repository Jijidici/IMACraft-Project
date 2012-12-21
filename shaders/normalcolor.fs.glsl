#version 330

in vec3 vNormal;
in vec2 vTexCoords;

out vec4 fFragColor;

float dotProduct(vec3 v1, vec3 v2){
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

float norm(vec3 v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 normalize(vec3 v){
	float vec_norm = norm(v);
	return vec3(v.x/vec_norm, v.y/vec_norm, v.z/vec_norm);
}

void main() {
	vec3 light = vec3(1.f, -1.f, 0.f);
	float simpleLightProcess = dotProduct(normalize(vNormal), normalize(light));
	fFragColor = vec4(0.7f - simpleLightProcess, 0.8f - simpleLightProcess, 1.f - simpleLightProcess, 1.f);
}

