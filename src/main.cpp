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
#include "imacraft/TerrainGrid.hpp"
#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/lighting/Material.hpp"
#include "imacraft/lighting/Lights.hpp"

static const Uint32 MIN_LOOP_TIME = 1000/60;
static const size_t WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
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
    
    glEnable(GL_DEPTH_TEST);
    
    // Creation des ressources OpenGL    
    
    GLuint program = imac2gl3::loadProgram("shaders/transform.vs.glsl", "shaders/normalcolor.fs.glsl");
    if(!program){
		return (EXIT_FAILURE);
	}
    glUseProgram(program);
    
    
    /** Matrices **/
    GLuint MVPLocation = glGetUniformLocation(program, "uMVPMatrix");
    glm::mat4 P = glm::perspective(70.f, WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 1000.f); // tout doit être en float !!!
    
    /* Physical terrain */
    imacraft::TerrainGrid grid;
    grid.readFile("terrain_imacraft.data");
    
    /* Renderer stuff */
    imacraft::CubeInstance model_cube;
    imacraft::Renderer rend(&model_cube, &grid);
    
    /* Material */
    imacraft::Material cubeMat(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.54f, 0.44f, 0.07f), glm::vec3(1.f, 1.f, 1.f), 1.f);
    imacraft::MaterialUniform cubeMatUniform;
    cubeMatUniform.getLocations("uMaterial", program);
    
    //~ Camera vue libre
    imacraft::FreeFlyCamera ffCam;
    
    //variable d'events
	bool is_lKeyPressed = false;
	bool is_rKeyPressed = false;
	bool is_uKeyPressed = false;
	bool is_dKeyPressed = false;
	float ffC_angleX = 0;
	float ffC_angleY = 0;
    
    // Boucle principale
    bool done = false;
    while(!done) {
		// Initilisation compteur
		Uint32 start = 0;
		Uint32 end = 0;
		Uint32 ellapsedTime = 0;
		start = SDL_GetTicks();
    
        // Nettoyage de la fenêtre
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 VP = P * ffCam.getViewMatrix();        
    
		MatrixStack myStack;
		myStack.set(VP);
		
		/********* AFFICHAGE **********/
		sendMaterial(cubeMat, cubeMatUniform);
		myStack.push();
			myStack.translate(glm::vec3(-1.f, -1.f, 0.f));
			rend.render(myStack, MVPLocation);
		myStack.pop();
		
        // Mise à jour de l'affichage
        SDL_GL_SwapBuffers();
        
        // Boucle de gestion des évenements
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
			switch(e.type){
				case SDL_QUIT:
					done = true;
					break;
				
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
						case SDLK_ESCAPE:
							done = true;
							break;
							
						case SDLK_q:
							is_lKeyPressed = true;
							break;

						case SDLK_d:
							is_rKeyPressed = true;
							break;

						case SDLK_z:
							is_uKeyPressed = true;
							break;

						case SDLK_s:
							is_dKeyPressed = true;
							break;
						
						default:
							break;
					}
					break;
					
				case SDL_KEYUP:
					switch(e.key.keysym.sym){							
						case SDLK_q:
							is_lKeyPressed = false;
							break;

						case SDLK_d:
							is_rKeyPressed = false;
							break;

						case SDLK_z:
							is_uKeyPressed = false;
							break;

						case SDLK_s:
							is_dKeyPressed = false;
							break;
						
						default:
							break;
					}
					break;
					
				case SDL_MOUSEMOTION:
					ffC_angleX = 0.6f*(WINDOW_WIDTH/2. - e.motion.x);
					ffC_angleY = 0.6f*(WINDOW_HEIGHT/2. - e.motion.y);
					ffCam.rotateLeft(ffC_angleX);
					ffCam.rotateUp(ffC_angleY);
					break;
							
				default:
					break;
			}
        }
        
        //IDLE
        if(is_lKeyPressed){ ffCam.moveLeft(0.01); }
		if(is_rKeyPressed){ ffCam.moveLeft(-0.01); }
		if(is_uKeyPressed){ ffCam.moveFront(0.01); }
		if(is_dKeyPressed){ ffCam.moveFront(-0.01); }
		
		// Gestion compteur
		end = SDL_GetTicks();
		ellapsedTime = end - start;
		if(ellapsedTime < MIN_LOOP_TIME){
			SDL_Delay(MIN_LOOP_TIME - ellapsedTime);
		}
    }
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
