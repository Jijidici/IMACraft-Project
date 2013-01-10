#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace imacraft{
	
struct Texture{
	private:
		const char* m_path;
		GLuint m_program;
		GLuint m_texture_gluint;
		GLint m_textureIndex;
		
		SDL_Surface* loadImage();
		void sendTextureToGPU();
		void getUniformSamplerLocation();
		
	public:
		Texture(const char* path, GLuint &program);
		~Texture();
		
		void print_test();
		
		void bindTexture();
		void debindTexture();
		
		static GLuint getFormat(SDL_Surface* surface);
};

}


#endif // _TEXTURE_HPP_
