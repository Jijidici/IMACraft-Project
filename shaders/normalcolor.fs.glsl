#version 330

//STRUCTURES
struct Material{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float shininess;
};

//END

in vec3 vNormal;
in vec2 vTexCoords;

uniform Material uMaterial;

out vec4 fFragColor;

void main() {
	fFragColor = vec4(uMaterial.Kd, 1.f);
}

