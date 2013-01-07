#include "imacraft/Skybox.hpp"

#include <iostream>
#include <string>
#include <GL/glew.h>
#include "imacraft/Player.hpp"
#include "imacraft/Texture.hpp"
#include "imacraft/shapes/CubeInstance.hpp"

namespace imacraft{
	
	Skybox::Skybox(const std::string& imgDirectory, Player* player, CubeInstance& model): m_model(model), m_pCamera(player){
		m_textureObj=0;
		
		/* get the images path */
		m_filenames[0] = imgDirectory + "/posX.jpg";
		m_filenames[1] = imgDirectory + "/negX.jpg";
		m_filenames[2] = imgDirectory + "/posY.jpg";
		m_filenames[3] = imgDirectory + "/negY.jpg";
		m_filenames[4] = imgDirectory + "/posZ.jpg";
		m_filenames[5] = imgDirectory + "/negZ.jpg";
		
		m_program = 0;
	}
	
	Skybox::~Skybox(){
		glDeleteTextures(1, &m_textureObj);
	}
	
	void Skybox::load(){
		GLenum types[6] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		};
		
		glGenTextures(1, &m_textureObj);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);
			SDL_Surface* pImg = NULL;
			for(uint8_t idx=0;idx<6;++idx){
				pImg = IMG_Load(m_filenames[idx].c_str());
				if(!pImg){
					std::cout << "Impossible de charger l'image : " << m_filenames[idx] << std::endl;
					exit(EXIT_FAILURE);
				}
				glTexImage2D(types[idx], 0, GL_RGBA, pImg->w, pImg->h, 0, Texture::getFormat(pImg), GL_UNSIGNED_BYTE, pImg->pixels);
				SDL_FreeSurface(pImg);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}
