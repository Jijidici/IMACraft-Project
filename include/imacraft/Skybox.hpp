#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "imacraft/Player.hpp"
#include "imacraft/shapes/CubeInstance.hpp"

namespace imacraft{
	class Skybox{
		private:
			CubeInstance* m_pModel;
			Player* m_pCamera;
		
		public:
			Skybox(Player* player, CubeInstance* pModel);
			~Skybox();
			void draw();
	};
}

#endif
