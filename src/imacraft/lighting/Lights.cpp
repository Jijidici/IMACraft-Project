#include "imacraft/lighting/Lights.hpp"

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace imacraft{
	DirectionalLight::DirectionalLight(const glm::vec3 inDir, const float inI) : 
		dir(inDir), i(inI){
	}
	
	DirectionalLightUniform::DirectionalLightUniform() : 
		DirLocation(0), ILocation(0){
	}
	
	void DirectionalLightUniform::getLocations(const char* uniform, const GLuint program){
		DirLocation = glGetUniformLocation(program, (std::string(uniform)+".dir").c_str());
		ILocation = glGetUniformLocation(program, (std::string(uniform)+".i").c_str());
	}
	
	void sendDirectionalLight(const DirectionalLight& dl, const DirectionalLightUniform& dlUniform){
		glUniform3fv(dlUniform.DirLocation, 1, glm::value_ptr(dl.dir));
		glUniform1f(dlUniform.ILocation, dl.i);
	}
}
