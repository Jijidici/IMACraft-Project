#include "imacraft/SoundPlayer.hpp"

#include <SDL/SDL_mixer.h>

namespace imacraft{
	SoundPlayer::SoundPlayer(): 
		bo(NULL), putCube(NULL), destroyCube(NULL), putTorch(NULL), changeBloc(NULL){
	}
	
	SoundPlayer::~SoundPlayer(){
		Mix_CloseAudio();
		Mix_FreeMusic(bo);
		Mix_FreeChunk(putCube);
		Mix_FreeChunk(destroyCube);
		Mix_FreeChunk(putTorch);
		Mix_FreeChunk(changeBloc);
	}
}

/*
		private:
			int nbSound;
			Mix_Music* bo;
			Mix_Chunk* putCube;
			Mix_Chunk* destroyCube;
			Mix_Chunk* putTorch;
			Mix_Chunk* changeBloc;
			
		public:
			SoundPlayer();
			~SoundPlayer();
			void init(char* filename);
			void playMusic();
			void playSound(int idxSound);*/
