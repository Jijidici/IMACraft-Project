#include "imacraft/Renderer.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imacraft/MatrixStack.hpp"
#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/TerrainGrid.hpp"

namespace imacraft{
	Renderer::Renderer(CubeInstance& cubeModel, TerrainGrid& grid){
		m_cubeModel = cubeModel;
		m_grid = grid;
	}
	
	Renderer::~Renderer(){}
	
	void Renderer::render(MatrixStack& ms, GLuint MVPLocation){
		uint32_t nbCubes = m_grid.length();
		for(uint32_t i=0;i<nbCubes;++i){
			if(m_grid[i] != 0){
				ms.push();
					float posZ = i / m_grid.width()*m_grid.height();
					uint32_t reste = i % m_grid.width()*m_grid.height();
					float posY = reste / m_grid.height();
					float posX = reste % m_grid.height();
					ms.translate(glm::vec3(posX, posY, posZ));
					glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(ms.top()));
					m_cubeModel.draw();
				ms.pop();
			}
		}
	}
}
