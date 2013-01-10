#ifndef __SOUND_PLAYER_HPP__
#define __SOUND_PLAYER_HPP__

#include <string>
#include <SDL/SDL_mixer.h>

namespace imacraft{
	class SoundPlayer{
		static const int VOLUME = 32;
	
		private:
			Mix_Music* bo;
			Mix_Chunk* putCube;
			Mix_Chunk* destroyCube;
			Mix_Chunk* putTorch;
			Mix_Chunk* changeBloc;
			Mix_Chunk* jump;
			
		public:
			SoundPlayer();
			~SoundPlayer();
			void init();
			void playMusic();
			void playPutCube();
			void playDestroyCube();
			void playPutTorch();
			void playChangeBloc();
			void playJump();
	};
}

#endif
