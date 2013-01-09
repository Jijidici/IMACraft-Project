#include "imacraft/shapes/QuadInstance.hpp"

#include "imacraft/shapes/CommonInstance.hpp"


namespace imacraft{
	QuadInstance::QuadInstance(Texture &texture) : quadTexture(texture){
		vertexCount = 6;
		ShapeVertex* vertices = new ShapeVertex[vertexCount];
		
		GLfloat hS = 0.5f;
		
		//Face
		vertices[0].position.x = -hS; vertices[0].position.y = -hS; vertices[0].position.z = 0.f;
		vertices[0].normal.x = 0.f; vertices[0].normal.y = 0.f; vertices[0].normal.z = 1.f;
		vertices[0].texCoords.x = 0.f; vertices[0].texCoords.y = 0.f; 
		
		vertices[1].position.x = hS; vertices[1].position.y = -hS; vertices[1].position.z = 0.f;
		vertices[1].normal.x = 0.f; vertices[1].normal.y = 0.f; vertices[1].normal.z = 1.f;
		vertices[1].texCoords.x = 1.f; vertices[1].texCoords.y = 0.f; 
		
		vertices[2].position.x = hS; vertices[2].position.y = hS; vertices[2].position.z = 0.f;
		vertices[2].normal.x = 0.f; vertices[2].normal.y = 0.f; vertices[2].normal.z = 1.f;
		vertices[2].texCoords.x = 1.f; vertices[2].texCoords.y = 1.f; 
		
		vertices[3].position.x = hS; vertices[3].position.y = hS; vertices[3].position.z = 0.f;
		vertices[3].normal.x = 0.f; vertices[3].normal.y = 0.f; vertices[3].normal.z = 1.f;
		vertices[3].texCoords.x = 1.f; vertices[3].texCoords.y = 1.f; 
		
		vertices[4].position.x = -hS; vertices[4].position.y = hS; vertices[4].position.z = 0.f;
		vertices[4].normal.x = 0.f; vertices[4].normal.y = 0.f; vertices[4].normal.z = 1.f;
		vertices[4].texCoords.x = 0.f; vertices[4].texCoords.y = 1.f; 
		
		vertices[5].position.x = -hS; vertices[5].position.y = -hS; vertices[5].position.z = 0.f;
		vertices[5].normal.x = 0.f; vertices[5].normal.y = 0.f; vertices[5].normal.z = 1.f;
		vertices[5].texCoords.x = 0.f; vertices[5].texCoords.y = 0.f; 
		
		//Creation du VBO
		vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertexCount*VERTEX_BYTE_SIZE, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		//generate the modelView matrices vbo
		MVvbo = 0;
		glGenBuffers(1, &MVvbo);
		
		//Creation du VAO
		vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
			glEnableVertexAttribArray(POSITION_LOCATION);
			glEnableVertexAttribArray(NORMAL_LOCATION);
			glEnableVertexAttribArray(TEXCOORDS_LOCATION);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glVertexAttribPointer(POSITION_LOCATION, POSITION_NUM_COMPONENTS, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, reinterpret_cast<const GLvoid*>(POSITION_OFFSET));
				glVertexAttribPointer(NORMAL_LOCATION, NORMAL_NUM_COMPONENTS, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, reinterpret_cast<const GLvoid*>(NORMAL_OFFSET));
				glVertexAttribPointer(TEXCOORDS_LOCATION, TEXCOORDS_NUM_COMPONENTS, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, reinterpret_cast<const GLvoid*>(TEXCOORDS_OFFSET));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			/* Active the Model Matrix as an attribute in the VS */
			for(int i=0;i<4;++i){
				glEnableVertexAttribArray(MATRIXMODEL_LOCATION + i); 
			}
			glBindBuffer(GL_ARRAY_BUFFER, MVvbo);
				for(int i=0;i<4;++i){
					glVertexAttribPointer(MATRIXMODEL_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<const GLvoid*>(sizeof(GLfloat) * i * 4));
					glVertexAttribDivisor(MATRIXMODEL_LOCATION +i, 1);
				}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		delete[] vertices;
	}
	
	//DESTRUCTOR
	QuadInstance::~QuadInstance(){
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &MVvbo);
		glDeleteVertexArrays(1, &vao);
	}
	
	//Dessin
	void QuadInstance::draw(uint32_t nbInstances, glm::mat4* MVMatrices){
		glBindBuffer(GL_ARRAY_BUFFER, MVvbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*nbInstances, MVMatrices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		quadTexture.bindTexture();
		glBindVertexArray(vao);
			glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, nbInstances);
		glBindVertexArray(0);
		quadTexture.debindTexture();
	}
	
	void QuadInstance::setTexture(Texture &texture){
		quadTexture = texture;
	}
}
