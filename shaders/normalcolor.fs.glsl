#version 330

//STRUCTURES
struct Material{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float shininess;
};

struct DirectionalLight{
	vec4 dir;
	vec3 i;
};
//END

in vec4 vNormal;
in vec4 vPosition;
in vec2 vTexCoords;

uniform Material uMaterial;
uniform DirectionalLight uDirLight;

out vec4 fFragColor;

void main() {
	vec4 nLightDir =  normalize(uDirLight.dir);
	vec4 reflectLightDir = reflect(nLightDir, vNormal);
	float coefDiffus = max(0, dot(vNormal, -nLightDir));
	float coefSpecular = pow(max(0, dot(-vPosition, reflectLightDir)), uMaterial.shininess);
	vec3 color =  uDirLight.i *(uMaterial.Ka + uMaterial.Kd*coefDiffus + uMaterial.Ks*coefSpecular);
	fFragColor = vec4(color, 1.f);
}

