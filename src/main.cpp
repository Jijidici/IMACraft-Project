#include <iostream>
#include <cstdlib>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <vector>

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
#include "imacraft/Texture.hpp"
#include "imacraft/Skybox.hpp"
#include "imacraft/tools.hpp"

#define PI 3.14159265

#define CENTER 0
#define NORTH 1
#define EAST 3
#define SOUTH 2
#define WEST 4
#define NORTH_EAST 5
#define SOUTH_EAST 6
#define SOUTH_WEST 7
#define NORTH_WEST 8

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
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    
    // Creation des ressources OpenGL    
    GLuint program = imac2gl3::loadProgram("shaders/transform.vs.glsl", "shaders/normalcolor.fs.glsl");
    if(!program){
		return (EXIT_FAILURE);
	}
    glUseProgram(program);
    
    /** Matrices **/
    GLuint PLocation = glGetUniformLocation(program, "uPMatrix");
    glm::mat4 P = glm::perspective(90.f, WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.001f, 1000.f);
	
	//~ FreeFlyCamera
    imacraft::Player player;
    
    /* Physical terrain */
    imacraft::TerrainGrid grid0;
    grid0.readFile("terrain_imacraft_test_.data");
    imacraft::TerrainGrid grid1;
    grid1.readFile("terrain_imacraft_test_.data");
    imacraft::TerrainGrid grid2;
    grid2.readFile("terrain_imacraft_test_.data");
    imacraft::TerrainGrid grid3;
    grid3.readFile("terrain_imacraft_test_.data");
    imacraft::TerrainGrid grid4;
    grid4.readFile("terrain_imacraft_test_.data");
    imacraft::TerrainGrid grid5;
    grid5.readFile("terrain_imacraft_test_.data");
    imacraft::TerrainGrid grid6;
    grid6.readFile("terrain_imacraft_test_.data");
    imacraft::TerrainGrid grid7;
    grid7.readFile("terrain_imacraft_test_.data");
    imacraft::TerrainGrid grid8;
    grid8.readFile("terrain_imacraft_test_.data");
    
    std::vector<imacraft::TerrainGrid*> vecGrid(9);
    vecGrid[CENTER] = &grid0;
    vecGrid[NORTH] = &grid1;
    vecGrid[SOUTH] = &grid2;
    vecGrid[EAST] = &grid3;
    vecGrid[WEST] = &grid4;
    vecGrid[NORTH_EAST] = &grid5;
    vecGrid[NORTH_WEST] = &grid6;
    vecGrid[SOUTH_EAST] = &grid7;
    vecGrid[SOUTH_WEST] = &grid8;
    
    if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
		std::cout << "error while loading grids" << std::endl;
	}
    
    
    /* Textures */ // create all the textures
    imacraft::Texture brickTexture("textures/brique.png", program);
    imacraft::Texture stoneTexture("textures/stone.png", program);
    imacraft::Texture skyTexture("textures/sky.png", program);
    
    std::vector<imacraft::Texture> vecTextures(3, brickTexture); // create the vector with the number of textures and a texture model, because push_back() method causes allocation issues
    vecTextures[1] = stoneTexture;
    vecTextures[2] = skyTexture;
    
    /* Renderer stuff */
    imacraft::CubeInstance model_cube(brickTexture); // texture needed in argument, could be replaced by a default texture
    imacraft::Skybox sky(program, &player, &model_cube);
    imacraft::Renderer rend(&model_cube, vecGrid, vecTextures, sky);
    
    /* Material */
    imacraft::Material cubeMat(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.54f, 0.41f, 0.078f), glm::vec3(0.f, 0.f, 0.f), 1000.f);
    imacraft::MaterialUniform cubeMatUniform;
    cubeMatUniform.getLocations("uMaterial", program);
    
    /* Lights */
    imacraft::DirectionalLight sun(glm::vec4(1.f, -1.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
    imacraft::PointLight torch(glm::vec4(0.f, 0.5f, 1.f, 1.f), glm::vec3(0.2f, 0.2f, 0.2f));
    
    imacraft::LightManager lMage;
    lMage.addLight(sun);
    lMage.addLight(torch);

    
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
        viewStack.set(player.getViewMatrix());
		
		sendMaterial(cubeMat, cubeMatUniform);
		
		/********* AFFICHAGE **********/
		viewStack.push();
			lMage.sendLights(program, viewStack.top());
			
			rend.render(P, viewStack, PLocation);
		viewStack.pop();
		
        // Mise à jour de l'affichage
        SDL_GL_SwapBuffers();
        
        //affichage position du perso i,j,k
		player.computeCubePosition( (*vecGrid[CENTER]).width(),(*vecGrid[CENTER]).height() );
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
								if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] ==0){
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
								
							case SDLK_t:
								rend.writeAllFiles();
								//~ grid.writeFile("test");
								break;
								
							case SDLK_p:
								player.printCurrentNEPosition();
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
					
					case SDL_MOUSEBUTTONDOWN:
						//Manage the view target
						player.whatCubeTargeted(vecGrid);
						break;
					
					default:
						break;
				}
		      }
		      
		  //IDLE - GESTION DES COLLISIONS
			//GAUCHE
		  if(is_lKeyPressed){
				if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
					if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x+1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x+1" << std::endl;
					}else{
						if(player.moveLeft(0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if(player.getViewAngle() >= PI/4 && player.getViewAngle() < 3*PI/4){
					if((*vecGrid[CENTER])[(player.getCubePosition().z-1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z-1" << std::endl;
					}else{
						if(player.moveLeft(0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if((player.getViewAngle() >= 3*PI/4 && player.getViewAngle() < PI)||(player.getViewAngle() >= -PI && player.getViewAngle() < -3*PI/4)){
					if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x-1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x-1" << std::endl;
					}else{
						if(player.moveLeft(0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if(player.getViewAngle() >= -3*PI/4 && player.getViewAngle() < -PI/4){
					if((*vecGrid[CENTER])[(player.getCubePosition().z+1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z+1" << std::endl;
					}else{
						if(player.moveLeft(0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
			}
			//DROITE
			if(is_rKeyPressed){
				if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
					if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x-1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x+1" << std::endl;
					}else{
						if(player.moveLeft(-0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if(player.getViewAngle() >= PI/4 && player.getViewAngle() < 3*PI/4){
					if((*vecGrid[CENTER])[(player.getCubePosition().z+1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z+1" << std::endl;
					}else{
						if(player.moveLeft(-0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if((player.getViewAngle() >= 3*PI/4 && player.getViewAngle() < PI)||(player.getViewAngle() >= -PI && player.getViewAngle() < -3*PI/4)){
					if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x+1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x-1" << std::endl;
					}else{
						if(player.moveLeft(-0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if(player.getViewAngle() >= -3*PI/4 && player.getViewAngle() < -PI/4){
					if((*vecGrid[CENTER])[(player.getCubePosition().z-1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z-1" << std::endl;
					}else{
						if(player.moveLeft(-0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
			}
			//EN AVANT
			if(is_uKeyPressed){
				if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
					if((*vecGrid[CENTER])[(player.getCubePosition().z +1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z+1" << std::endl;
					}else{
						if(player.moveFront(0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if(player.getViewAngle() >= PI/4 && player.getViewAngle() < 3*PI/4){
					if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x+1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x-1" << std::endl;
					}else{
						if(player.moveFront(0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if((player.getViewAngle() >= 3*PI/4 && player.getViewAngle() < PI)||(player.getViewAngle() >= -PI && player.getViewAngle() < -3*PI/4)){
					if((*vecGrid[CENTER])[(player.getCubePosition().z -1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z-1" << std::endl;
					}else{
						if(player.moveFront(0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if(player.getViewAngle() >= -3*PI/4 && player.getViewAngle() < -PI/4){
					if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x-1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x+1" << std::endl;
					}else{
						if(player.moveFront(0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
			}
			//EN ARRIERE
			if(is_dKeyPressed){ 
				if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
					if((*vecGrid[CENTER])[(player.getCubePosition().z -1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z-1" << std::endl;
					}else{
						if(player.moveFront(-0.01)){
							std::cout << "changeGrid !!" << std::endl;
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if(player.getViewAngle() >= PI/4 && player.getViewAngle() < 3*PI/4){
					if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x-1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x+1" << std::endl;
					}else{
						if(player.moveFront(-0.01)){
							std::cout << "changeGrid !!" << std::endl;
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if((player.getViewAngle() >= 3*PI/4 && player.getViewAngle() < PI)||(player.getViewAngle() >= -PI && player.getViewAngle() < -3*PI/4)){
					if((*vecGrid[CENTER])[(player.getCubePosition().z +1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
						player.blockZ(player.getPosition().z);
						std::cout << "collision z+1" << std::endl;
					}else{
						if(player.moveFront(-0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
				}
				else if(player.getViewAngle() >= -3*PI/4 && player.getViewAngle() < -PI/4){
					if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x+1] !=0) {
						player.blockX(player.getPosition().x);
						std::cout << "collision x-1" << std::endl;
					}else{
						if(player.moveFront(-0.01)){
							std::cout << "changeGrid !!" << std::endl;
							rend.writeAllFiles();
							if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
								std::cout << "error while loading grids" << std::endl;
							}
						}
					}
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
