#ifndef __CUBE_INSTANCE_HPP__
#define __CUBE_INSTANCE_HPP__

#include <GL/glew.h>
#include <glm/glm.hpp>

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
		
		private:
			GLuint vbo;
			GLuint vao;
			GLuint vertexCount;
			
		public:
			CubeInstance();
			~CubeInstance();
			void draw();
	};
	
	struct ShapeVertex {
		glm::vec3 position, normal;
		glm::vec2 texCoords;
	};
}

#endif
