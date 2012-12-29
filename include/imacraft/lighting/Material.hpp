#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace imacraft{
	struct Material{
		const glm::vec3 Ka;
		const glm::vec3 Kd;
		const glm::vec3 Ks;
		const float shininess;
		
		Material(glm::vec3 inKa, glm::vec3 inKd, glm::vec3 inKs, float inShininess);
	};
	
	struct MaterialUniform{
		GLint KaLocation;
		GLint KdLocation;
		GLint KsLocation;
		GLint ShininessLocation;
		
		MaterialUniform();
		void getLocations(const char* uniform, const GLuint program);
	};
	
	/* Envoie du materiau a la carte graphique */
	void sendMaterial(const Material& material, const MaterialUniform& materialUniform);
}

#endif
