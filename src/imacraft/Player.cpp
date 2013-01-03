#include "imacraft/Player.hpp"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define CUBE_SIZE 0.015625
#define PI 3.14159265

namespace imacraft{

	Player::Player(){
		m_Position = glm::vec3(0.f, 5*CUBE_SIZE, 0.f);
		m_fPhi = 0;
		m_fTheta = 0;
		m_CubePosition = glm::ivec3(0,0,0);
		m_currentNorthPosition = 0;
		m_currentEastPosition = 0;

		computeDirectionVectors();
	}

	void Player::computeDirectionVectors(){
		m_FrontVector = glm::vec3(glm::cos(m_fTheta)*glm::sin(m_fPhi), glm::sin(m_fTheta), glm::cos(m_fTheta)*glm::cos(m_fPhi));
		m_LeftVector = glm::vec3(glm::sin(m_fPhi+(PI/2)), 0.f, glm::cos(m_fPhi+(PI/2)));
		m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
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
		return changeGrid;
	}

	void Player::rotateLeft(float degree){
		m_fPhi = glm::radians(degree);
		computeDirectionVectors();
	}

	void Player::rotateUp(float degree){
		m_fTheta = glm::radians(degree);
		computeDirectionVectors();
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
		float cpt = 0.0;
		while(cpt<=CUBE_SIZE){
			moveFront(0.001);
			m_Position.y += 0.001;
			cpt+=0.001;
		}
	}

	void Player::fall(float end){
		float cpt = 0.0;
		while(cpt<=end){
			moveFront(0.001);
			m_Position.y -= 0.001;
			cpt+= 0.001;
		}
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
