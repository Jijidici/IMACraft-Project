#version 330
#define MAX_POINT_LIGHT_COUNT 16

//Render Type
#define LIGHTED 0
#define INTERFACE 1
#define BILLBOARD 2
#define SKYBOX 3

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
in vec3 vSkyTexCoords;

uniform Material uMaterial;
uniform DirectionalLight uDirLight;
uniform PointLight uPointLights[MAX_POINT_LIGHT_COUNT];
uniform int uPointLightCount = 0;

uniform sampler2D uTextureSampler0;
uniform int render_type = 0;

out vec4 fFragColor;

//FUNCTIONS
vec4 createVector(vec4 p1, vec4 p2){
	return vec4(p2.x-p1.x, p2.y-p1.y, p2.z-p1.z, p2.w-p1.w);
}

//END

void main() {
	if(render_type == LIGHTED){
		//compute the ambiant color
		vec3 ambColor = texture(uTextureSampler0, vTexCoords).xyz*uMaterial.Ka;
	
		//compute the diffuse color
		vec3 diffColor = texture(uTextureSampler0, vTexCoords).xyz*uMaterial.Kd;
		
		
		//compute the reflection of the directionnal light
		vec4 d_LightDir =  normalize(uDirLight.dir);
		vec4 d_reflectLightDir = normalize(reflect(d_LightDir, vNormal));
		
		float d_coefDiffus = max(0, dot(vNormal, -d_LightDir));
		float d_coefSpecular = pow(max(0, dot(-vPosition, d_reflectLightDir)), uMaterial.shininess);
		
		vec3 color =  uDirLight.i *(ambColor + diffColor*d_coefDiffus + uMaterial.Ks*d_coefSpecular);
		
		//compute the reflection of all the point light
		for(int i=0;i<uPointLightCount;++i){
			vec4 p_LightDir = createVector(uPointLights[i].lPos, vPosition);
			float p_lengLightDir = 10*dot(p_LightDir, p_LightDir);
			vec4 p_normalizedLD = normalize(p_LightDir);
			vec4 p_reflectLightDir = normalize(reflect(p_normalizedLD, vNormal));
			
			float p_coefDiffus = max(0, dot(vNormal, -p_normalizedLD)) / p_lengLightDir;
			float p_coefSpecular = pow(max(0, dot(-vPosition, p_reflectLightDir)), uMaterial.shininess) / p_lengLightDir;
			
			color += uPointLights[i].i * (ambColor + diffColor*p_coefDiffus + uMaterial.Ks*p_coefSpecular);
		}
		
		fFragColor = vec4(color, 1.f);
	}else{
		vec4 texel = texture(uTextureSampler0, vTexCoords);
		if(texel.a <0.5){
			discard;
		}
		
		fFragColor = texel;
	}
}

