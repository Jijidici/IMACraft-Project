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
		static const GLint MATRIXMODEL_LOCATION = 3;
	
		private:
			CubeInstance* m_pCubeModel;
			TerrainGrid* m_pGrid;
			GLuint m_mMvbo;
			GLuint m_mMvao;
		
		public:
			Renderer(CubeInstance* cubeModel, TerrainGrid* grid);
			~Renderer();
			void render(MatrixStack& ms, MatrixStack& vs, GLuint MVPLocation, GLuint MVLocation, GLuint NormalLocation);
	};
}

#endif // _RENDERER_HPP_
