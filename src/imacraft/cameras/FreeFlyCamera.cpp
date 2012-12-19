#include "imac2gl3/cameras/FreeFlyCamera.hpp"

#define PI 3.141592654

void FreeFlyCamera::computeDirectionVectors(){
	m_FrontVector.x = cos(m_fTheta)*sin(m_fPhi);
	m_FrontVector.y = sin(m_fTheta);
	m_FrontVector.z = cos(m_fTheta)*cos(m_fPhi);
	
	m_LeftVector.x = sin(m_fPhi + PI/2.);
	m_LeftVector.y = 0.;
	m_LeftVector.z = cos(m_fPhi + PI/2.);
	
	m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

FreeFlyCamera::FreeFlyCamera() : m_Position(glm::vec3(0.,0.,0.)), m_fPhi(PI), m_fTheta(0.){
	computeDirectionVectors();
}

FreeFlyCamera::~FreeFlyCamera(){
}

void FreeFlyCamera::moveLeft(float t){
	m_Position += t*m_LeftVector;
}

void FreeFlyCamera::moveFront(float t){
	m_Position += t*m_FrontVector;
}

void FreeFlyCamera::rotateLeft(float degrees){
	float radAngle = degrees / PI;
	m_fPhi += radAngle/90.;
	computeDirectionVectors();
}

void FreeFlyCamera::rotateUp(float degrees){
	float radAngle = degrees / PI;
	m_fTheta += radAngle/90.;
	computeDirectionVectors();
}

glm::mat4 FreeFlyCamera::getViewMatrix() const{
	glm::mat4 V = glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
	//V = glm::rotate(V, m_fAngleX, glm::vec3(1.0f,0,0));
	//V = glm::rotate(V, m_fAngleY, glm::vec3(0,1.f,0));
	
	return V;
}
