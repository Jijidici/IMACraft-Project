#include "imacraft/shapes/CubeInstance.hpp"

#include <GL/glew.h>

namespace imacraft{
	CubeInstance::CubeInstance(){
		vertexCount = 36;
		ShapeVertex* vertices = new ShapeVertex[vertexCount];
		
		GLfloat hS = 0.5f;
		
		//Face avant
		vertices[0].position.x = -hS; vertices[0].position.y = -hS; vertices[0].position.z = hS;
		vertices[0].normal.x = 0.f; vertices[0].normal.y = 0.f; vertices[0].normal.z = 1.f;
		vertices[0].texCoords.x = 0.f; vertices[0].texCoords.y = 1.f; 
		
		vertices[1].position.x = hS; vertices[1].position.y = -hS; vertices[1].position.z = hS;
		vertices[1].normal.x = 0.f; vertices[1].normal.y = 0.f; vertices[1].normal.z = 1.f;
		vertices[1].texCoords.x = 1.f; vertices[1].texCoords.y = 1.f; 
		
		vertices[2].position.x = hS; vertices[2].position.y = hS; vertices[2].position.z = hS;
		vertices[2].normal.x = 0.f; vertices[2].normal.y = 0.f; vertices[2].normal.z = 1.f;
		vertices[2].texCoords.x = 1.f; vertices[2].texCoords.y = 0.f; 
		
		vertices[3].position.x = hS; vertices[3].position.y = hS; vertices[3].position.z = hS;
		vertices[3].normal.x = 0.f; vertices[3].normal.y = 0.f; vertices[3].normal.z = 1.f;
		vertices[3].texCoords.x = 1.f; vertices[3].texCoords.y = 0.f; 
		
		vertices[4].position.x = -hS; vertices[4].position.y = hS; vertices[4].position.z = hS;
		vertices[4].normal.x = 0.f; vertices[4].normal.y = 0.f; vertices[4].normal.z = 1.f;
		vertices[4].texCoords.x = 0.f; vertices[4].texCoords.y = 0.f; 
		
		vertices[5].position.x = -hS; vertices[5].position.y = -hS; vertices[5].position.z = hS;
		vertices[5].normal.x = 0.f; vertices[5].normal.y = 0.f; vertices[5].normal.z = 1.f;
		vertices[5].texCoords.x = 0.f; vertices[5].texCoords.y = 1.f; 
		
		
		//Face arriere
		vertices[6].position.x = -hS; vertices[6].position.y = -hS; vertices[6].position.z = -hS;
		vertices[6].normal.x = 0.f; vertices[6].normal.y = 0.f; vertices[6].normal.z = -1.f;
		vertices[6].texCoords.x = 1.f; vertices[6].texCoords.y = 1.f; 
		
		vertices[7].position.x = hS; vertices[7].position.y = -hS; vertices[7].position.z = -hS;
		vertices[7].normal.x = 0.f; vertices[7].normal.y = 0.f; vertices[7].normal.z = -1.f;
		vertices[7].texCoords.x = 0.f; vertices[7].texCoords.y = 1.f; 
		
		vertices[8].position.x = hS; vertices[8].position.y = hS; vertices[8].position.z = -hS;
		vertices[8].normal.x = 0.f; vertices[8].normal.y = 0.f; vertices[8].normal.z = -1.f;
		vertices[8].texCoords.x = 0.f; vertices[8].texCoords.y = 0.f; 
		
		vertices[9].position.x = hS; vertices[9].position.y = hS; vertices[9].position.z = -hS;
		vertices[9].normal.x = 0.f; vertices[9].normal.y = 0.f; vertices[9].normal.z = -1.f;
		vertices[9].texCoords.x = 0.f; vertices[9].texCoords.y = 0.f; 
		
		vertices[10].position.x = -hS; vertices[10].position.y = hS; vertices[10].position.z = -hS;
		vertices[10].normal.x = 0.f; vertices[10].normal.y = 0.f; vertices[10].normal.z = -1.f;
		vertices[10].texCoords.x = 1.f; vertices[10].texCoords.y = 0.f; 
		
		vertices[11].position.x = -hS; vertices[11].position.y = -hS; vertices[11].position.z = -hS;
		vertices[11].normal.x = 0.f; vertices[11].normal.y = 0.f; vertices[11].normal.z = -1.f;
		vertices[11].texCoords.x = 1.f; vertices[11].texCoords.y = 1.f; 
		
		
		//Face gauche
		vertices[12].position.x = -hS; vertices[12].position.y = -hS; vertices[12].position.z = -hS;
		vertices[12].normal.x = -1.f; vertices[12].normal.y = 0.f; vertices[12].normal.z = 0.f;
		vertices[12].texCoords.x = 0.f; vertices[12].texCoords.y = 1.f; 
		
		vertices[13].position.x = -hS; vertices[13].position.y = -hS; vertices[13].position.z = hS;
		vertices[13].normal.x = -1.f; vertices[13].normal.y = 0.f; vertices[13].normal.z = 0.f;
		vertices[13].texCoords.x = 1.f; vertices[13].texCoords.y = 1.f; 
		
		vertices[14].position.x = -hS; vertices[14].position.y = hS; vertices[14].position.z = hS;
		vertices[14].normal.x = -1.f; vertices[14].normal.y = 0.f; vertices[14].normal.z = 0.f;
		vertices[14].texCoords.x = 1.f; vertices[14].texCoords.y = 0.f; 
		
		vertices[15].position.x = -hS; vertices[15].position.y = hS; vertices[15].position.z = hS;
		vertices[15].normal.x = -1.f; vertices[15].normal.y = 0.f; vertices[15].normal.z = 0.f;
		vertices[15].texCoords.x = 1.f; vertices[15].texCoords.y = 0.f; 
		
		vertices[16].position.x = -hS; vertices[16].position.y = hS; vertices[16].position.z = -hS;
		vertices[16].normal.x = -1.f; vertices[16].normal.y = 0.f; vertices[16].normal.z = 0.f;
		vertices[16].texCoords.x = 0.f; vertices[16].texCoords.y = 0.f; 
		
		vertices[17].position.x = -hS; vertices[17].position.y = -hS; vertices[17].position.z = -hS;
		vertices[17].normal.x = -1.f; vertices[17].normal.y = 0.f; vertices[17].normal.z = 0.f;
		vertices[17].texCoords.x = 0.f; vertices[17].texCoords.y = 1.f; 
		
		
		//Face droite
		vertices[18].position.x = hS; vertices[18].position.y = -hS; vertices[18].position.z = -hS;
		vertices[18].normal.x = 1.f; vertices[18].normal.y = 0.f; vertices[18].normal.z = 0.f;
		vertices[18].texCoords.x = 1.f; vertices[18].texCoords.y = 1.f; 
		
		vertices[19].position.x = hS; vertices[19].position.y = -hS; vertices[19].position.z = hS;
		vertices[19].normal.x = 1.f; vertices[19].normal.y = 0.f; vertices[19].normal.z = 0.f;
		vertices[19].texCoords.x = 0.f; vertices[19].texCoords.y = 1.f; 
		
		vertices[20].position.x = hS; vertices[20].position.y = hS; vertices[20].position.z = hS;
		vertices[20].normal.x = 1.f; vertices[20].normal.y = 0.f; vertices[20].normal.z = 0.f;
		vertices[20].texCoords.x = 0.f; vertices[20].texCoords.y = 0.f; 
		
		vertices[21].position.x = hS; vertices[21].position.y = hS; vertices[21].position.z = hS;
		vertices[21].normal.x = 1.f; vertices[21].normal.y = 0.f; vertices[21].normal.z = 0.f;
		vertices[21].texCoords.x = 0.f; vertices[21].texCoords.y = 0.f; 
		
		vertices[22].position.x = hS; vertices[22].position.y = hS; vertices[22].position.z = -hS;
		vertices[22].normal.x = 1.f; vertices[22].normal.y = 0.f; vertices[22].normal.z = 0.f;
		vertices[22].texCoords.x = 1.f; vertices[22].texCoords.y = 0.f; 
		
		vertices[23].position.x = hS; vertices[23].position.y = -hS; vertices[23].position.z = -hS;
		vertices[23].normal.x = 1.f; vertices[23].normal.y = 0.f; vertices[23].normal.z = 0.f;
		vertices[23].texCoords.x = 1.f; vertices[23].texCoords.y = 1.f;
		
		
		//Face bas
		vertices[24].position.x = -hS; vertices[24].position.y = -hS; vertices[24].position.z = -hS;
		vertices[24].normal.x = 0.f; vertices[24].normal.y = -1.f; vertices[24].normal.z = 0.f;
		vertices[24].texCoords.x = 0.f; vertices[24].texCoords.y = 0.f; 
		
		vertices[25].position.x = hS; vertices[25].position.y = -hS; vertices[25].position.z = -hS;
		vertices[25].normal.x = 0.f; vertices[25].normal.y = -1.f; vertices[25].normal.z = 0.f;
		vertices[25].texCoords.x = 0.f; vertices[25].texCoords.y = 1.f; 
		
		vertices[26].position.x = hS; vertices[26].position.y = -hS; vertices[26].position.z = hS;
		vertices[26].normal.x = 0.f; vertices[26].normal.y = -1.f; vertices[26].normal.z = 0.f;
		vertices[26].texCoords.x = 1.f; vertices[26].texCoords.y = 1.f; 
		
		vertices[27].position.x = hS; vertices[27].position.y = -hS; vertices[27].position.z = hS;
		vertices[27].normal.x = 0.f; vertices[27].normal.y = -1.f; vertices[27].normal.z = 0.f;
		vertices[27].texCoords.x = 1.f; vertices[27].texCoords.y = 1.f; 
		
		vertices[28].position.x = -hS; vertices[28].position.y = -hS; vertices[28].position.z = hS;
		vertices[28].normal.x = 0.f; vertices[28].normal.y = -1.f; vertices[28].normal.z = 0.f;
		vertices[28].texCoords.x = 1.f; vertices[28].texCoords.y = 0.f; 
		
		vertices[29].position.x = -hS; vertices[29].position.y = -hS; vertices[29].position.z = -hS;
		vertices[29].normal.x = 0.f; vertices[29].normal.y = -1.f; vertices[29].normal.z = 0.f;
		vertices[29].texCoords.x = 0.f; vertices[29].texCoords.y = 0.f;
		
		
		//Face haute
		vertices[30].position.x = -hS; vertices[30].position.y = hS; vertices[30].position.z = -hS;
		vertices[30].normal.x = 0.f; vertices[30].normal.y = 1.f; vertices[30].normal.z = 0.f;
		vertices[30].texCoords.x = 1.f; vertices[30].texCoords.y = 1.f; 
		
		vertices[31].position.x = hS; vertices[31].position.y = hS; vertices[31].position.z = -hS;
		vertices[31].normal.x = 0.f; vertices[31].normal.y = 1.f; vertices[31].normal.z = 0.f;
		vertices[31].texCoords.x = 0.f; vertices[31].texCoords.y = 1.f; 
		
		vertices[32].position.x = hS; vertices[32].position.y = hS; vertices[32].position.z = hS;
		vertices[32].normal.x = 0.f; vertices[32].normal.y = 1.f; vertices[32].normal.z = 0.f;
		vertices[32].texCoords.x = 0.f; vertices[32].texCoords.y = 0.f; 
		
		vertices[33].position.x = hS; vertices[33].position.y = hS; vertices[33].position.z = hS;
		vertices[33].normal.x = 0.f; vertices[33].normal.y = 1.f; vertices[33].normal.z = 0.f;
		vertices[33].texCoords.x = 0.f; vertices[33].texCoords.y = 0.f; 
		
		vertices[34].position.x = -hS; vertices[34].position.y = hS; vertices[34].position.z = hS;
		vertices[34].normal.x = 0.f; vertices[34].normal.y = 1.f; vertices[34].normal.z = 0.f;
		vertices[34].texCoords.x = 1.f; vertices[34].texCoords.y = 0.f; 
		
		vertices[35].position.x = -hS; vertices[35].position.y = hS; vertices[35].position.z = -hS;
		vertices[35].normal.x = 0.f; vertices[35].normal.y = 1.f; vertices[35].normal.z = 0.f;
		vertices[35].texCoords.x = 1.f; vertices[35].texCoords.y = 1.f;
		
		//Creation du VBO
		vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertexCount*VERTEX_BYTE_SIZE, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
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
		glBindVertexArray(0);
		
		delete[] vertices;
	}
	
	//DESTRUCTOR
	CubeInstance::~CubeInstance(){
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
	
	//Dessin
	void CubeInstance::draw(uint32_t nbInstances){	
		glBindVertexArray(vao);
			glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, nbInstances);
		glBindVertexArray(0);
	}
}
