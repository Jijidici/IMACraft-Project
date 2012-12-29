#include "imacraft/lighting/LightManager.hpp"

namespace imacraft{
	LightManager::LightManager() : m_currentPtLightCount(0){
	}
	
	LightManager::~LightManager(){
		if(NULL != m_pPtLights){
			delete[] m_pPtLights;
		}
	}
}

/*LightManager();
~LightManager();
void addLight(const DirectionalLight& light);
void addLight(const PointLight& light);
void sendLights(GLuint program, const glm::mat4& viewMatrix);*/
