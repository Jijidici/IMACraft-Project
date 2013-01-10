#include <iostream>
#include <fstream>
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
#include "imacraft/shapes/QuadInstance.hpp"
#include "imacraft/lighting/Material.hpp"
#include "imacraft/lighting/Lights.hpp"
#include "imacraft/lighting/LightManager.hpp"
#include "imacraft/Texture.hpp"
#include "imacraft/Skybox.hpp"
#include "imacraft/tools.hpp"

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
    grid0.readFile("default.data");
    imacraft::TerrainGrid grid1;
    grid1.readFile("default.data");
    imacraft::TerrainGrid grid2;
    grid2.readFile("default.data");
    imacraft::TerrainGrid grid3;
    grid3.readFile("default.data");
    imacraft::TerrainGrid grid4;
    grid4.readFile("default.data");
    imacraft::TerrainGrid grid5;
    grid5.readFile("default.data");
    imacraft::TerrainGrid grid6;
    grid6.readFile("default.data");
    imacraft::TerrainGrid grid7;
    grid7.readFile("default.data");
    imacraft::TerrainGrid grid8;
    grid8.readFile("default.data");
    
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

    
    /* Textures */ // create all the textures from the config file
    std::vector<imacraft::Texture*> texturePtVector;
	
	// textures config file
    const char* configPath = "config/textures.config";
	
    std::ifstream configFile_in;
    configFile_in.open(configPath);
		if(!configFile_in){
			std::cout << "Unable to open textures config file" << std::endl;
			return(EXIT_FAILURE);
		}
    
		int texturesNumber;
		int index;
		configFile_in >> texturesNumber;
		char path[80];
		
		for(int i = 0; i < texturesNumber; ++i){
			configFile_in >> index; // useless, but needed to read the index in the file
			configFile_in >> path;
			
			imacraft::Texture * tempTexture = new imacraft::Texture(path, program);
			texturePtVector.push_back(tempTexture);
		}	
    configFile_in.close();
    
    
    /* Material */
    imacraft::Material cubeMat(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.54f, 0.41f, 0.078f), glm::vec3(0.f, 0.f, 0.f), 1000.f);
    imacraft::MaterialUniform cubeMatUniform;
    cubeMatUniform.getLocations("uMaterial", program);
    
    /* Lights */
    imacraft::DirectionalLight sun(glm::vec4(1.f, -1.f, 1.f, 0.f), glm::vec3(1.5f, 1.5f, 1.5f));
    
    imacraft::LightManager lMage;
    lMage.addLight(sun);
    
    /* Renderer stuff */
    imacraft::Texture defaultTexture("textures/sand.jpg", program);
    imacraft::CubeInstance model_cube(defaultTexture); // texture needed in argument, could be replaced by a default texture
    imacraft::QuadInstance model_quad(defaultTexture);
    imacraft::Skybox sky(&player, &model_cube);
    imacraft::Renderer rend(&model_cube, &model_quad, vecGrid, texturePtVector, sky);
    
    //variable d'events
	bool is_lKeyPressed = false;
	bool is_rKeyPressed = false;
	bool is_uKeyPressed = false;
	bool is_dKeyPressed = false;
	float ffC_angleY = 0;
	float old_positionX = 0.;
	float new_positionX = 0.;
	float new_positionY = 0.;
	float gravity = 0.01;
    
    float moveStep = 0.005;
    
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
			
			rend.render(program, P, viewStack, PLocation, player, lMage);
		viewStack.pop();
		
        // Mise à jour de l'affichage
        SDL_GL_SwapBuffers();
        
        //affichage position du perso i,j,k
		player.computeCubePosition( (*vecGrid[CENTER]).width(),(*vecGrid[CENTER]).height() );

		SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_ShowCursor(SDL_DISABLE);

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
							
							//jump
							case SDLK_SPACE:
								player.jump();
								break;
							
							//turn off the older light
							case SDLK_o:
								lMage.removeLight(0);
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
						new_positionX = e.motion.x;
						new_positionY = e.motion.y;
						ffC_angleY = 0.6f*(WINDOW_HEIGHT/2. - e.motion.y);
						if(ffC_angleY >= 90) ffC_angleY = 90;
						if(ffC_angleY <= -70) ffC_angleY = -70;

						player.rotateUp(ffC_angleY);
						
						break;
					
					case SDL_MOUSEBUTTONDOWN:
						int idxGrid;
						switch(e.button.button){
							case SDL_BUTTON_LEFT:
								//destroy a cube
								idxGrid = player.whatCubeTargeted(vecGrid);
								if(idxGrid != -1){
									(*vecGrid[idxGrid]).removeCube(player.getSeenPosInCube());
								}
								break;
							
							case SDL_BUTTON_RIGHT:
								//create a cube
								idxGrid = player.whatCubeTargeted(vecGrid);
								if(idxGrid != -1){
									glm::ivec3 camPos = player.getCubePosition();
									glm::vec3 camFrontVector = player.getFrontVector();
									float step = CUBE_SIZE/8.;
									glm::vec3 previousPos = player.getSeenPosInCube() - step * camFrontVector;
									//manage the grid changing
									if(previousPos.x > 1){
										previousPos.x = -1;
										
									}else if(previousPos.x < -1){
										previousPos.x = 1;
										
									}else if(previousPos.z > 1){
										previousPos.z = -1;
										
									}else if(previousPos.z < -1){
										previousPos.z = 1;
									}
									
									glm::ivec3 previousCube = imacraft::TerrainGrid::getCubeIntegerPosition(previousPos);
									if(previousCube != camPos){
										(*vecGrid[CENTER]).addCube(previousPos, uint8_t(1)); //replace 1 by the type of cube
									}
								}
								break;
							
							//put a torch
							case SDL_BUTTON_MIDDLE:
								idxGrid = player.whatCubeTargeted(vecGrid);
								if(idxGrid != -1){
									glm::vec3 camFrontVector = player.getFrontVector();
									float step = CUBE_SIZE/8.;
									glm::vec3 previousPos = player.getSeenPosInCube() - step * camFrontVector;
									glm::ivec3 previousCube = imacraft::TerrainGrid::getCubeIntegerPosition(previousPos);
									imacraft::PointLight tmpTorch(glm::vec4(previousCube.x*CUBE_SIZE-1, previousCube.y*CUBE_SIZE-1, previousCube.z*CUBE_SIZE-1, 1.f), glm::vec3(0.05f, 0.05f, 0.05f));
									lMage.addLight(tmpTorch);
								}
								break;
							
							//increase and decreas the field of view
							case SDL_BUTTON_WHEELUP:
								player.changeFrustumFar(0.05);
								break;
								
							case SDL_BUTTON_WHEELDOWN:
								player.changeFrustumFar(-0.05);
								break;
							
							default:
								break;
						}
						break;
					
					default:
						break;
			}
		}
		
		
		//Gestion of gravity
		if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + (player.getCubePosition().y-1)*(*vecGrid[CENTER]).width() + player.getCubePosition().x] == 0){
			player.fall(gravity);
		}
    
		//IDLE - GESTION DES COLLISIONS
		//GAUCHE
		if(is_lKeyPressed){
			if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
				if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x+1] !=0) {
					player.blockX(player.getPosition().x);
					std::cout << "collision x+1" << std::endl;
				}else{
					int exitSide = player.moveLeft(moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveLeft(moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveLeft(moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveLeft(moveStep);
					if(exitSide){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
						rend.writeAllFiles();
						if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
							std::cout << "error while loading grids" << std::endl;
						}
					}
				}
			}
		} // end left
		//DROITE
		if(is_rKeyPressed){
			if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
				if((*vecGrid[CENTER])[player.getCubePosition().z*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x-1] !=0) {
					player.blockX(player.getPosition().x);
					std::cout << "collision x+1" << std::endl;
				}else{
					int exitSide = player.moveLeft(-moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveLeft(-moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveLeft(-moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveLeft(-moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
						rend.writeAllFiles();
						if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
							std::cout << "error while loading grids" << std::endl;
						}
					}
				}
			}
		} // end right
		//EN AVANT
		if(is_uKeyPressed){
			if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
				if((*vecGrid[CENTER])[(player.getCubePosition().z +1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
					player.blockZ(player.getPosition().z);
					std::cout << "collision z+1" << std::endl;
				}else{
					int exitSide = player.moveFront(moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveFront(moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveFront(moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveFront(moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
						rend.writeAllFiles();
						if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
							std::cout << "error while loading grids" << std::endl;
						}
					}
				}
			}
		} // end front
		//EN ARRIERE
		if(is_dKeyPressed){ 
			if(player.getViewAngle() >= -PI/4 && player.getViewAngle() < PI/4){
				if((*vecGrid[CENTER])[(player.getCubePosition().z -1)*(*vecGrid[CENTER]).width()*(*vecGrid[CENTER]).height() + player.getCubePosition().y*(*vecGrid[CENTER]).width() + player.getCubePosition().x] !=0) {
					player.blockZ(player.getPosition().z);
					std::cout << "collision z-1" << std::endl;
				}else{
					int exitSide = player.moveFront(-moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
						rend.writeAllFiles();
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
					int exitSide = player.moveFront(-moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
						rend.writeAllFiles();
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
					int exitSide = player.moveFront(-moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
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
					int exitSide = player.moveFront(-moveStep);
					if(exitSide != 0){
						std::cout << "changeGrid !!" << std::endl;
						lMage.updatePtLightsPosition(exitSide);
						rend.writeAllFiles();
						if(loadGrids(player.getCurrentNorthPosition(), player.getCurrentEastPosition(), vecGrid) == false){
							std::cout << "error while loading grids" << std::endl;
						}
					}
				}
			}
		} // end right
		
		//Gestion rotation
		if(new_positionX >= WINDOW_WIDTH-1){
			SDL_WarpMouse(0, new_positionY);
			old_positionX = 0-(old_positionX - new_positionX);
			new_positionX = 0;
		}else if(new_positionX <= 0){
			SDL_WarpMouse(WINDOW_WIDTH, new_positionY);
			old_positionX = WINDOW_WIDTH+(old_positionX - new_positionX);
			new_positionX = WINDOW_WIDTH;
		}
		player.rotateLeft((old_positionX - new_positionX)*0.6);
		old_positionX = new_positionX;

		// Gestion compteur
		end = SDL_GetTicks();
		ellapsedTime = end - start;
		if(ellapsedTime < MIN_LOOP_TIME){
			SDL_Delay(MIN_LOOP_TIME - ellapsedTime);
		}
		
	} // end events
		
	rend.writeAllFiles(); // auto save !
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
