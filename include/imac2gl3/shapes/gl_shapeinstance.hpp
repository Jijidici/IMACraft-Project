#ifndef __DEF__GL_SHAPE_INSTANCE__
#define __DEF__GL_SHAPE_INSTANCE__

#include <GL/glew.h>
#include "imac2gl3/shapes/Sphere.hpp"
#include "imac2gl3/shapes/Cone.hpp"
#include "imac2gl3/shapes/Cylinder.hpp"
#include "imac2gl3/shapes/Cube.hpp"

namespace imac2gl3{

	class GLShapeInstance{
		private:
			GLuint vbo;
			GLuint vao;
			GLuint vertexCount;
		
		public:
			GLShapeInstance();
			GLShapeInstance(const imac2gl3::Sphere &inSphere);
			GLShapeInstance(const imac2gl3::Cone &inCone);
			GLShapeInstance(const imac2gl3::Cylinder &inCylinder);
			GLShapeInstance(const imac2gl3::Cube &inCube);
			~GLShapeInstance();
		
			void draw();
	};

}

#endif
