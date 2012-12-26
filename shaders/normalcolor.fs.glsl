#version 330

//STRUCTURES
struct Material{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float shininess;
};

struct DirectionalLight{
	vec3 dir;
	vec3 i;
};
//END

in vec4 vNormal;
in vec2 vTexCoords;

uniform Material uMaterial;
uniform DirectionalLight uDirLight;

out vec4 fFragColor;

void main() {
	float coefDiffus = max(0, dot(vNormal, - normalize(vec4(uDirLight.dir, 0.f))));
	vec3 color =  uDirLight.i *(uMaterial.Ka + uMaterial.Kd*coefDiffus);
	fFragColor = vec4(color, 1.f);
}

