#include "imacraft/lighting/LightManager.hpp"

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <GL/glew.h>
#include "imacraft/lighting/Lights.hpp"

namespace imacraft{
	LightManager::LightManager() : m_currentPtLightCount(0){
	}
	
	LightManager::~LightManager(){
	}
	
	void LightManager::addLight(const DirectionalLight& light){
		m_dirLight = light;
	}
	
	void LightManager::addLight(const PointLight& light){
		if(m_currentPtLightCount >= MAX_POINT_LIGHT_COUNT){
			removeLight(0);
		}
		
		m_ptLights[m_currentPtLightCount] = light;
		m_currentPtLightCount++;
	}
	
	void LightManager::removeLight(int idx){
		if(m_currentPtLightCount != 0){
			for(int i=idx;i<m_currentPtLightCount-1;++i){
				m_ptLights[i] = m_ptLights[i+1];
			}
			m_currentPtLightCount--;
		}
	}
	
	void LightManager::sendLights(GLuint program, const glm::mat4& viewMatrix){
		DirectionalLightUniform dlUniform;
		dlUniform.getLocations("uDirLight", program);
		DirectionalLight viewDirLight(viewMatrix * m_dirLight.dir, m_dirLight.i);
		sendDirectionalLight(viewDirLight, dlUniform);
		
		//send the count of point lights
		GLuint CountPLLocation = glGetUniformLocation(program, "uPointLightCount");
		glUniform1i(CountPLLocation, m_currentPtLightCount);
		
		PointLightUniform plUniform;
		PointLight viewPointLight;
		for(int i=1;i<=m_currentPtLightCount;++i){
			std::stringstream uniformName;
			uniformName<<"uPointLights["<<i<<"]";
			plUniform.getLocations(uniformName.str().c_str(), program);
			viewPointLight.lPos = viewMatrix * m_ptLights[i-1].lPos;
			viewPointLight.i = m_ptLights[i-1].i;
			sendPointLight(viewPointLight, plUniform);
		}
	}
	
	void LightManager::updatePtLightsPosition(int exitSide){
		switch(exitSide){
			case EAST:
				for(int idx=0;idx<m_currentPtLightCount;++idx){
					m_ptLights[idx].lPos.x -= 2.; 
				}
				break;
			
			case WEST:
				for(int idx=0;idx<m_currentPtLightCount;++idx){
					m_ptLights[idx].lPos.x += 2.; 
				}
				break;
				
			case NORTH:
				for(int idx=0;idx<m_currentPtLightCount;++idx){
					m_ptLights[idx].lPos.z -= 2.; 
				}
				break;
				
			case SOUTH:
				for(int idx=0;idx<m_currentPtLightCount;++idx){
					m_ptLights[idx].lPos.z += 2.; 
				}
				break;
			
			default:
				break;
		}
	}
	
	const int LightManager::getNbPointLight() const{
		return m_currentPtLightCount;
	}
	
	const glm::vec3 LightManager::getPointLightPos(const int idx) const{
		return glm::vec3(m_ptLights[idx].lPos.x, m_ptLights[idx].lPos.y, m_ptLights[idx].lPos.z);
	}
}
