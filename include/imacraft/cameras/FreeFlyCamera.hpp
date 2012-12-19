#ifndef _FREEFLYCAMERA_HPP_
#define _FREEFLYCAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class FreeFlyCamera{
	private :
		glm::vec3 m_Position; // position de la camera
		float m_fPhi, m_fTheta;
		
		glm::vec3 m_FrontVector;
		glm::vec3 m_LeftVector;
		glm::vec3 m_UpVector;
		
		void computeDirectionVectors();
		
	
	public :
		void moveLeft(float t);
		void moveFront(float t);
		void rotateLeft(float degrees);
		void rotateUp(float degrees);
		FreeFlyCamera();
		~FreeFlyCamera();
		glm::mat4 getViewMatrix() const;
};

#endif //_FREEFLYCAMERA_HPP_
