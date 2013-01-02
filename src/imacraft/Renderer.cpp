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
	Renderer::Renderer(CubeInstance* cubeModel, TerrainGrid* grid, std::vector<imacraft::Texture> &vecTextures){
		m_pCubeModel = cubeModel;
		m_pGrid = grid;
		m_vecTextures = vecTextures;
	}
	
	Renderer::~Renderer(){
	}
	
	void Renderer::render(glm::mat4& P, MatrixStack& vs, GLuint PLocation){
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(P));
	
		std::vector<glm::mat4> vecModelMatrix1;
		std::vector<glm::mat4> vecModelMatrix2;
	
		for(uint16_t i=0;i<m_pGrid->width();++i){
			for(uint16_t j=0;j<m_pGrid->height();++j){
				for(uint16_t k=0;k<m_pGrid->width();++k){
					uint32_t currentCube = k*m_pGrid->width()*m_pGrid->height() + j*m_pGrid->width() + i;
					/* If there is a bloc */
					if((*m_pGrid)[currentCube] != 0){
						vs.push();
							/* Compute the MV matrix*/
							vs.translate(glm::vec3(CUBE_SIZE*i-1. + 2*(*m_pGrid).getEastPos(), CUBE_SIZE*j-1., CUBE_SIZE*k-1. + 2*(*m_pGrid).getNorthPos())); // offset (north & east positions)
							vs.scale(glm::vec3(CUBE_SIZE));
							
							if(k%2){ // condition to define different sets of blocks (determines the texture too)
								vecModelMatrix1.push_back(vs.top());
							}else{
								vecModelMatrix2.push_back(vs.top());
							}
							
						vs.pop();
					}
				}
			}
		}
		uint32_t drawedCubeCount1 = vecModelMatrix1.size();
		glm::mat4* MVMatrices1 = new glm::mat4[drawedCubeCount1];
		
		uint32_t drawedCubeCount2 = vecModelMatrix2.size();
		glm::mat4* MVMatrices2 = new glm::mat4[drawedCubeCount2];
		
		for(uint32_t i=0;i<drawedCubeCount1;++i){
			MVMatrices1[i] = vecModelMatrix1[i];
		}
		
		for(uint32_t i=0;i<drawedCubeCount2;++i){
			MVMatrices2[i] = vecModelMatrix2[i];
		}
		
		/* Draw the blocs */
		// here, assign textures to the matching set of blocks
		(*m_pCubeModel).setTexture(m_vecTextures[0]); // assign corresponding texture
		m_pCubeModel->draw(drawedCubeCount1, MVMatrices1);
		
		(*m_pCubeModel).setTexture(m_vecTextures[1]); // assign corresponding texture
		m_pCubeModel->draw(drawedCubeCount2, MVMatrices2);
		
		delete[] MVMatrices1;
		delete[] MVMatrices2;
	}
}
