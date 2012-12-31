#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/TerrainGrid.hpp"
#include "imacraft/MatrixStack.hpp"

#define CUBE_SIZE 0.015625

namespace imacraft{
	struct Renderer{
	
		private:
			CubeInstance* m_pCubeModel;
			TerrainGrid* m_pGrid;
		
		public:
			Renderer(CubeInstance* cubeModel, TerrainGrid* grid);
			~Renderer();
			void render(glm::mat4& P, MatrixStack& vs, GLuint PLocation);
	};
}

#endif // _RENDERER_HPP_
