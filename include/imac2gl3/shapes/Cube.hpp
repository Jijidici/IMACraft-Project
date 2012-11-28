#ifndef _IMAC2GL3_CUBE_
#define _IMAC2GL3_CUBE_

#include <GL/glew.h>
#include "common.hpp"

namespace imac2gl3{

//Represent un cube compose de 12 triangles centre en (0,0,0) dans son repere local
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
class Cube{
	static const GLint POSITION_NUM_COMPONENTS = 3;
	static const GLint NORMAL_NUM_COMPONENTS = 3;
	static const GLint TEXCOORDS_NUM_COMPONENTS = 2;
	
	static const GLsizei POSITION_OFFSET = 0;
	static const GLsizei NORMAL_OFFSET = POSITION_NUM_COMPONENTS * sizeof(GLfloat);
	static const GLsizei TEXCOORDS_OFFSET = (POSITION_NUM_COMPONENTS + NORMAL_NUM_COMPONENTS) * sizeof(GLfloat);
	
	static const GLsizei VERTEX_BYTE_SIZE = (POSITION_NUM_COMPONENTS + NORMAL_NUM_COMPONENTS + TEXCOORDS_NUM_COMPONENTS) * sizeof(GLfloat);
	
	public:
		Cube(GLfloat size);
		~Cube();
		const GLvoid* getDataPointer() const; //Renvoie le tableau de données
		GLsizei getVertexCount() const; //Renvoie le nombre de vertices
		GLsizeiptr getByteSize() const; //Renvoie la taille du tableau en octets
		GLint getPositionNumComponents() const; //Renvoie le nombre de composant pour la position
		GLint getNormalNumComponents() const; //Renvoie le nombre de composant pour la normale
		GLint getTexCoordsNumComponents() const; //Renvoie le nombre de composant pour les coordonnees de texture
		GLsizei getVertexByteSize() const; //Renvoie la taille en octet d'un vertex
		const GLvoid* getPositionOffset() const; //Renvoie l'offset de l'attribut position
		const GLvoid* getNormalOffset() const; //Renvoie l'offset de l'attribut normale
		const GLvoid* getTexCoordsOffset() const; //Renvoie l'offset de l'attribut coordonnees de texture
		GLenum getDataType() const; //Renvoie le type de donnees d'une composante d'attribut
		
	private:
		ShapeVertex * m_pDataPointer; //Pointeur vers les données du tableau
		GLsizei m_nVertexCount;
};

}
#endif
