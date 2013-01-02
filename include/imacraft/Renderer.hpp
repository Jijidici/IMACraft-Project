#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/TerrainGrid.hpp"
#include "imacraft/MatrixStack.hpp"
#include "imacraft/Texture.hpp"

#define CUBE_SIZE 0.015625

namespace imacraft{
	struct Renderer{
	
		private:
			CubeInstance* m_pCubeModel;
			TerrainGrid* m_pGrid;
			std::vector<imacraft::Texture> m_vecTextures;
		
		public:
			Renderer(CubeInstance* cubeModel, TerrainGrid* grid, std::vector<imacraft::Texture> &vecTextures);
			~Renderer();
			void render(glm::mat4& P, MatrixStack& vs, GLuint PLocation);
	};
}

#endif // _RENDERER_HPP_
