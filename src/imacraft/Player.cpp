#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imacraft/Player.hpp"

#define CUBE_SIZE 0.015625
#define PI 3.14159265

//~ static const size_t WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

namespace imacraft{

	Player::Player() : m_nearDistance(0.01), m_farDistance(1.), m_verticalFieldOfView(90.0){
		m_Position = glm::vec3(0.5*CUBE_SIZE, 5*CUBE_SIZE, 0.5*CUBE_SIZE);
		m_fPhi = PI;
		m_fTheta = 0;
		m_CubePosition = glm::ivec3(0,0,0);
		m_currentNorthPosition = 0;
		m_currentEastPosition = 0;

		//frustum culling points
		m_frustumNearPlanePoint = glm::vec3(0.f, 0.f, 0.f);
		m_frustumFarPlanePoint = glm::vec3(0.f, 0.f, 0.f);
		m_frustumTopPlanePoint = glm::vec3(0.f, 0.f, 0.f);
		m_frustumRightPlanePoint = glm::vec3(0.f, 0.f, 0.f);
		m_frustumBottomPlanePoint = glm::vec3(0.f, 0.f, 0.f);
		m_frustumLeftPlanePoint = glm::vec3(0.f, 0.f, 0.f);
		//frustum culling planes
		m_frustumNearPlaneNormal = glm::vec3(0.f, 0.f, 0.f);
		m_frustumFarPlaneNormal = glm::vec3(0.f, 0.f, 0.f);
		m_frustumTopPlaneNormal = glm::vec3(0.f, 0.f, 0.f);
		m_frustumRightPlaneNormal = glm::vec3(0.f, 0.f, 0.f);
		m_frustumBottomPlaneNormal = glm::vec3(0.f, 0.f, 0.f);
		m_frustumLeftPlaneNormal = glm::vec3(0.f, 0.f, 0.f);

		computeDirectionVectors();
		computeFrustumPlanes();
	}

	void Player::computeDirectionVectors(){
		m_FrontVector = glm::vec3(glm::cos(m_fTheta)*glm::sin(m_fPhi), glm::sin(m_fTheta), glm::cos(m_fTheta)*glm::cos(m_fPhi));
		m_LeftVector = glm::vec3(glm::sin(m_fPhi+(PI/2.)), 0.f, glm::cos(m_fPhi+(PI/2.)));
		m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
	}
	
	//frustum culling
	void Player::computeFrustumPlanes(){
		m_frustumNearPlanePoint = m_Position + (m_FrontVector*m_nearDistance);
		m_frustumNearPlaneNormal = glm::normalize(m_frustumNearPlanePoint - m_Position);
		
		m_frustumFarPlanePoint = m_Position + (m_FrontVector*m_farDistance);
		m_frustumFarPlaneNormal = glm::normalize(m_Position - m_frustumFarPlanePoint);
		
		//~ float ratio = WINDOW_WIDTH / WINDOW_HEIGHT; // ratio between height and width
		
		float nearHalfHeight = tan(m_verticalFieldOfView/2.) * m_nearDistance;
		//~ float nearHalfWidth = nearHalfHeight * ratio;
		
		//~ float farHalfHeight = tan(m_verticalFieldOfView/2.) * m_farDistance;
		//~ float farHalfWidth = farHalfHeight * ratio;
		
		m_frustumTopPlanePoint = m_Position;
		m_frustumTopPlaneNormal = glm::normalize(glm::cross(m_LeftVector, (m_frustumNearPlanePoint + m_UpVector*nearHalfHeight) - m_Position));
		
		m_frustumRightPlanePoint = m_Position;
		m_frustumRightPlaneNormal = glm::normalize(glm::cross(m_UpVector, (m_frustumNearPlanePoint - m_LeftVector*nearHalfHeight) - m_Position));
		
		m_frustumBottomPlanePoint = m_Position;
		m_frustumBottomPlaneNormal = glm::normalize(glm::cross((m_frustumNearPlanePoint - m_UpVector*nearHalfHeight) - m_Position, m_LeftVector));
		
		m_frustumLeftPlanePoint = m_Position;
		m_frustumLeftPlaneNormal = glm::normalize(glm::cross((m_frustumNearPlanePoint + m_LeftVector*nearHalfHeight) - m_Position, m_UpVector));
		
	}
	
	bool Player::frustumTest(uint16_t i, uint16_t j, uint16_t k, uint16_t gridWidth, int northPos, int eastPos){
		uint16_t halfGridWidth = gridWidth*0.5;
		
		if(glm::dot(m_frustumLeftPlaneNormal, (glm::vec3((i - halfGridWidth - gridWidth*eastPos)*CUBE_SIZE, (j - halfGridWidth)*CUBE_SIZE, (k - halfGridWidth + gridWidth*northPos)*CUBE_SIZE) - m_frustumLeftPlanePoint)) < 0.){
			return false;
		}
		if(glm::dot(m_frustumRightPlaneNormal, (glm::vec3((i - halfGridWidth - gridWidth*eastPos)*CUBE_SIZE, (j - halfGridWidth)*CUBE_SIZE, (k - halfGridWidth + gridWidth*northPos)*CUBE_SIZE) - m_frustumRightPlanePoint)) < 0.){
			return false;
		}
		if(glm::dot(m_frustumNearPlaneNormal, (glm::vec3((i - halfGridWidth - gridWidth*eastPos)*CUBE_SIZE, (j - halfGridWidth)*CUBE_SIZE, (k - halfGridWidth + gridWidth*northPos)*CUBE_SIZE) - m_frustumNearPlanePoint)) < 0.){
			return false;
		}
		if(glm::dot(m_frustumFarPlaneNormal, (glm::vec3((i - halfGridWidth - gridWidth*eastPos)*CUBE_SIZE, (j - halfGridWidth)*CUBE_SIZE, (k - halfGridWidth + gridWidth*northPos)*CUBE_SIZE) - m_frustumFarPlanePoint)) < 0.){
			return false;
		}
		if(glm::dot(m_frustumTopPlaneNormal, (glm::vec3((i - halfGridWidth - gridWidth*eastPos)*CUBE_SIZE, (j - halfGridWidth)*CUBE_SIZE, (k - halfGridWidth + gridWidth*northPos)*CUBE_SIZE) - m_frustumTopPlanePoint)) < 0.){
			return false;
		}
		if(glm::dot(m_frustumBottomPlaneNormal, (glm::vec3((i - halfGridWidth - gridWidth*eastPos)*CUBE_SIZE, (j - halfGridWidth)*CUBE_SIZE, (k - halfGridWidth + gridWidth*northPos)*CUBE_SIZE) - m_frustumBottomPlanePoint)) < 0.){
			return false;
		}
		
		return true;
	}
	
