#include "imac2gl3/shapes/gl_shapeinstance.hpp"

static const size_t POSITION_LOCATION = 0;
static const size_t NORMAL_LOCATION = 1;
static const size_t TEXTCOORD_LOCATION = 2;

namespace imac2gl3{

	//Constructeur
	GLShapeInstance::GLShapeInstance(){
		this->vbo = 0;
		this->vao = 0;
		this->vertexCount = 0;
	}

	//Constructeur SPHERE
	GLShapeInstance::GLShapeInstance(const imac2gl3::Sphere &inSphere){
	
		//Creation du VBO
		glGenBuffers(1, &(this->vbo));	
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferData(GL_ARRAY_BUFFER, inSphere.getByteSize(), inSphere.getDataPointer(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		//Creation du VAO
		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);
			glEnableVertexAttribArray(POSITION_LOCATION);
			glEnableVertexAttribArray(NORMAL_LOCATION);
			glEnableVertexAttribArray(TEXTCOORD_LOCATION);
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
				glVertexAttribPointer(POSITION_LOCATION, inSphere.getPositionNumComponents(), inSphere.getDataType(), GL_FALSE, inSphere.getVertexByteSize(), inSphere.getPositionOffset());
		 		glVertexAttribPointer(NORMAL_LOCATION, inSphere.getNormalNumComponents(), inSphere.getDataType(), GL_FALSE, inSphere.getVertexByteSize(), inSphere.getNormalOffset());
		 		glVertexAttribPointer(TEXTCOORD_LOCATION, inSphere.getTexCoordsNumComponents(), inSphere.getDataType(), GL_FALSE, inSphere.getVertexByteSize(), inSphere.getTexCoordsOffset());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	
		this->vertexCount = inSphere.getVertexCount();
	}

	//Constructeur CONE
	GLShapeInstance::GLShapeInstance(const imac2gl3::Cone &inCone){
	
		//Creation du VBO
		glGenBuffers(1, &(this->vbo));	
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferData(GL_ARRAY_BUFFER, inCone.getByteSize(), inCone.getDataPointer(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		//Creation du VAO
		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);
			glEnableVertexAttribArray(POSITION_LOCATION);
			glEnableVertexAttribArray(NORMAL_LOCATION);
			glEnableVertexAttribArray(TEXTCOORD_LOCATION);
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
				glVertexAttribPointer(POSITION_LOCATION, inCone.getPositionNumComponents(), inCone.getDataType(), GL_FALSE, inCone.getVertexByteSize(), inCone.getPositionOffset());
		 		glVertexAttribPointer(NORMAL_LOCATION, inCone.getNormalNumComponents(), inCone.getDataType(), GL_FALSE, inCone.getVertexByteSize(), inCone.getNormalOffset());
		 		glVertexAttribPointer(TEXTCOORD_LOCATION, inCone.getTexCoordsNumComponents(), inCone.getDataType(), GL_FALSE, inCone.getVertexByteSize(), inCone.getTexCoordsOffset());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	
		this->vertexCount = inCone.getVertexCount();
	}

	//Constructeur CYLINDER
	GLShapeInstance::GLShapeInstance(const imac2gl3::Cylinder &inCylinder){
	
		//Creation du VBO
		glGenBuffers(1, &(this->vbo));	
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferData(GL_ARRAY_BUFFER, inCylinder.getByteSize(), inCylinder.getDataPointer(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		//Creation du VAO
		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);
			glEnableVertexAttribArray(POSITION_LOCATION);
			glEnableVertexAttribArray(NORMAL_LOCATION);
			glEnableVertexAttribArray(TEXTCOORD_LOCATION);
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
				glVertexAttribPointer(POSITION_LOCATION, inCylinder.getPositionNumComponents(), inCylinder.getDataType(), GL_FALSE, inCylinder.getVertexByteSize(), inCylinder.getPositionOffset());
		 		glVertexAttribPointer(NORMAL_LOCATION, inCylinder.getNormalNumComponents(), inCylinder.getDataType(), GL_FALSE, inCylinder.getVertexByteSize(), inCylinder.getNormalOffset());
		 		glVertexAttribPointer(TEXTCOORD_LOCATION, inCylinder.getTexCoordsNumComponents(), inCylinder.getDataType(), GL_FALSE, inCylinder.getVertexByteSize(), inCylinder.getTexCoordsOffset());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	
		this->vertexCount = inCylinder.getVertexCount();
	}
	
	//Constructeur Cube
	GLShapeInstance::GLShapeInstance(const imac2gl3::Cube &inCube){
	
		//Creation du VBO
		glGenBuffers(1, &(this->vbo));	
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferData(GL_ARRAY_BUFFER, inCube.getByteSize(), inCube.getDataPointer(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		//Creation du VAO
		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);
			glEnableVertexAttribArray(POSITION_LOCATION);
			glEnableVertexAttribArray(NORMAL_LOCATION);
			glEnableVertexAttribArray(TEXTCOORD_LOCATION);
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
				glVertexAttribPointer(POSITION_LOCATION, inCube.getPositionNumComponents(), inCube.getDataType(), GL_FALSE, inCube.getVertexByteSize(), inCube.getPositionOffset());
		 		glVertexAttribPointer(NORMAL_LOCATION, inCube.getNormalNumComponents(), inCube.getDataType(), GL_FALSE, inCube.getVertexByteSize(), inCube.getNormalOffset());
		 		glVertexAttribPointer(TEXTCOORD_LOCATION, inCube.getTexCoordsNumComponents(), inCube.getDataType(), GL_FALSE, inCube.getVertexByteSize(), inCube.getTexCoordsOffset());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	
		this->vertexCount = inCube.getVertexCount();
	}
	
	//Destructeur
	GLShapeInstance::~GLShapeInstance(){
		if(this->vertexCount != 0){
			glDeleteBuffers(1, &(this->vbo));
			glDeleteVertexArrays(1, &(this->vao));
		}
	}

	//fonction de dessin
	void GLShapeInstance::draw(){
		glBindVertexArray(this->vao);
			glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
		glBindVertexArray(0);
	}

}
