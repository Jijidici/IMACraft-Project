#ifndef __POINT_TOOL__
#define __POINT_TOOL__

#include <GL/glew.h>

/************************/
class Vec2f{
	
	private:
		GLfloat x;
		GLfloat y;
	
	public:
		Vec2f();
		Vec2f(GLfloat inX, GLfloat inY);
	
		GLfloat getX();
		GLfloat getY();
};

/***************************/
class Col3f{
	
	private:
		GLfloat r;
		GLfloat g;
		GLfloat b;
		
	public:
		Col3f();
		Col3f(GLfloat inR, GLfloat inG, GLfloat inB);
		
		GLfloat getR();
		GLfloat getG();
		GLfloat getB();
};

/***************************/
class Vertex{
	private:
		Vec2f point;
		Col3f color;
		
	public:
		Vertex();
		Vertex(Vec2f inPoint, Col3f inColor);
		
		Vec2f getPoint();
		Col3f getColor();
};

/******************************/
class Square{
	private:
		Vertex* square_vertices;
		
	public:
		Square();
		Square(Vertex vx1, Vertex vx2, Vertex vx3, Vertex vx4);
		
		Vertex getVertex(int index);
		Vertex* getVertices();
};

/**************************************/
class Circle{
	private:
		Vertex* circle_vertices;
		int nb_segment;
		GLfloat radius;
		
	public:
		Circle();
		Circle(Vertex inOrigin, GLfloat inRadius, int drawing_precision);
		
		int getNbVertex();
		Vertex getAVertex(int index);
		Vertex* getVertices();
		
		~Circle();	
};

#endif
