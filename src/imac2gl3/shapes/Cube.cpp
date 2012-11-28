#include <vector>
#include "imac2gl3/shapes/Cube.hpp"
#include "imac2gl3/shapes/common.hpp"

namespace imac2gl3{

	Cube::Cube(GLfloat size){
		m_nVertexCount = 36;
		
		m_pDataPointer = new ShapeVertex[m_nVertexCount];
		
		GLfloat hS = size /2.f;
		
		//Face avant
		m_pDataPointer[0].position.x = -hS; m_pDataPointer[0].position.y = -hS; m_pDataPointer[0].position.z = hS;
		m_pDataPointer[0].normal.x = 0.f; m_pDataPointer[0].normal.y = 0.f; m_pDataPointer[0].normal.z = 1.f;
		m_pDataPointer[0].texCoords.x = 0.f; m_pDataPointer[0].texCoords.y = 0.f; 
		
		m_pDataPointer[1].position.x = hS; m_pDataPointer[1].position.y = -hS; m_pDataPointer[1].position.z = hS;
		m_pDataPointer[1].normal.x = 0.f; m_pDataPointer[1].normal.y = 0.f; m_pDataPointer[1].normal.z = 1.f;
		m_pDataPointer[1].texCoords.x = 1.f; m_pDataPointer[1].texCoords.y = 0.f; 
		
		m_pDataPointer[2].position.x = hS; m_pDataPointer[2].position.y = hS; m_pDataPointer[2].position.z = hS;
		m_pDataPointer[2].normal.x = 0.f; m_pDataPointer[2].normal.y = 0.f; m_pDataPointer[2].normal.z = 1.f;
		m_pDataPointer[2].texCoords.x = 1.f; m_pDataPointer[2].texCoords.y = 1.f; 
		
		m_pDataPointer[3].position.x = hS; m_pDataPointer[3].position.y = hS; m_pDataPointer[3].position.z = hS;
		m_pDataPointer[3].normal.x = 0.f; m_pDataPointer[3].normal.y = 0.f; m_pDataPointer[3].normal.z = 1.f;
		m_pDataPointer[3].texCoords.x = 1.f; m_pDataPointer[3].texCoords.y = 1.f; 
		
		m_pDataPointer[4].position.x = -hS; m_pDataPointer[4].position.y = hS; m_pDataPointer[4].position.z = hS;
		m_pDataPointer[4].normal.x = 0.f; m_pDataPointer[4].normal.y = 0.f; m_pDataPointer[4].normal.z = 1.f;
		m_pDataPointer[4].texCoords.x = 0.f; m_pDataPointer[4].texCoords.y = 1.f; 
		
		m_pDataPointer[5].position.x = -hS; m_pDataPointer[5].position.y = -hS; m_pDataPointer[5].position.z = hS;
		m_pDataPointer[5].normal.x = 0.f; m_pDataPointer[5].normal.y = 0.f; m_pDataPointer[5].normal.z = 1.f;
		m_pDataPointer[5].texCoords.x = 0.f; m_pDataPointer[5].texCoords.y = 0.f; 
		
		
		//Face arriere
		m_pDataPointer[6].position.x = -hS; m_pDataPointer[6].position.y = -hS; m_pDataPointer[6].position.z = -hS;
		m_pDataPointer[6].normal.x = 0.f; m_pDataPointer[6].normal.y = 0.f; m_pDataPointer[6].normal.z = -1.f;
		m_pDataPointer[6].texCoords.x = 1.f; m_pDataPointer[6].texCoords.y = 0.f; 
		
		m_pDataPointer[7].position.x = hS; m_pDataPointer[7].position.y = -hS; m_pDataPointer[7].position.z = -hS;
		m_pDataPointer[7].normal.x = 0.f; m_pDataPointer[7].normal.y = 0.f; m_pDataPointer[7].normal.z = -1.f;
		m_pDataPointer[7].texCoords.x = 0.f; m_pDataPointer[7].texCoords.y = 0.f; 
		
		m_pDataPointer[8].position.x = hS; m_pDataPointer[8].position.y = hS; m_pDataPointer[8].position.z = -hS;
		m_pDataPointer[8].normal.x = 0.f; m_pDataPointer[8].normal.y = 0.f; m_pDataPointer[8].normal.z = -1.f;
		m_pDataPointer[8].texCoords.x = 0.f; m_pDataPointer[8].texCoords.y = 1.f; 
		
		m_pDataPointer[9].position.x = hS; m_pDataPointer[9].position.y = hS; m_pDataPointer[9].position.z = -hS;
		m_pDataPointer[9].normal.x = 0.f; m_pDataPointer[9].normal.y = 0.f; m_pDataPointer[9].normal.z = -1.f;
		m_pDataPointer[9].texCoords.x = 0.f; m_pDataPointer[9].texCoords.y = 1.f; 
		
		m_pDataPointer[10].position.x = -hS; m_pDataPointer[10].position.y = hS; m_pDataPointer[10].position.z = -hS;
		m_pDataPointer[10].normal.x = 0.f; m_pDataPointer[10].normal.y = 0.f; m_pDataPointer[10].normal.z = -1.f;
		m_pDataPointer[10].texCoords.x = 1.f; m_pDataPointer[10].texCoords.y = 1.f; 
		
		m_pDataPointer[11].position.x = -hS; m_pDataPointer[11].position.y = -hS; m_pDataPointer[11].position.z = -hS;
		m_pDataPointer[11].normal.x = 0.f; m_pDataPointer[11].normal.y = 0.f; m_pDataPointer[11].normal.z = -1.f;
		m_pDataPointer[11].texCoords.x = 1.f; m_pDataPointer[11].texCoords.y = 0.f; 
		
		
		//Face gauche
		m_pDataPointer[12].position.x = -hS; m_pDataPointer[12].position.y = -hS; m_pDataPointer[12].position.z = -hS;
		m_pDataPointer[12].normal.x = -1.f; m_pDataPointer[12].normal.y = 0.f; m_pDataPointer[12].normal.z = 0.f;
		m_pDataPointer[12].texCoords.x = 0.f; m_pDataPointer[12].texCoords.y = 0.f; 
		
		m_pDataPointer[13].position.x = -hS; m_pDataPointer[13].position.y = -hS; m_pDataPointer[13].position.z = hS;
		m_pDataPointer[13].normal.x = -1.f; m_pDataPointer[13].normal.y = 0.f; m_pDataPointer[13].normal.z = 0.f;
		m_pDataPointer[13].texCoords.x = 1.f; m_pDataPointer[13].texCoords.y = 0.f; 
		
		m_pDataPointer[14].position.x = -hS; m_pDataPointer[14].position.y = hS; m_pDataPointer[14].position.z = hS;
		m_pDataPointer[14].normal.x = -1.f; m_pDataPointer[14].normal.y = 0.f; m_pDataPointer[14].normal.z = 0.f;
		m_pDataPointer[14].texCoords.x = 1.f; m_pDataPointer[14].texCoords.y = 1.f; 
		
		m_pDataPointer[15].position.x = -hS; m_pDataPointer[15].position.y = hS; m_pDataPointer[15].position.z = hS;
		m_pDataPointer[15].normal.x = -1.f; m_pDataPointer[15].normal.y = 0.f; m_pDataPointer[15].normal.z = 0.f;
		m_pDataPointer[15].texCoords.x = 1.f; m_pDataPointer[15].texCoords.y = 1.f; 
		
		m_pDataPointer[16].position.x = -hS; m_pDataPointer[16].position.y = hS; m_pDataPointer[16].position.z = -hS;
		m_pDataPointer[16].normal.x = -1.f; m_pDataPointer[16].normal.y = 0.f; m_pDataPointer[16].normal.z = 0.f;
		m_pDataPointer[16].texCoords.x = 0.f; m_pDataPointer[16].texCoords.y = 1.f; 
		
		m_pDataPointer[17].position.x = -hS; m_pDataPointer[17].position.y = -hS; m_pDataPointer[17].position.z = -hS;
		m_pDataPointer[17].normal.x = -1.f; m_pDataPointer[17].normal.y = 0.f; m_pDataPointer[17].normal.z = 0.f;
		m_pDataPointer[17].texCoords.x = 0.f; m_pDataPointer[17].texCoords.y = 0.f; 
		
		
		//Face droite
		m_pDataPointer[18].position.x = hS; m_pDataPointer[18].position.y = -hS; m_pDataPointer[18].position.z = -hS;
		m_pDataPointer[18].normal.x = 1.f; m_pDataPointer[18].normal.y = 0.f; m_pDataPointer[18].normal.z = 0.f;
		m_pDataPointer[18].texCoords.x = 1.f; m_pDataPointer[18].texCoords.y = 0.f; 
		
		m_pDataPointer[19].position.x = hS; m_pDataPointer[19].position.y = -hS; m_pDataPointer[19].position.z = hS;
		m_pDataPointer[19].normal.x = 1.f; m_pDataPointer[19].normal.y = 0.f; m_pDataPointer[19].normal.z = 0.f;
		m_pDataPointer[19].texCoords.x = 0.f; m_pDataPointer[19].texCoords.y = 0.f; 
		
		m_pDataPointer[20].position.x = hS; m_pDataPointer[20].position.y = hS; m_pDataPointer[20].position.z = hS;
		m_pDataPointer[20].normal.x = 1.f; m_pDataPointer[20].normal.y = 0.f; m_pDataPointer[20].normal.z = 0.f;
		m_pDataPointer[20].texCoords.x = 0.f; m_pDataPointer[20].texCoords.y = 1.f; 
		
		m_pDataPointer[21].position.x = hS; m_pDataPointer[21].position.y = hS; m_pDataPointer[21].position.z = hS;
		m_pDataPointer[21].normal.x = 1.f; m_pDataPointer[21].normal.y = 0.f; m_pDataPointer[21].normal.z = 0.f;
		m_pDataPointer[21].texCoords.x = 0.f; m_pDataPointer[21].texCoords.y = 1.f; 
		
		m_pDataPointer[22].position.x = hS; m_pDataPointer[22].position.y = hS; m_pDataPointer[22].position.z = -hS;
		m_pDataPointer[22].normal.x = 1.f; m_pDataPointer[22].normal.y = 0.f; m_pDataPointer[22].normal.z = 0.f;
		m_pDataPointer[22].texCoords.x = 1.f; m_pDataPointer[22].texCoords.y = 1.f; 
		
		m_pDataPointer[23].position.x = hS; m_pDataPointer[23].position.y = -hS; m_pDataPointer[23].position.z = -hS;
		m_pDataPointer[23].normal.x = 1.f; m_pDataPointer[23].normal.y = 0.f; m_pDataPointer[23].normal.z = 0.f;
		m_pDataPointer[23].texCoords.x = 1.f; m_pDataPointer[23].texCoords.y = 1.f;
		
		
		//Face bas
		m_pDataPointer[24].position.x = -hS; m_pDataPointer[24].position.y = -hS; m_pDataPointer[24].position.z = -hS;
		m_pDataPointer[24].normal.x = 0.f; m_pDataPointer[24].normal.y = -1.f; m_pDataPointer[24].normal.z = 0.f;
		m_pDataPointer[24].texCoords.x = 0.f; m_pDataPointer[24].texCoords.y = 0.f; 
		
		m_pDataPointer[25].position.x = hS; m_pDataPointer[25].position.y = -hS; m_pDataPointer[25].position.z = -hS;
		m_pDataPointer[25].normal.x = 0.f; m_pDataPointer[25].normal.y = -1.f; m_pDataPointer[25].normal.z = 0.f;
		m_pDataPointer[25].texCoords.x = 1.f; m_pDataPointer[25].texCoords.y = 0.f; 
		
		m_pDataPointer[26].position.x = hS; m_pDataPointer[26].position.y = -hS; m_pDataPointer[26].position.z = hS;
		m_pDataPointer[26].normal.x = 0.f; m_pDataPointer[26].normal.y = -1.f; m_pDataPointer[26].normal.z = 0.f;
		m_pDataPointer[26].texCoords.x = 1.f; m_pDataPointer[26].texCoords.y = 1.f; 
		
		m_pDataPointer[27].position.x = hS; m_pDataPointer[27].position.y = -hS; m_pDataPointer[27].position.z = hS;
		m_pDataPointer[27].normal.x = 0.f; m_pDataPointer[27].normal.y = -1.f; m_pDataPointer[27].normal.z = 0.f;
		m_pDataPointer[27].texCoords.x = 1.f; m_pDataPointer[27].texCoords.y = 1.f; 
		
		m_pDataPointer[28].position.x = -hS; m_pDataPointer[28].position.y = -hS; m_pDataPointer[28].position.z = hS;
		m_pDataPointer[28].normal.x = 0.f; m_pDataPointer[28].normal.y = -1.f; m_pDataPointer[28].normal.z = 0.f;
		m_pDataPointer[28].texCoords.x = 1.f; m_pDataPointer[28].texCoords.y = 0.f; 
		
		m_pDataPointer[29].position.x = -hS; m_pDataPointer[29].position.y = -hS; m_pDataPointer[29].position.z = -hS;
		m_pDataPointer[29].normal.x = 0.f; m_pDataPointer[29].normal.y = -1.f; m_pDataPointer[29].normal.z = 0.f;
		m_pDataPointer[29].texCoords.x = 0.f; m_pDataPointer[29].texCoords.y = 0.f;
		
		
		//Face haute
		m_pDataPointer[30].position.x = -hS; m_pDataPointer[30].position.y = hS; m_pDataPointer[30].position.z = -hS;
		m_pDataPointer[30].normal.x = 0.f; m_pDataPointer[30].normal.y = 1.f; m_pDataPointer[30].normal.z = 0.f;
		m_pDataPointer[30].texCoords.x = 0.f; m_pDataPointer[30].texCoords.y = 1.f; 
		
		m_pDataPointer[31].position.x = hS; m_pDataPointer[31].position.y = hS; m_pDataPointer[31].position.z = -hS;
		m_pDataPointer[31].normal.x = 0.f; m_pDataPointer[31].normal.y = 1.f; m_pDataPointer[31].normal.z = 0.f;
		m_pDataPointer[31].texCoords.x = 1.f; m_pDataPointer[31].texCoords.y = 1.f; 
		
		m_pDataPointer[32].position.x = hS; m_pDataPointer[32].position.y = hS; m_pDataPointer[32].position.z = hS;
		m_pDataPointer[32].normal.x = 0.f; m_pDataPointer[32].normal.y = 1.f; m_pDataPointer[32].normal.z = 0.f;
		m_pDataPointer[32].texCoords.x = 1.f; m_pDataPointer[32].texCoords.y = 0.f; 
		
		m_pDataPointer[33].position.x = hS; m_pDataPointer[33].position.y = hS; m_pDataPointer[33].position.z = hS;
		m_pDataPointer[33].normal.x = 0.f; m_pDataPointer[33].normal.y = 1.f; m_pDataPointer[33].normal.z = 0.f;
		m_pDataPointer[33].texCoords.x = 1.f; m_pDataPointer[33].texCoords.y = 0.f; 
		
		m_pDataPointer[34].position.x = -hS; m_pDataPointer[34].position.y = hS; m_pDataPointer[34].position.z = hS;
		m_pDataPointer[34].normal.x = 0.f; m_pDataPointer[34].normal.y = 1.f; m_pDataPointer[34].normal.z = 0.f;
		m_pDataPointer[34].texCoords.x = 0.f; m_pDataPointer[34].texCoords.y = 0.f; 
		
		m_pDataPointer[35].position.x = -hS; m_pDataPointer[35].position.y = hS; m_pDataPointer[35].position.z = -hS;
		m_pDataPointer[35].normal.x = 0.f; m_pDataPointer[35].normal.y = 1.f; m_pDataPointer[35].normal.z = 0.f;
		m_pDataPointer[35].texCoords.x = 0.f; m_pDataPointer[35].texCoords.y = 1.f;
	}
	
