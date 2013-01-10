#include "imacraft/SoundPlayer.hpp"

#include <iostream>
#include <stdexcept>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

namespace imacraft{
	SoundPlayer::SoundPlayer(): 
		bo(NULL), putCube(NULL), destroyCube(NULL), putTorch(NULL), changeBloc(NULL), jump(NULL){
	}
	
	SoundPlayer::~SoundPlayer(){
		Mix_CloseAudio();
		Mix_FreeMusic(bo);
		Mix_FreeChunk(putCube);
		Mix_FreeChunk(destroyCube);
		Mix_FreeChunk(putTorch);
		Mix_FreeChunk(changeBloc);
		Mix_FreeChunk(jump);
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
			Mix_AllocateChannels(5);
			
			putCube = Mix_LoadWAV("sounds/put_cube.wav");
			if(putCube == NULL){
				std::cout<<"[!]-> Unable to load put_cube sound"<<std::endl;
			}
			
			destroyCube = Mix_LoadWAV("sounds/destroy_cube.wav");
			if(destroyCube == NULL){
				std::cout<<"[!]-> Unable to load destroy_cube sound"<<std::endl;
			}
			
			putTorch = Mix_LoadWAV("sounds/put_torch.wav");
			if(putTorch == NULL){
				std::cout<<"[!]-> Unable to load put_torch sound"<<std::endl;
			}
			
			changeBloc = Mix_LoadWAV("sounds/change_bloc.wav");
			if(changeBloc == NULL){
				std::cout<<"[!]-> Unable to load change_bloc sound"<<std::endl;
			}
			
			jump = Mix_LoadWAV("sounds/jump.wav");
			if(jump == NULL){
				std::cout<<"[!]-> Unable to load jump sound"<<std::endl;
			}
			
			Mix_Volume(-1, VOLUME);
	}
	
	void SoundPlayer::playMusic(){
		Mix_PlayMusic(bo, -1);
	}
	
	void SoundPlayer::playPutCube(){
		Mix_PlayChannel(-1, putCube, 0);
	}
	
	void SoundPlayer::playDestroyCube(){
		Mix_PlayChannel(-1, destroyCube, 0);
	}
	
	void SoundPlayer::playPutTorch(){
		Mix_PlayChannel(-1, putTorch, 0);
	}
	
	void SoundPlayer::playChangeBloc(){
		Mix_PlayChannel(-1, changeBloc, 0);
	}
	
	void SoundPlayer::playJump(){
		Mix_PlayChannel(-1, jump, 0);
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
