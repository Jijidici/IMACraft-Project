#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/shapes/QuadInstance.hpp"
#include "imacraft/Player.hpp"
#include "imacraft/TerrainGrid.hpp"
#include "imacraft/MatrixStack.hpp"
#include "imacraft/Texture.hpp"
#include "imacraft/Skybox.hpp"
#include "imacraft/lighting/LightManager.hpp"

namespace imacraft{
	struct Renderer{
		private:
			CubeInstance* m_pCubeModel;
			QuadInstance* m_pQuadModel;
			std::vector<TerrainGrid*> m_vecGrid;
			std::vector<Texture*> m_vecTextures;
			Skybox m_sky;
			
			glm::mat4 getBillboardedMatrix(glm::mat4 matrix);
		
		public:
			Renderer(CubeInstance* cubeModel, QuadInstance* quadModel, std::vector<TerrainGrid*> &vecGrid, std::vector<Texture*> &vecTextures, Skybox& inSky);
			~Renderer();
			void render(GLuint program, glm::mat4& P, MatrixStack& vs, GLuint PLocation, Player& player, LightManager& lMage);
			bool writeAllFiles();
	};
}

#endif // _RENDERER_HPP_
