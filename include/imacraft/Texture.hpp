#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace imacraft{
	
struct Texture{
	private:
		//~ const char* m_path;
		GLuint m_program;
		//~ GLuint m_texture_gluint;
		SDL_Surface* loadImage();
		//~ GLint m_textureIndex;
		
		void sendTextureToGPU(GLuint &texture_gluint);
		GLuint getFormat(SDL_Surface* surface);
		void getUniformSamplerLocation();
		
	public:
		Texture();
		Texture(const char* path, GLuint &program);
		~Texture();
		
		/**/
		GLint m_textureIndex;
		GLuint m_texture_gluint;
		const char* m_path;
		/**/
		
		void print_test();
		
		void bindTexture();
		void debindTexture();
		
		static GLint textureIndex;
};

}


#endif // _TEXTURE_HPP_
