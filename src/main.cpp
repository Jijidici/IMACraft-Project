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
#include "imacraft/lighting/Material.hpp"
#include "imacraft/lighting/Lights.hpp"
#include "imacraft/lighting/LightManager.hpp"
#include "imacraft/Texture.hpp"
#include "imacraft/Skybox.hpp"
#include "imacraft/tools.hpp"

#define PI 3.14159265
#define CUBE_SIZE 0.015625

#define CENTER 0
#define NORTH 1
#define EAST 3
#define SOUTH 2
#define WEST 4
#define NORTH_EAST 5
#define SOUTH_EAST 6
#define SOUTH_WEST 7
#define NORTH_WEST 8

#define GROUND 0
#define STONE 1
#define SKY 2

static const Uint32 MIN_LOOP_TIME = 1000/60;
static const size_t WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
static const size_t BYTES_PER_PIXEL = 32;


int main(int argc, char** argv) {
    /********************************************************************
     * INITIALISATION DU PROGRAMME
     ********************************************************************/
    
    /*************/
    /* Conf file */
    /*************/
    
    
    //~ std::cout << value << std::endl;
    //~ std::cout << text << std::endl;
    
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
    grid0.readFile("terrain_imacraft_.data");
    imacraft::TerrainGrid grid1;
    grid1.readFile("terrain_imacraft_.data");
    imacraft::TerrainGrid grid2;
    grid2.readFile("terrain_imacraft_.data");
    imacraft::TerrainGrid grid3;
    grid3.readFile("terrain_imacraft_.data");
    imacraft::TerrainGrid grid4;
    grid4.readFile("terrain_imacraft_.data");
    imacraft::TerrainGrid grid5;
    grid5.readFile("terrain_imacraft_.data");
    imacraft::TerrainGrid grid6;
    grid6.readFile("terrain_imacraft_.data");
    imacraft::TerrainGrid grid7;
    grid7.readFile("terrain_imacraft_.data");
    imacraft::TerrainGrid grid8;
    grid8.readFile("terrain_imacraft_.data");
    
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
		char path[texturesNumber][80];
		
		for(int i = 0; i < texturesNumber; ++i){
			configFile_in >> index;
			configFile_in >> path[index];
		}	
    configFile_in.close();
    
    
    /* Textures */ // create all the textures
    imacraft::Texture defaultTexture("textures/sand.jpg", program);
    imacraft::Texture groundTexture(path[GROUND], program);
    imacraft::Texture stoneTexture(path[STONE], program);
    imacraft::Texture skyTexture(path[SKY], program);
    
    // didn't find how to change number of textures and assign them dynamically...
    std::vector<imacraft::Texture> vecTextures(3, defaultTexture); // create the vector with the number of textures and a texture model, because push_back() method causes allocation issues
    vecTextures[GROUND] = groundTexture;
    vecTextures[STONE] = stoneTexture;
    vecTextures[SKY] = skyTexture;
    
    /* Renderer stuff */
    imacraft::CubeInstance model_cube(defaultTexture); // texture needed in argument, could be replaced by a default texture
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
			
			rend.render(P, viewStack, PLocation, player);
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

							case SDLK_SPACE:
								player.jump();
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
								std::cout << grid0.height() << std::endl;
							
								std::cout << "front : " << std::endl;
								std::cout << player.m_FrontVector.x << std::endl;
								std::cout << player.m_FrontVector.y << std::endl;
								std::cout << player.m_FrontVector.z << std::endl << std::endl;
								
								std::cout << "near normal : " << std::endl;
								std::cout << player.m_frustumNearPlaneNormal.x << std::endl;
								std::cout << player.m_frustumNearPlaneNormal.y << std::endl;
								std::cout << player.m_frustumNearPlaneNormal.z << std::endl << std::endl;
								
								std::cout << "far normal : " << std::endl;
								std::cout << player.m_frustumFarPlaneNormal.x << std::endl;
								std::cout << player.m_frustumFarPlaneNormal.y << std::endl;
								std::cout << player.m_frustumFarPlaneNormal.z << std::endl << std::endl;
								
								std::cout << "left normal : " << std::endl;
								std::cout << player.m_frustumLeftPlaneNormal.x << std::endl;
								std::cout << player.m_frustumLeftPlaneNormal.y << std::endl;
								std::cout << player.m_frustumLeftPlaneNormal.z << std::endl << std::endl;
								
								std::cout << "right normal : " << std::endl;
								std::cout << player.m_frustumRightPlaneNormal.x << std::endl;
								std::cout << player.m_frustumRightPlaneNormal.y << std::endl;
								std::cout << player.m_frustumRightPlaneNormal.z << std::endl << std::endl;
								
								std::cout << "top normal : " << std::endl;
								std::cout << player.m_frustumTopPlaneNormal.x << std::endl;
								std::cout << player.m_frustumTopPlaneNormal.y << std::endl;
								std::cout << player.m_frustumTopPlaneNormal.z << std::endl << std::endl;
								
								std::cout << "bottom normal : " << std::endl;
								std::cout << player.m_frustumBottomPlaneNormal.x << std::endl;
								std::cout << player.m_frustumBottomPlaneNormal.y << std::endl;
								std::cout << player.m_frustumBottomPlaneNormal.z << std::endl << std::endl;
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
					if(player.moveLeft(moveStep)){
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
					if(player.moveLeft(moveStep)){
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
					if(player.moveLeft(moveStep)){
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
					if(player.moveLeft(moveStep)){
						std::cout << "changeGrid !!" << std::endl;
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
					if(player.moveLeft(-moveStep)){
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
					if(player.moveLeft(-moveStep)){
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
					if(player.moveLeft(-moveStep)){
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
					if(player.moveLeft(-moveStep)){
						std::cout << "changeGrid !!" << std::endl;
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
					if(player.moveFront(moveStep)){
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
					if(player.moveFront(moveStep)){
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
					if(player.moveFront(moveStep)){
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
					if(player.moveFront(moveStep)){
						std::cout << "changeGrid !!" << std::endl;
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
					if(player.moveFront(-moveStep)){
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
					if(player.moveFront(-moveStep)){
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
					if(player.moveFront(-moveStep)){
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
					if(player.moveFront(-moveStep)){
						std::cout << "changeGrid !!" << std::endl;
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
		
	//~ rend.writeAllFiles(); // auto save !
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
