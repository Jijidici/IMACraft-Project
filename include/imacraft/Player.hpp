#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <glm/glm.hpp>
#include <stdint.h>

namespace imacraft{

class Player{
	private:
		glm::vec3 m_Position;
		float m_fPhi;
		float m_fTheta;
		glm::ivec3 m_CubePosition;
		//~ glm::vec3 m_FrontVector;
		glm::vec3 m_LeftVector;
		glm::vec3 m_UpVector;

		int m_currentNorthPosition;
		int m_currentEastPosition;
		
		float m_nearDistance, m_farDistance, m_verticalFieldOfView;

		void computeDirectionVectors();
		void computeFrustumPlanes();
		
	public:
		Player();
		bool moveLeft(float const t);
		bool moveFront(float const t);
		void rotateLeft(float degree);
		void rotateUp(float degree);
		glm::mat4 getViewMatrix() const;
		glm::vec3 getPosition() const;
		void computeCubePosition(uint16_t, uint16_t);
		glm::ivec3 getCubePosition() const;
		float getViewAngle() const;
		void blockX(float);
		void blockZ(float);
		void jump();
		void fall(float end);
		
		bool frustumTest(uint16_t i, uint16_t j, uint16_t k);
		
		glm::vec3 m_FrontVector;
		
		glm::vec3 m_frustumNearPlanePoint;
		glm::vec3 m_frustumFarPlanePoint;
		glm::vec3 m_frustumTopPlanePoint;
		glm::vec3 m_frustumRightPlanePoint;
		glm::vec3 m_frustumBottomPlanePoint;
		glm::vec3 m_frustumLeftPlanePoint;
		
		glm::vec3 m_frustumNearPlaneNormal;
		glm::vec3 m_frustumFarPlaneNormal;
		glm::vec3 m_frustumTopPlaneNormal;
		glm::vec3 m_frustumRightPlaneNormal;
		glm::vec3 m_frustumBottomPlaneNormal;
		glm::vec3 m_frustumLeftPlaneNormal;
		
		void setCurrentNEPosition(int &north, int &east);
		void printCurrentNEPosition();
		int getCurrentNorthPosition();
		int getCurrentEastPosition();
		
};

}

#endif
