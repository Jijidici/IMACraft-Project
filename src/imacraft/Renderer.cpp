#include "imacraft/Renderer.hpp"

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imacraft/MatrixStack.hpp"
#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/TerrainGrid.hpp"

namespace imacraft{
	Renderer::Renderer(CubeInstance* cubeModel, TerrainGrid* grid){
		m_pCubeModel = cubeModel;
		m_pGrid = grid;
	}
	
	Renderer::~Renderer(){}
	
	void Renderer::render(MatrixStack& ms, GLuint MVPLocation){
		for(uint16_t i=0;i<m_pGrid->width();++i){
			for(uint16_t j=0;j<m_pGrid->height();++j){
				for(uint16_t k=0;k<m_pGrid->width();++k){
					uint32_t currentCube = k*m_pGrid->width()*m_pGrid->height() + j*m_pGrid->width() + i;
					if((*m_pGrid)[currentCube] != 0){
						ms.push();
							ms.translate(glm::vec3(CUBE_SIZE*i, CUBE_SIZE*j, CUBE_SIZE*k));
							ms.scale(glm::vec3(CUBE_SIZE));
							glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(ms.top()));
							m_pCubeModel->draw();
						ms.pop();
					}
				}
			}
		}
	}
}
