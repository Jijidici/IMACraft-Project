#include "imacraft/SoundPlayer.hpp"

#include <iostream>
#include <stdexcept>
#include <SDL/SDL.h>
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
	
	void SoundPlayer::init(){
			if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
				throw std::runtime_error(Mix_GetError());
			}
			
			//load music
			bo = Mix_LoadMUS("sounds/imacraft_bo.mp3");
			if(bo == NULL){
				std::cout<<"[!]-> Unable to load music bo"<<std::endl;
			}
			
			Mix_VolumeMusic(VOLUME);
			
			//load sound
			
	}
	
	void SoundPlayer::playMusic(){
		Mix_PlayMusic(bo, -1);
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
