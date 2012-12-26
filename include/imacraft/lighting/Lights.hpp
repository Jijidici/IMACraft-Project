#ifndef __LIGHTS_HPP__
#define __LIGHTS_HPP__

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace imacraft{
	struct DirectionalLight{
		glm::vec4 dir;
		glm::vec3 i;
		
		DirectionalLight(const glm::vec4 inDir, const glm::vec3 inI);
	};
	
	struct DirectionalLightUniform{
		GLint DirLocation;
		GLint ILocation;
		
		DirectionalLightUniform();
		void getLocations(const char* uniform, const GLuint program);
	};
	
	/* Envoie à la carte graphique */
	void sendDirectionalLight(const DirectionalLight& dl, const DirectionalLightUniform& dlUniform);
}

#endif
