#ifndef __CUBE_INSTANCE_HPP__
#define __CUBE_INSTANCE_HPP__

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "imacraft/Texture.hpp"

namespace imacraft{
	struct CubeInstance{
		static const GLint POSITION_NUM_COMPONENTS = 3;
		static const GLint NORMAL_NUM_COMPONENTS = 3;
		static const GLint TEXCOORDS_NUM_COMPONENTS = 2;
		
		static const GLsizei POSITION_OFFSET = 0;
		static const GLsizei NORMAL_OFFSET = POSITION_NUM_COMPONENTS * sizeof(GLfloat);
		static const GLsizei TEXCOORDS_OFFSET = (POSITION_NUM_COMPONENTS + NORMAL_NUM_COMPONENTS) * sizeof(GLfloat);
		
		static const GLsizei VERTEX_BYTE_SIZE = (POSITION_NUM_COMPONENTS + NORMAL_NUM_COMPONENTS + TEXCOORDS_NUM_COMPONENTS) * sizeof(GLfloat);
		
		static const GLint POSITION_LOCATION = 0;
		static const GLint NORMAL_LOCATION = 1;
		static const GLint TEXCOORDS_LOCATION = 2;
		static const GLint MATRIXMODEL_LOCATION = 3;
		
		private:
			GLuint vbo;
			GLuint MVvbo;
			GLuint vao;
			GLuint vertexCount;
			Texture cubeTexture;
			
		public:
			CubeInstance(Texture &texture);
			~CubeInstance();
			
			void draw(uint32_t nbInstances, glm::mat4* MVMatrices);
			void setTexture(Texture *texture);
			
	};
}

#endif
