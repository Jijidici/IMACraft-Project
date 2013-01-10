#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imacraft/Player.hpp"

#define PI 3.14159265

namespace imacraft{

	Player::Player() : m_nearDistance(0.00001), m_farDistance(1.0), m_verticalFieldOfView(90.0){
		m_Position = glm::vec3(1+CUBE_SIZE/2, 5*CUBE_SIZE, 1+CUBE_SIZE/2);
		m_fPhi = PI;
		m_fTheta = 0;
		m_CubePosition = glm::ivec3(0,0,0);
		m_seenPosInCube = glm::vec3(-10, -10, -10);
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
		
		float nearHalfHeight = tan(m_verticalFieldOfView/2.) * m_nearDistance;
		
		m_frustumTopPlanePoint = m_Position;
		m_frustumTopPlaneNormal = glm::normalize(glm::cross(m_LeftVector, (m_frustumNearPlanePoint + m_UpVector*nearHalfHeight) - m_Position));
		
		m_frustumRightPlanePoint = m_Position;
		m_frustumRightPlaneNormal = glm::normalize(glm::cross(m_UpVector, (m_frustumNearPlanePoint - m_LeftVector*nearHalfHeight) - m_Position));
		
		m_frustumBottomPlanePoint = m_Position;
		m_frustumBottomPlaneNormal = glm::normalize(glm::cross((m_frustumNearPlanePoint - m_UpVector*nearHalfHeight) - m_Position, m_LeftVector));
		
		m_frustumLeftPlanePoint = m_Position;
		m_frustumLeftPlaneNormal = glm::normalize(glm::cross((m_frustumNearPlanePoint + m_LeftVector*nearHalfHeight) - m_Position, m_UpVector));
		
	}
	
	void Player::changeFrustumFar(float amount){
		if(m_farDistance + amount > 0.1){
			m_farDistance += amount;
		}
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
	
	int Player::moveLeft(float const t){
		int changeGrid = 0;
		
		if(m_Position.x > 1){
			m_Position.x = -1+CUBE_SIZE;
			--m_currentEastPosition;
			std::cout << std::endl << "went east" << std::endl;
			changeGrid = 3;
		}
		if(m_Position.x <= -1){
			m_Position.x = 1;
			++m_currentEastPosition;
			std::cout << std::endl << "went west" << std::endl;
			changeGrid = 4;
		}
		if(m_Position.z >= 1){
			m_Position.z = -1+CUBE_SIZE;
			++m_currentNorthPosition;
			std::cout << std::endl << "went north" << std::endl;
			changeGrid = 1;
		}
		if(m_Position.z <= -1){
			m_Position.z = 1;
			--m_currentNorthPosition;
			std::cout << std::endl << "went south" << std::endl;
			changeGrid = 2;
		}
		m_Position += t * m_LeftVector;
		computeFrustumPlanes();
		
		return changeGrid;
	}

	int Player::moveFront(float const t){
		int changeGrid = 0;
		
		if(m_Position.x >= 1){
			m_Position.x = -1+CUBE_SIZE;
			--m_currentEastPosition;
			std::cout << std::endl << "went east" << std::endl;
			changeGrid = 3;
		}
		if(m_Position.x <= -1){

			m_Position.x = 1;
			++m_currentEastPosition;
			std::cout << std::endl << "went west" << std::endl;
			changeGrid = 4;
		}
		if(m_Position.z >= 1){
			m_Position.z = -1+CUBE_SIZE;
			++m_currentNorthPosition;
			std::cout << std::endl << "went north" << std::endl;
			changeGrid = 1;
		}
		if(m_Position.z <= -1){

			m_Position.z = 1;
			--m_currentNorthPosition;
			std::cout << std::endl << "went south" << std::endl;
			changeGrid = 2;
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
	
	const glm::vec3 Player::getFrontVector() const{
		return m_FrontVector;
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

	void Player::blockY(float posY){
		m_Position.y = posY;
	}

	void Player::fall(float gravity){
			m_Position.y += gravity;
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
	
	const glm::vec3 Player::getSeenPosInCube() const{
		return m_seenPosInCube;
	}
	
	//view target
	const int Player::whatCubeTargeted(std::vector<imacraft::TerrainGrid*>& vecGrids){
		float viewLimit = VIEW_LIMIT*CUBE_SIZE;
		glm::vec3 currentPos = m_Position;
		float step = CUBE_SIZE/16.;
		float distanceRoamed = 0;
		int idxGrid = CENTER;
		
		while(distanceRoamed < viewLimit){
			currentPos = currentPos + step * m_FrontVector;
			distanceRoamed += step;
			
			//manage the grid changing
			if(currentPos.x > 1){
				currentPos.x = -1;
				idxGrid = WEST;
				
			}else if(currentPos.x < -1){
				currentPos.x = 1;
				idxGrid = EAST;
				
			}else if(currentPos.z > 1){
				currentPos.z = -1;
				idxGrid = NORTH;
				
			}else if(currentPos.z < -1){
				currentPos.z = 1;
				idxGrid = SOUTH;
			}
			
			glm::ivec3 currentCube = TerrainGrid::getCubeIntegerPosition(currentPos);
			int inGridPos = currentCube.z*TerrainGrid::TERRAIN_HEIGHT*TerrainGrid::TERRAIN_WIDTH + currentCube.y*TerrainGrid::TERRAIN_WIDTH + currentCube.x;
			
			if((*vecGrids[idxGrid])[inGridPos]!= 0){
				//we see a fill cube
				m_seenPosInCube = currentPos;			
				return idxGrid;
			}
		}
		//this position is too far from the camera
		return -1;
	}
}
