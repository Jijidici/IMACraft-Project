#include "imac2gl3/cameras/TrackBallCamera.hpp"

TrackBallCamera::TrackBallCamera() : m_fDistance(0.f), m_fAngleX(0.f), m_fAngleY(0.f){
}

TrackBallCamera::~TrackBallCamera(){
}

void TrackBallCamera::moveFront(float distance){
	m_fDistance += distance;
}

void TrackBallCamera::rotateLeft(float degrees){
	m_fAngleY += degrees;
}

void TrackBallCamera::rotateUp(float degrees){
	m_fAngleX += degrees;
}

glm::mat4 TrackBallCamera::getViewMatrix() const{
	glm::mat4 V = glm::lookAt(glm::vec3(0,0,-m_fDistance), glm::vec3(0,0,0), glm::vec3(0,1,0));
	V = glm::rotate(V, m_fAngleX, glm::vec3(1.0f,0,0));
	V = glm::rotate(V, m_fAngleY, glm::vec3(0,1.f,0));
	
	return V;
}
