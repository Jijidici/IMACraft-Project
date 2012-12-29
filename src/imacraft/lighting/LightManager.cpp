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
		std::cout<<"//-> The current directionnal light will be replace"<<std::endl;
		m_dirLight = light;
	}
	
	void LightManager::addLight(const PointLight& light){
		if(m_currentPtLightCount >= MAX_POINT_LIGHT_COUNT){
			throw std::out_of_range("You can't add another point light.");
		}else{
			m_ptLights[m_currentPtLightCount] = light;
			m_currentPtLightCount++;
			std::cout<<"//-> nb point lights : "<<m_currentPtLightCount<<std::endl;
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
}
