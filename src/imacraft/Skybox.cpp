#include "imacraft/Skybox.hpp"

#include <iostream>
#include <string>
#include <stdexcept>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imacraft/Player.hpp"
#include "imacraft/MatrixStack.hpp"
#include "imacraft/Texture.hpp"
#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/shader_tools.hpp"

namespace imacraft{
	
	Skybox::Skybox(GLuint program, Player* player, CubeInstance* pModel):
		m_program(program), m_pModel(pModel), m_pCamera(player){
	}
	
	Skybox::~Skybox(){
	}
	
	void Skybox::draw(){		
		//computing the MV Matrix
		MatrixStack mvs;
		mvs.mult(m_pCamera->getViewMatrix());
		mvs.translate(m_pCamera->getPosition());
		mvs.scale(glm::vec3(2.f, 2.f, 2.f));
		
		//get the checker location
		GLuint SkyFlagLocation = glGetUniformLocation(m_program, "is_not_lighted");
		//set the sky flag to true
		glUniform1i(SkyFlagLocation, 1);
		
		glm::mat4 MVMatrix = mvs.top();
		m_pModel->draw(uint32_t(1), &MVMatrix);
		
		//set the sky flag to false
		glUniform1i(SkyFlagLocation, 0);
	}
}
