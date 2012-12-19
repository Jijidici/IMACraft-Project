#ifndef _TRACKBALLCAMERA_HPP_
#define _TRACKBALLCAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TrackBallCamera{
	private :
		float m_fDistance; // distance par rapport à la scène
		float m_fAngleX; // rotation autour de l'axe X (haut-bas)
		float m_fAngleY; // rotation autour de l'axe Y (gauche-droite)
	
	public :
		TrackBallCamera();
		~TrackBallCamera();
		void moveFront(float distance);
		void rotateLeft(float degrees);
		void rotateUp(float degrees);
		glm::mat4 getViewMatrix() const;
};

#endif //_TRACKBALLCAMERA_HPP_