	Cube::~Cube(){ delete[] m_pDataPointer;}
	
	// Renvoit le pointeur vers les données
    const GLvoid* Cube::getDataPointer() const {
        return m_pDataPointer;
    }
    
    // Renvoit le nombre de vertex
    GLsizei Cube::getVertexCount() const {
        return m_nVertexCount;
    }

    // Renvoit la taille du tableau en octets
    GLsizeiptr Cube::getByteSize() const {
        return getVertexCount() * VERTEX_BYTE_SIZE; // Nombre de sommet * taille d'un sommet
    }
    
    // Renvoit le nombre de composantes de l'attribut position
    GLint Cube::getPositionNumComponents() const {
        return POSITION_NUM_COMPONENTS;
    }
    
    // Renvoit le nombre de composantes de l'attribut normale
    GLint Cube::getNormalNumComponents() const {
        return NORMAL_NUM_COMPONENTS;
    }
    
    // Renvoit le nombre de composantes de l'attribut coordonnées de texture
    GLint Cube::getTexCoordsNumComponents() const {
        return TEXCOORDS_NUM_COMPONENTS;
    }
    
    // Renvoit la taille d'un vertex en octets
    GLsizei Cube::getVertexByteSize() const {
        return VERTEX_BYTE_SIZE;
    }
    
    // Renvoit l'offset de l'attribut position
    const GLvoid* Cube::getPositionOffset() const {
        return reinterpret_cast<const GLvoid*>(POSITION_OFFSET);
    }
    
    // Renvoit l'offset de l'attribut normale
    const GLvoid* Cube::getNormalOffset() const {
        return reinterpret_cast<const GLvoid*>(NORMAL_OFFSET);
    }
    
    // Renvoit l'offset de l'attribut coordonnées de texture
    const GLvoid* Cube::getTexCoordsOffset() const {
        return reinterpret_cast<const GLvoid*>(TEXCOORDS_OFFSET);
    }
    
    // Renvoit le type OpenGL d'une composante d'attribut
    GLenum Cube::getDataType() const {
        return GL_FLOAT;
    }
}
