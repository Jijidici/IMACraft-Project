#ifndef __LIGHT_MANAGER_HPP
#define __LIGHT_MANAGER_HPP

#include <vector>
#include <glm/glm.hpp>
#include "imacraft/lighting/Lights.hpp"
#include "imacraft/TerrainGrid.hpp"


namespace imacraft{
	class LightManager{
		static const uint8_t MAX_POINT_LIGHT_COUNT = 16; 
	
		public:
			LightManager();
			~LightManager();
			void addLight(const DirectionalLight& light);
			void addLight(const PointLight& light);
			void removeLight(int idx);
			void sendLights(GLuint program, const glm::mat4& viewMatrix);
			void updatePtLightsPosition(int exitSide);
			
		private:
			DirectionalLight m_dirLight;
			PointLight m_ptLights[MAX_POINT_LIGHT_COUNT];
			int m_currentPtLightCount;
	};
}

#endif
