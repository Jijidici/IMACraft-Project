#ifndef __LIGHTS_HPP__
#define __LIGHTS_HPP__

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace imacraft{
	struct DirectionalLight{
		glm::vec3 dir;
		float i;
		
		DirectionalLight(const glm::vec3 inDir, const float inI);
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
