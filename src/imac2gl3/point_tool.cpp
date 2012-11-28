#include "imac2gl3/point_tool.hpp"
#include <iostream>
#include <cmath>
#include <GL/glew.h>

#define MY_PI 3.14159265

Vec2f::Vec2f(){
	this->x =0;
	this->y =0;
}

Vec2f::Vec2f(GLfloat inX, GLfloat inY){
	this->x = inX;
	this->y = inY;
}

GLfloat Vec2f::getX(){
	return this->x;
}

GLfloat Vec2f::getY(){
	return this->y;
}

//************************************************

Col3f::Col3f(){
	this->r = 0;
	this->g = 0;
	this->b = 0;
}

Col3f::Col3f(GLfloat inR, GLfloat inG, GLfloat inB){
	this->r = inR;
	this->g = inG;
	this->b = inB;
}

GLfloat Col3f::getR(){
	return this->r;
}

GLfloat Col3f::getG(){
	return this->g;
}

GLfloat Col3f::getB(){
	return this->b;
}

//*****************************************************************

Vertex::Vertex(){
	this->point = Vec2f(0.,0.);
	this->color = Col3f(0.,0.,0.);
}

Vertex::Vertex(Vec2f inPoint, Col3f inColor){
	this->point = inPoint;
	this->color = inColor;
}

Vec2f Vertex::getPoint(){
	return this->point;
}

Col3f Vertex::getColor(){
	return this->color;
}

//*************************************************

Square::Square(){
	this->square_vertices = new Vertex[4];

	this->square_vertices[0] = Vertex(Vec2f(-0.5f,-0.5f), Col3f(0.f,0.f,0.f));
	this->square_vertices[1] = Vertex(Vec2f( 0.5f,-0.5f), Col3f(0.f,0.f,0.f));
	this->square_vertices[2] = Vertex(Vec2f( 0.5f, 0.5f), Col3f(0.f,0.f,0.f));
	this->square_vertices[3] = Vertex(Vec2f(-0.5f, 0.5f), Col3f(0.f,0.f,0.f));
}

Square::Square(Vertex vx1, Vertex vx2, Vertex vx3, Vertex vx4){
	this->square_vertices = new Vertex[4];
	
	this->square_vertices[0] = vx1;
	this->square_vertices[1] = vx2;
	this->square_vertices[2] = vx3;
	this->square_vertices[3] = vx4;
}

Vertex Square::getVertex(int index){
	if(index>=0 && index<4){
		return this->square_vertices[index];
	}else{
		if(index<0){
			return this->square_vertices[0];
		}else{
			return this->square_vertices[3];
		}
	}
}


Vertex* Square::getVertices(){
	return this->square_vertices;
}

//****************************************************

Circle::Circle(){
	this->circle_vertices = NULL;
	this->nb_segment = 50;
	this->radius = 0;
}

Circle::Circle(Vertex inOrigin, GLfloat inRadius, int drawing_precision){
	if(drawing_precision < 5){
		drawing_precision = 5;
	}
	this->nb_segment = drawing_precision;
	this->circle_vertices = new Vertex[this->nb_segment];
	this->radius = inRadius;
	
	this->circle_vertices[0] = inOrigin;
	
	for(int i=1;i<this->nb_segment;++i){
		this->circle_vertices[i] = Vertex(Vec2f(cos( (i-1) * (2*MY_PI)/(this->nb_segment-2) ) * this->radius + inOrigin.getPoint().getX(), sin( (i-1)* (2*MY_PI)/(this->nb_segment-2) ) * this->radius + inOrigin.getPoint().getY()), inOrigin.getColor());
		std::cout<<i<<"]|| cos : "<<cos( (i-1) * (2*MY_PI)/(this->nb_segment-2) ) * this->radius + inOrigin.getPoint().getX()<<"\t sin : "<<sin( (i-1)* (2*MY_PI)/(this->nb_segment-2) ) * this->radius + inOrigin.getPoint().getY()<<std::endl;
	}
}

int Circle::getNbVertex(){
	return this->nb_segment;
}


Vertex Circle::getAVertex(int index){
	return this->circle_vertices[index];
}

Vertex* Circle::getVertices(){
	return this->circle_vertices;
}

Circle::~Circle(){
	//delete this->circle_vertices;
}
