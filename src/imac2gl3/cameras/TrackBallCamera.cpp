#include "imac2gl3/cameras/TrackBallCamera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imac2gl3/MatrixStack.hpp"

namespace imac2gl3{

// --> DEFAULT CONSTRUCTOR
TrackBallCamera::TrackBallCamera() : m_fDistance(5), m_fAngleX(0), m_fAngleY(0){
}

void TrackBallCamera::moveFront(float distance){
	m_fDistance += distance;
	if(m_fDistance < 1.5){
		m_fDistance = 1.5;
	}
}

void TrackBallCamera::rotateLeft(float degrees){
	m_fAngleY = degrees;
}

void TrackBallCamera::rotateUp(float degrees){
	m_fAngleX = degrees;
}

glm::mat4 TrackBallCamera::getViewMatrix() const{
	MatrixStack V;
	V.set(glm::lookAt(glm::vec3(0.f,0.f,m_fDistance), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f,1.f,0.f)));
	V.rotate(m_fAngleX, glm::vec3(1.f, 0.f, 0.f));
	V.rotate(m_fAngleY, glm::vec3(0.f, 1.f, 0.f));
	return V.top();
}

}