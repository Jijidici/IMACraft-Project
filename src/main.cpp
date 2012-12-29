#include <iostream>
#include <cstdlib>
#include <sstream>

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
    MatrixStack myStack;
	myStack.set(P);
	
    GLuint	MVLocation = glGetUniformLocation(program, "uMVMatrix");
    GLuint	NormalLocation = glGetUniformLocation(program, "uNormalMatrix");
    
    /* Physical terrain */
    imacraft::TerrainGrid grid;
    grid.readFile("terrain_imacraft.data");
    
    /* Renderer stuff */
    imacraft::CubeInstance model_cube;
    imacraft::Renderer rend(&model_cube, &grid);
    
    /* Material */
    imacraft::Material cubeMat(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.54f, 0.41f, 0.078f), glm::vec3(0.f, 0.f, 0.f), 1000.f);
    imacraft::MaterialUniform cubeMatUniform;
    cubeMatUniform.getLocations("uMaterial", program);
    
    /* Lights */
    imacraft::DirectionalLight sun(glm::vec4(1.f, -1.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
    imacraft::DirectionalLightUniform sunUniform;
    sunUniform.getLocations("uDirLight", program);
    
    imacraft::PointLight torch(glm::vec4(0.f, 0.5f, 0.f, 1.f), glm::vec3(1.f, 1.f, 1.f));
    imacraft::PointLightUniform torchUniform;
    torchUniform.getLocations("uPointLight", program);
    
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
        MatrixStack viewStack;
        viewStack.set(ffCam.getViewMatrix());
		
		sendMaterial(cubeMat, cubeMatUniform);
		
		/********* AFFICHAGE **********/
		myStack.push();			
			/* compute the sun direction in the view space */
			imacraft::DirectionalLight viewSun(viewStack.top() * sun.dir, sun.i);
			sendDirectionalLight(viewSun, sunUniform);
			
			/* compute the torch position in the view space */
			imacraft::PointLight viewTorch(viewStack.top() * torch.lPos, torch.i);
			sendPointLight(viewTorch, torchUniform);
			
			rend.render(myStack, viewStack, MVPLocation, MVLocation, NormalLocation);
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
        if(is_lKeyPressed){ ffCam.moveLeft(0.05); }
		if(is_rKeyPressed){ ffCam.moveLeft(-0.05); }
		if(is_uKeyPressed){ ffCam.moveFront(0.05); }
		if(is_dKeyPressed){ ffCam.moveFront(-0.05); }
		
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
