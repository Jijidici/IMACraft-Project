#include <iostream>
#include <cstdlib>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imacraft/shader_tools.hpp"

#include "imacraft/MatrixStack.hpp"

#include "imacraft/cameras/FreeFlyCamera.hpp" 

#include "imacraft/Renderer.hpp"


static const size_t WINDOW_WIDTH = 512, WINDOW_HEIGHT = 512;
static const size_t BYTES_PER_PIXEL = 32;

int main(int argc, char** argv) {
    /********************************************************************
     * INITIALISATION DU PROGRAMME
     ********************************************************************/
    
    // Initialisation de la SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    // Creation de la fenêtre et d'un contexte OpenGL
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BYTES_PER_PIXEL, SDL_OPENGL);
    
    // Initialisation de GLEW
    GLenum error;
    if(GLEW_OK != (error = glewInit())) {
        std::cerr << "Impossible d'initialiser GLEW: " << glewGetErrorString(error) << std::endl;
        return EXIT_FAILURE;
    }
    
    // Creation des ressources OpenGL    
    
    
    GLuint program = imac2gl3::loadProgram("shaders/transform.vs.glsl", "shaders/normalcolor.fs.glsl");
    if(!program){
		return (EXIT_FAILURE);
	}
    glUseProgram(program);
    
    
    /** Matrices **/
    GLuint MVPLocation = glGetUniformLocation(program, "uMVPMatrix");
    
    glm::mat4 P = glm::perspective(70.f, WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 1000.f); // tout doit être en float !!!
    
    
    //~ Camera vue libre
    FreeFlyCamera V;
	
    V.moveFront(-5.f);
    
    int posX = 0;
    int posY = 0;
    
    // Boucle principale
    bool done = false;
    while(!done) {
        // Nettoyage de la fenêtre
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        glm::mat4 VP = P * V.getViewMatrix();
    
		MatrixStack myStack;
		myStack.set(VP);
		
		/********* AFFICHAGE **********/
		
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(myStack.top()));


        // Mise à jour de l'affichage
        SDL_GL_SwapBuffers();
        
        
        Uint8 *keystate = SDL_GetKeyState(NULL);
        
        // Boucle de gestion des évenements
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            // Traitement de l'évenement fermeture de fenêtre
            if(e.type == SDL_QUIT) {
                done = true;
                break;
            }
            if(e.type == SDL_KEYUP){
            	if(e.key.keysym.sym == SDLK_ESCAPE){
            		done = true;
            		break;
            	}
            }
            // Traitement des autres évenements:
            
            /** PLACEZ VOTRE CODE DE TRAITEMENT DES EVENTS ICI **/
            if(SDL_GetRelativeMouseState(&posX, &posY)&SDL_BUTTON(SDL_BUTTON_RIGHT)){
				//std::cout << "posX : " << posX << " posY : " << posY << std::endl;
				V.rotateLeft(-posX);
				V.rotateUp(-posY);
			}
			
			if(keystate[SDLK_z]){
				V.moveFront(0.1);
			}
			if(keystate[SDLK_s]){
				V.moveFront(-0.1);
			}
			if(keystate[SDLK_q]){
				V.moveLeft(0.1);
			}
			if(keystate[SDLK_d]){
				V.moveLeft(-0.1);
			}
        }
    }
    
    // Destruction des ressources OpenGL
    
    /** PLACEZ VOTRE CODE DE DESTRUCTION DES VBOS/VAOS/SHADERS/... ICI **/
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
