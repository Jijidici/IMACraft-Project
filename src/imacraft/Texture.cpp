#include <iostream>
#include <cstdlib>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>

#include "imacraft/Texture.hpp"

namespace imacraft{

GLint Texture::textureIndex = 0;

Texture::Texture(const char* path, GLuint& program) : m_path(path), m_program(program), m_texture_gluint(textureIndex){
	//~ std::cout << m_path << std::endl;
	m_textureIndex = textureIndex;
	//~ std::cout << m_textureIndex << std::endl;
	sendTextureToGPU(m_texture_gluint);
	getUniformSamplerLocation();
	++textureIndex;
}
	
SDL_Surface* Texture::loadImage(){
	SDL_Surface* image = IMG_Load(m_path);
	if(!image){
		std::cout << "Impossible de charger l'image : " << m_path << std::endl;
		exit(EXIT_FAILURE);
	}
	
	return image;
}
Texture::~Texture(){
	glDeleteTextures(1, &m_texture_gluint);
}

void Texture::print_test(){
	//~ std::cout << textureIndex << std::endl;
}

void Texture::sendTextureToGPU(GLuint &texture_gluint){
	SDL_Surface* image = loadImage();
	
	//~ std::cout << "test" << texture_gluint << std::endl;
	
	glGenTextures(1, &texture_gluint);
	glBindTexture(GL_TEXTURE_2D, texture_gluint);
	
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			image->w,
			image->h,
			0,
			getFormat(image),
			GL_UNSIGNED_BYTE,
			image->pixels
		);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image);
}

void Texture::getUniformSamplerLocation(){
	//~ std::cout << m_textureIndex << std::endl;
	//~ if(m_textureIndex == 0){
		//~ glUniform1i(glGetUniformLocation(m_program, "uTextureSampler0"), m_textureIndex); // !!! 0 à changer pour textureIndex ??
	//~ }
	//~ if(m_textureIndex == 1){
		//~ glUniform1i(glGetUniformLocation(m_program, "uTextureSampler1"), m_textureIndex); // !!! 0 à changer pour textureIndex ??
	//~ }
	glUniform1i(glGetUniformLocation(m_program, "uTextureSampler0"), 0); // !!! 0 à changer pour textureIndex ??
	
}

void Texture::bindTexture(){
	//~ if(m_textureIndex == 0){
		//~ glActiveTexture(GL_TEXTURE0);
	//~ }
	//~ if(m_textureIndex == 1){
		//~ glActiveTexture(GL_TEXTURE1);
	//~ }
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_gluint);
}

void Texture::debindTexture(){
	//~ if(m_textureIndex == 0){
		//~ glActiveTexture(GL_TEXTURE0);
	//~ }
	//~ if(m_textureIndex == 1){
		//~ glActiveTexture(GL_TEXTURE1);
	//~ }
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getFormat(SDL_Surface* surface){
	GLuint format;
    if(surface->format->BytesPerPixel == 1){
		format = GL_RED;
	}
    if(surface->format->BytesPerPixel == 3){
		format = GL_RGB;
		//~ std::cout << format << std::endl;
	}
	if(surface->format->BytesPerPixel == 4){
		format = GL_RGBA;
	}
	
	return format;
}

}
