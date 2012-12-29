#include "imacraft/lighting/Material.hpp"

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace imacraft{
	Material::Material(glm::vec3 inKa, glm::vec3 inKd, glm::vec3 inKs, float inShininess) :
		Ka(inKa), Kd(inKd), Ks(inKs), shininess(inShininess){
	}
	
	MaterialUniform::MaterialUniform() : 
		KaLocation(0), KdLocation(0), KsLocation(0), ShininessLocation(0){
	}
	
	void MaterialUniform::getLocations(const char* uniform, const GLuint program){
		KaLocation = glGetUniformLocation(program, (std::string(uniform)+".Ka").c_str());
		KdLocation = glGetUniformLocation(program, (std::string(uniform)+".Kd").c_str());
		KsLocation = glGetUniformLocation(program, (std::string(uniform)+".Ks").c_str());
		ShininessLocation = glGetUniformLocation(program, (std::string(uniform)+".shininess").c_str());
	}
	
	void sendMaterial(const Material& material, const MaterialUniform& materialUniform){
		glUniform3fv(materialUniform.KaLocation, 1, glm::value_ptr(material.Ka));
		glUniform3fv(materialUniform.KdLocation, 1, glm::value_ptr(material.Kd));
		glUniform3fv(materialUniform.KsLocation, 1, glm::value_ptr(material.Ks));
		glUniform1f(materialUniform.ShininessLocation, material.shininess);
	}
}
