#include "imacraft/Renderer.hpp"

#include <iostream>
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
		
		//generate the model matrices vbo
		m_mMvbo = 0;
		glGenBuffers(1, &m_mMvbo);
		
		//generate and init the model matrices vao
		m_mMvao = 0;
		glGenVertexArrays(1, &m_mMvao);
		glBindVertexArray(m_mMvao);
			/* Active the Model Matrix as an attribute in the VS */
			for(int i=0;i<4;++i){
				glEnableVertexAttribArray(MATRIXMODEL_LOCATION + i); 
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_mMvbo);
				for(int i=0;i<4;++i){
					glVertexAttribPointer(MATRIXMODEL_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
					glVertexAttribDivisor(MATRIXMODEL_LOCATION +i, 1);
				}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	Renderer::~Renderer(){
		glDeleteBuffers(1, &m_mMvbo);
		glDeleteVertexArrays(1, &m_mMvao);
	}
	
	void Renderer::render(MatrixStack& ms, MatrixStack& vs, GLuint MVPLocation, GLuint MVLocation, GLuint NormalLocation){
		for(uint16_t i=0;i<m_pGrid->width();++i){
			for(uint16_t j=0;j<m_pGrid->height();++j){
				for(uint16_t k=0;k<m_pGrid->width();++k){
					uint32_t currentCube = k*m_pGrid->width()*m_pGrid->height() + j*m_pGrid->width() + i;
					
					/* If there is a bloc */
					if((*m_pGrid)[currentCube] != 0){
						
						vs.push();
							/* Compute the MV matrix */
							vs.translate(glm::vec3(CUBE_SIZE*i-1., CUBE_SIZE*j-1., CUBE_SIZE*k-1.));
							vs.scale(glm::vec3(CUBE_SIZE));
							glUniformMatrix4fv(MVLocation, 1, GL_FALSE, glm::value_ptr(vs.top()));
							
							/* Compute the Normal matrix */
							glm::mat4 normalMatrix = glm::transpose(glm::inverse(vs.top()));
							glUniformMatrix4fv(NormalLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
							
							/* Compute the MVP matrix*/
							ms.push();
								ms.mult(vs.top());
								glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(ms.top()));
							ms.pop();
						vs.pop();
						
						/* Draw the bloc */
						m_pCubeModel->draw();
					}
				}
			}
		}
	}
}