	bool Player::moveLeft(float const t){
		bool changeGrid = false;
		
		if(m_Position.x > 1){
			std::cout << "testX1" << std::endl;
			m_Position.x = -1+CUBE_SIZE;
			--m_currentEastPosition;
			std::cout << "east++" << std::endl;
			changeGrid = true;
		}
		if(m_Position.x <= -1){
			std::cout << "testX2" << std::endl;
			m_Position.x = 1;
			++m_currentEastPosition;
			std::cout << "east--" << std::endl;
			changeGrid = true;
		}
		if(m_Position.z >= 1){
			std::cout << "testZ1" << std::endl;
			m_Position.z = -1+CUBE_SIZE;
			++m_currentNorthPosition;
			std::cout << "north++" << std::endl;
			changeGrid = true;
		}
		if(m_Position.z <= -1){
			std::cout << "testZ2" << std::endl;
			m_Position.z = 1;
			--m_currentNorthPosition;
			std::cout << "north--" << std::endl;
			changeGrid = true;
		}
		m_Position += t * m_LeftVector;
		computeFrustumPlanes();
		
		return changeGrid;
	}

	bool Player::moveFront(float const t){
		bool changeGrid = false;
		
		if(m_Position.x >= 1){
			std::cout << "testX1" << std::endl;
			m_Position.x = -1+CUBE_SIZE;
			--m_currentEastPosition;
			std::cout << "east++" << std::endl;
			changeGrid = true;
		}
		if(m_Position.x <= -1){
			std::cout << "testX2" << std::endl;
			m_Position.x = 1;
			++m_currentEastPosition;
			std::cout << "east--" << std::endl;
			changeGrid = true;
		}
		if(m_Position.z >= 1){
			std::cout << "testZ1" << std::endl;
			m_Position.z = -1+CUBE_SIZE;
			++m_currentNorthPosition;
			std::cout << "north++" << std::endl;
			changeGrid = true;
		}
		if(m_Position.z <= -1){
			std::cout << "testZ2" << std::endl;
			m_Position.z = 1;
			--m_currentNorthPosition;
			std::cout << "north--" << std::endl;
			changeGrid = true;
		}
		m_Position.x += t * m_FrontVector.x;
		m_Position.z += t * m_FrontVector.z;
		computeFrustumPlanes();
		
		return changeGrid;
	}

	void Player::rotateLeft(float degree){
		m_fPhi += glm::radians(degree);
		while(m_fPhi > PI){
			m_fPhi -= 2*PI;
		}
		while(m_fPhi < -PI){
			m_fPhi += 2*PI;
		}
		computeDirectionVectors();
		computeFrustumPlanes();
	}

	void Player::rotateUp(float degree){
		m_fTheta = glm::radians(degree);
		computeDirectionVectors();
		computeFrustumPlanes();
	}

	glm::mat4 Player::getViewMatrix() const{
		return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
	}

	glm::vec3 Player::getPosition() const{
		return m_Position;
	}

	void Player::computeCubePosition(uint16_t terrainWidth, uint16_t terrainHeight){
		int i = (m_Position.x+1)*terrainWidth/2;
		int j = (m_Position.y+1)*terrainHeight/2-1;
		int k = (m_Position.z+1)*terrainWidth/2;
		m_CubePosition = glm::ivec3(i,j,k);
	}

	glm::ivec3 Player::getCubePosition() const{
		return m_CubePosition;
	}

	float Player::getViewAngle() const{
		return m_fPhi;
	}

	void Player::blockX(float posX){
		m_Position.x = posX;
	}	

	void Player::blockZ(float posZ){
		m_Position.z = posZ;
	}

	void Player::jump(){
			m_Position.y += 0.03;
	}

	void Player::fall(float gravity){
			m_Position.y -= gravity;
	}
	
	void Player::setCurrentNEPosition(int &north, int &east){
		m_currentNorthPosition = north;
		m_currentEastPosition = east;
	}
	
	void Player::printCurrentNEPosition(){
		std::cout << "North position : " << m_currentNorthPosition << std::endl;
		std::cout << "East position : " << m_currentEastPosition << std::endl;
	}
	
	int Player::getCurrentNorthPosition(){
		return m_currentNorthPosition;
	}
	int Player::getCurrentEastPosition(){
		return m_currentEastPosition;
	}
}
