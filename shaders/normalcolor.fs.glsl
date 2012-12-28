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

struct PointLight{
	vec4 lPos;
	vec3 i;
};
//END

in vec4 vNormal;
in vec4 vPosition;
in vec2 vTexCoords;

uniform Material uMaterial;
uniform DirectionalLight uDirLight;
uniform PointLight uPointLight;

out vec4 fFragColor;

//FUNCTIONS
vec4 createVector(vec4 p1, vec4 p2){
	return vec4(p2.x-p1.x, p2.y-p1.y, p2.z-p1.z, p2.w-p1.w);
}

//END

void main() {
	//compute the reflection of the directionnal light
	vec4 d_LightDir =  normalize(uDirLight.dir);
	vec4 d_reflectLightDir = normalize(reflect(d_LightDir, vNormal));
	float d_coefDiffus = max(0, dot(vNormal, -d_LightDir));
	float d_coefSpecular = pow(max(0, dot(-vPosition, d_reflectLightDir)), uMaterial.shininess);
	vec3 d_colorLight =  uDirLight.i *(uMaterial.Ka + uMaterial.Kd*d_coefDiffus + uMaterial.Ks*d_coefSpecular);
	
	//compute the reflection of the point light
	vec4 p_LightDir = normalize(createVector(uPointLight.lPos, vPosition));
	float p_lengLightDir = dot(p_LightDir, p_LightDir);
	vec4 p_reflectLightDir = normalize(reflect(p_LightDir, vNormal));
	float p_coefDiffus = max(0, dot(vNormal, -p_LightDir)) / p_lengLightDir;
	float p_coefSpecular = pow(max(0, dot(-vPosition, p_reflectLightDir)), uMaterial.shininess) / p_lengLightDir;
	//vec3 p_colorLight = uPointLight.i * (uMaterial.Ka + uMaterial.Kd*p_coefDiffus);
	vec3 p_colorLight = uPointLight.i * (uMaterial.Ka + uMaterial.Kd*p_coefDiffus + uMaterial.Ks*p_coefSpecular);
	
	//fFragColor = vec4(p_colorLight, 1.f);
	fFragColor = vec4(d_colorLight + p_colorLight, 1.f);
}

