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
	Renderer::Renderer(CubeInstance* cubeModel, std::vector<TerrainGrid*> &vecGrid, std::vector<Texture> &vecTextures){
		m_pCubeModel = cubeModel;
		m_vecGrid = vecGrid;
		m_vecTextures = vecTextures;
	}
	
	Renderer::~Renderer(){
	}
	
	bool Renderer::writeAllFiles(){
		for(size_t i = 0; i < m_vecGrid.size(); ++i){
			if(!(*m_vecGrid[i]).writeFile("terrain_imacraft_")) return false;
		}
		
		return true;
	}
	
	void Renderer::render(glm::mat4& P, MatrixStack& vs, GLuint PLocation){
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(P));
	
		std::vector<glm::mat4> vecModelMatrix1;
		std::vector<glm::mat4> vecModelMatrix2;
		
		TerrainGrid *currentGrid = m_vecGrid[0];
		
		//~ for(size_t i = 0; i < m_vecGrid.size(); ++i){
		for(size_t i = 0; i < 3; ++i){ // remplacer par la ligne commentée au dessus
			
			currentGrid = m_vecGrid[i];
			
			for(uint16_t i=0;i<currentGrid->width();++i){
				for(uint16_t j=0;j<currentGrid->height();++j){
					for(uint16_t k=0;k<currentGrid->width();++k){
						uint32_t currentCube = k*currentGrid->width()*currentGrid->height() + j*currentGrid->width() + i;
						/* If there is a bloc */
						if((*currentGrid)[currentCube] != 0){
							vs.push();
								/* Compute the MV matrix*/
								vs.translate(glm::vec3(CUBE_SIZE*i-1. - 2*(*currentGrid).getEastPos(), CUBE_SIZE*j-1., CUBE_SIZE*k-1. + 2*(*currentGrid).getNorthPos())); // offset (north & east positions)
								vs.scale(glm::vec3(CUBE_SIZE));
								
								if(k%2){ // condition to define different sets of blocks (determines the texture too) => replace by types written in the binary file
									vecModelMatrix1.push_back(vs.top());
								}else{
									vecModelMatrix2.push_back(vs.top());
								}
								
							vs.pop();
						}
					}
				}
			}
			
		} // end for
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
	} // end render()
}
