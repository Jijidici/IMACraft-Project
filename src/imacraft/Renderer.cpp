#include "imacraft/Renderer.hpp"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imacraft/MatrixStack.hpp"
#include "imacraft/Player.hpp"
#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/TerrainGrid.hpp"
#include "imacraft/Skybox.hpp"
#include "imacraft/lighting/LightManager.hpp"

#define GROUND 0
#define STONE 1
#define SKY 2
#define TORCH 4

namespace imacraft{
	Renderer::Renderer(CubeInstance* cubeModel, QuadInstance* quadModel, std::vector<TerrainGrid*> &vecGrid, std::vector<Texture*> &vecTextures, Skybox& inSky): 
		m_pCubeModel(cubeModel), m_pQuadModel(quadModel), m_vecGrid(vecGrid), m_vecTextures(vecTextures), m_sky(inSky){
	}
	
	Renderer::~Renderer(){
	}
	
	bool Renderer::writeAllFiles(){
		for(size_t i = 0; i < m_vecGrid.size(); ++i){
			if(!(*m_vecGrid[i]).writeFile("terrain_imacraft_")) return false;
		}
		
		return true;
	}
	
	void Renderer::render(GLuint program, glm::mat4& P, MatrixStack& vs, GLuint PLocation, Player& player, LightManager& lMage){
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(P));
	
		std::vector<glm::mat4> vecModelMatrix1;
		std::vector<glm::mat4> vecModelMatrix2;
		
		TerrainGrid *currentGrid = m_vecGrid[0];
		
		for(size_t i = 0; i < m_vecGrid.size(); ++i){
			currentGrid = m_vecGrid[i];
			
			for(uint16_t i=0;i<currentGrid->width();++i){
				for(uint16_t j=0;j<currentGrid->height();++j){
					for(uint16_t k=0;k<currentGrid->width();++k){
						uint32_t currentCube = k*currentGrid->width()*currentGrid->height() + j*currentGrid->width() + i;
						/* If there is a bloc */
						if((*currentGrid)[currentCube] != 0){
							if(player.frustumTest(i, j, k, (*currentGrid).width(), (*currentGrid).getNorthPos(), (*currentGrid).getEastPos())){ // if if the camera's field of view
								vs.push();
									/* Compute the MV matrix*/
									vs.translate(glm::vec3(CUBE_SIZE*i-1. - 2*(*currentGrid).getEastPos(), CUBE_SIZE*j-1., CUBE_SIZE*k-1. + 2*(*currentGrid).getNorthPos())); // offset (north & east positions)
									vs.scale(glm::vec3(CUBE_SIZE));
									
									if((*currentGrid)[currentCube] == 1){ // condition to define different sets of blocks (determines the texture too) => replace by types written in the binary file
										vecModelMatrix1.push_back(vs.top());
									}else if((*currentGrid)[currentCube] == 2){
										vecModelMatrix2.push_back(vs.top());
									}
									
								vs.pop();
							}
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
		m_pCubeModel->setTexture(m_vecTextures[GROUND]); // assign corresponding texture
		m_pCubeModel->draw(drawedCubeCount1, MVMatrices1);
		
		m_pCubeModel->setTexture(m_vecTextures[STONE]); // assign corresponding texture
		m_pCubeModel->draw(drawedCubeCount2, MVMatrices2);
		
		delete[] MVMatrices1;
		delete[] MVMatrices2;
		
		
		/* ALL UNLIGHTED ELEMENTS */ 
		//get the checker location
		GLuint LightedLocation = glGetUniformLocation(program, "is_not_lighted");
		//set the ligthed flag to true
		glUniform1i(LightedLocation, 1);
		
		/* Draw the torchs */
		glm::mat4 MVMatricesTorches[16];
		for(uint16_t idx=0;idx<lMage.getNbPointLight();++idx){
			vs.push();
				/* Compute the MVMatrix */
				vs.translate(lMage.getPointLightPos(idx));
				vs.set(getBillboardedMatrix(vs.top()));
				vs.scale(glm::vec3(CUBE_SIZE/4.));
				MVMatricesTorches[idx] = vs.top();
			vs.pop();
		}
		m_pQuadModel->setTexture(m_vecTextures[TORCH]);
		m_pQuadModel->draw(lMage.getNbPointLight(), MVMatricesTorches);
		
		
		//draw the skybox
		m_pCubeModel->setTexture(m_vecTextures[SKY]); // assign corresponding texture
		m_sky.draw();
		
		//set the ligthed flag to false
		glUniform1i(LightedLocation, 0);
	} // end render()
	
	
	glm::mat4 Renderer::getBillboardedMatrix(glm::mat4 matrix){
		for(int i=0;i<3;++i){
			for(int j=0;j<3;++j){
				if(i==j){
					matrix[i][j]=1;
				}else{
					matrix[i][j]=0;
				}
			}
		}
		return matrix;
	}
}
