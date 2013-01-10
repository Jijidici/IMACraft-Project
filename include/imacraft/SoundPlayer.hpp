#ifndef __SOUND_PLAYER_HPP__
#define __SOUND_PLAYER_HPP__

#include <string>
#include <SDL/SDL_mixer.h>

namespace imacraft{
	class SoundPlayer{
		private:
			Mix_Music* bo;
			Mix_Chunk* putCube;
			Mix_Chunk* destroyCube;
			Mix_Chunk* putTorch;
			Mix_Chunk* changeBloc;
			
		public:
			SoundPlayer();
			~SoundPlayer();
			void init(std::string& filename);
			void playMusic();
	};
}

#endif
