#include "imacraft/Renderer.hpp"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imacraft/MatrixStack.hpp"
#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/TerrainGrid.hpp"

namespace imacraft{
	Renderer::Renderer(CubeInstance* cubeModel, TerrainGrid* grid){
		m_pCubeModel = cubeModel;
		m_pGrid = grid;
	}
	
	Renderer::~Renderer(){
	}
	
	void Renderer::render(glm::mat4& P, MatrixStack& vs, GLuint PLocation){
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(P));
	
		std::vector<glm::mat4> vecModelMatrix;
	
		for(uint16_t i=0;i<m_pGrid->width();++i){
			for(uint16_t j=0;j<m_pGrid->height();++j){
				for(uint16_t k=0;k<m_pGrid->width();++k){
					uint32_t currentCube = k*m_pGrid->width()*m_pGrid->height() + j*m_pGrid->width() + i;
					
					/* If there is a bloc */
					if((*m_pGrid)[currentCube] != 0){
						vs.push();
							/* Compute the MV matrix*/
							vs.translate(glm::vec3(CUBE_SIZE*i-1., CUBE_SIZE*j-1., CUBE_SIZE*k-1.));
							vs.scale(glm::vec3(CUBE_SIZE));
							vecModelMatrix.push_back(vs.top());
						vs.pop();
					}
				}
			}
		}
		uint32_t drawedCubeCount = vecModelMatrix.size();
		glm::mat4* MVMatrices = new glm::mat4[drawedCubeCount];
		for(uint32_t i=0;i<drawedCubeCount;++i){
			MVMatrices[i] = vecModelMatrix[i];
		}
		
		/* Draw the blocs */
		m_pCubeModel->draw(drawedCubeCount, MVMatrices);
		
		
		delete[] MVMatrices;
	}
}
