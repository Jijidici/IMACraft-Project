#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/TerrainGrid.hpp"
#include "imacraft/MatrixStack.hpp"
#include "imacraft/Texture.hpp"
#include "imacraft/Skybox.hpp"

#define CUBE_SIZE 0.015625

namespace imacraft{
	struct Renderer{
		private:
			CubeInstance* m_pCubeModel;
			std::vector<TerrainGrid*> m_vecGrid;
			std::vector<Texture> m_vecTextures;
			Skybox m_sky;
		
		public:
			Renderer(CubeInstance* cubeModel, std::vector<TerrainGrid*> &vecGrid, std::vector<Texture> &vecTextures, Skybox& inSky);
			~Renderer();
			void render(glm::mat4& P, MatrixStack& vs, GLuint PLocation);
			bool writeAllFiles();
	};
}

#endif // _RENDERER_HPP_
