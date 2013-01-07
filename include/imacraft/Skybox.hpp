#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

#include <string>
#include <GL/glew.h>
#include "imacraft/Player.hpp"
#include "imacraft/shapes/CubeInstance.hpp"

namespace imacraft{
	class Skybox{
		private:
			GLuint m_textureObj;
			std::string m_filenames[6];
			GLuint m_program;
			CubeInstance m_model;
			Player* m_pCamera;
		
		public:
			Skybox(const std::string& imgDirectory, Player* player, CubeInstance& model);
			~Skybox();
			void load();
	};
}

#endif
