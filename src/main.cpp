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
#include "imacraft/Player.hpp" 
#include "imacraft/Renderer.hpp"
#include "imacraft/TerrainGrid.hpp"
#include "imacraft/shapes/CubeInstance.hpp"
#include "imacraft/lighting/Material.hpp"
#include "imacraft/lighting/Lights.hpp"
#include "imacraft/lighting/LightManager.hpp"

#define PI 3.14159265

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
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    
    // Creation des ressources OpenGL    
    GLuint program = imac2gl3::loadProgram("shaders/transform.vs.glsl", "shaders/normalcolor.fs.glsl");
    if(!program){
		return (EXIT_FAILURE);
	}
    glUseProgram(program);
    
    /** Matrices **/
    GLuint MVPLocation = glGetUniformLocation(program, "uMVPMatrix");
    glm::mat4 P = glm::perspective(70.f, WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.001f, 1000.f); // tout doit être en float !!!
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
    imacraft::PointLight torch(glm::vec4(0.f, 0.5f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f));
    
    imacraft::LightManager lMage;
    lMage.addLight(sun);
    lMage.addLight(torch);
    torch.lPos.z = -1.f;
    lMage.addLight(torch);
    
    //~ Camera vue libre
    imacraft::FreeFlyCamera ffCam;
    imacraft::Player player;
    
    //variable d'events
		bool is_lKeyPressed = false;
		bool is_rKeyPressed = false;
		bool is_uKeyPressed = false;
		bool is_dKeyPressed = false;
		float ffC_angleX = 0;
		float ffC_angleY = 0;
		uint32_t currentCube;
    
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
        viewStack.set(player.getViewMatrix());
		
		sendMaterial(cubeMat, cubeMatUniform);
		
		/********* AFFICHAGE **********/
		viewStack.push();	
			//viewStack.translate(glm::vec3(-1.f, -1.f, 0.f));		
			
			lMage.sendLights(program, viewStack.top());
			
			rend.render(myStack, viewStack, MVPLocation, MVLocation, NormalLocation);
		viewStack.pop();
		
        // Mise à jour de l'affichage
        SDL_GL_SwapBuffers();
        
        //affichage position du perso i,j,k
		player.computeCubePosition(grid.width(),grid.height());
		//std::cout << "i : " << player.getCubePosition().x << " /// j : " << player.getCubePosition().y << " /// k : " << player.getCubePosition().z << std::endl;
        
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

							case SDLK_SPACE:
								player.jump();
								if(grid[player.getCubePosition().z*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x] ==0){
									player.fall(CUBE_SIZE);
								}
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
						if(ffC_angleX < 180 && ffC_angleX > -180) player.rotateLeft(ffC_angleX); //empêcher le perso de faire un tour sur lui même
						if(ffC_angleY >= 30) ffC_angleY = 30;
						if(ffC_angleY <= -40) ffC_angleY = -40;
						player.rotateUp(ffC_angleY);
						break;
							
					default:
						break;
				}
		      }
		      
		  //IDLE - GESTION DES COLLISIONS
			//GAUCHE
		  if(is_lKeyPressed){
				if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
					if(grid[player.getCubePosition().z*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x+1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x+1" << std::endl;
					}else player.moveLeft(0.01);
				}
				else if(player.getViewAngle() >= PI/4 && player.getViewAngle() < 3*PI/4){
					if(grid[(player.getCubePosition().z-1)*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z-1" << std::endl;
					}else player.moveLeft(0.01);
				}
				else if((player.getViewAngle() >= 3*PI/4 && player.getViewAngle() < PI)||(player.getViewAngle() >= -PI && player.getViewAngle() < -3*PI/4)){
					if(grid[player.getCubePosition().z*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x-1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x-1" << std::endl;
					}else player.moveLeft(0.01);
				}
				else if(player.getViewAngle() >= -3*PI/4 && player.getViewAngle() < -PI/4){
					if(grid[(player.getCubePosition().z+1)*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z+1" << std::endl;
					}else player.moveLeft(0.01);
				}
			}
			//DROITE
			if(is_rKeyPressed){
				if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
					if(grid[player.getCubePosition().z*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x-1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x+1" << std::endl;
					}else player.moveLeft(-0.01);
				}
				else if(player.getViewAngle() >= PI/4 && player.getViewAngle() < 3*PI/4){
					if(grid[(player.getCubePosition().z+1)*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z+1" << std::endl;
					}else player.moveLeft(-0.01);
				}
				else if((player.getViewAngle() >= 3*PI/4 && player.getViewAngle() < PI)||(player.getViewAngle() >= -PI && player.getViewAngle() < -3*PI/4)){
					if(grid[player.getCubePosition().z*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x+1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x-1" << std::endl;
					}else player.moveLeft(-0.01);
				}
				else if(player.getViewAngle() >= -3*PI/4 && player.getViewAngle() < -PI/4){
					if(grid[(player.getCubePosition().z-1)*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z-1" << std::endl;
					}else player.moveLeft(-0.01);
				}
			}
			//EN AVANT
			if(is_uKeyPressed){
				if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
					if(grid[(player.getCubePosition().z +1)*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z+1" << std::endl;
					}else player.moveFront(0.01);
				}
				else if(player.getViewAngle() >= PI/4 && player.getViewAngle() < 3*PI/4){
					if(grid[player.getCubePosition().z*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x+1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x-1" << std::endl;
					}else player.moveFront(0.01);
				}
				else if((player.getViewAngle() >= 3*PI/4 && player.getViewAngle() < PI)||(player.getViewAngle() >= -PI && player.getViewAngle() < -3*PI/4)){
					if(grid[(player.getCubePosition().z -1)*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z-1" << std::endl;
					}else player.moveFront(0.01);
				}
				else if(player.getViewAngle() >= -3*PI/4 && player.getViewAngle() < -PI/4){
					if(grid[player.getCubePosition().z*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x-1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x+1" << std::endl;
					}else player.moveFront(0.01);
				}
			}
			//EN ARRIERE
			if(is_dKeyPressed){ 
				if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
					if(grid[(player.getCubePosition().z -1)*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z-1" << std::endl;
					}else player.moveFront(-0.01);
				}
				else if(player.getViewAngle() >= PI/4 && player.getViewAngle() < 3*PI/4){
					if(grid[player.getCubePosition().z*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x-1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x+1" << std::endl;
					}else player.moveFront(-0.01);
				}
				else if((player.getViewAngle() >= 3*PI/4 && player.getViewAngle() < PI)||(player.getViewAngle() >= -PI && player.getViewAngle() < -3*PI/4)){
					if(grid[(player.getCubePosition().z +1)*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z+1" << std::endl;
					}else player.moveFront(-0.01);
				}
				else if(player.getViewAngle() >= -3*PI/4 && player.getViewAngle() < -PI/4){
					if(grid[player.getCubePosition().z*grid.width()*grid.height() + player.getCubePosition().y*grid.width() + player.getCubePosition().x+1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x-1" << std::endl;
					}else player.moveFront(-0.01);
				}
			}
		
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
