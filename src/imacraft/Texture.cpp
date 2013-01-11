#include <iostream>
#include <cstdlib>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>

#include "imacraft/Texture.hpp"

namespace imacraft{

Texture::Texture(const char* path, GLuint& program) : m_path(path), m_program(program), m_texture_gluint(0){
	sendTextureToGPU();
	getUniformSamplerLocation();
}
	
SDL_Surface* Texture::loadImage(){
	SDL_Surface* image = IMG_Load(m_path);
	if(!image){
		std::cout << "Unable to load the image : " << m_path << std::endl;
		exit(EXIT_FAILURE);
	}
	
	return image;
}
Texture::~Texture(){
	glDeleteTextures(1, &m_texture_gluint);
}

void Texture::print_test(){
	std::cout << m_path << std::endl;
}

void Texture::sendTextureToGPU(){
	SDL_Surface* image = loadImage();
	
	glGenTextures(1, &m_texture_gluint);
	glBindTexture(GL_TEXTURE_2D, m_texture_gluint);
	
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
	glUniform1i(glGetUniformLocation(m_program, "uTextureSampler0"), 0);
	
}

void Texture::bindTexture(){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture_gluint);
}

void Texture::debindTexture(){
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
	}
	if(surface->format->BytesPerPixel == 4){
		format = GL_RGBA;
	}
	
	return format;
}

}
