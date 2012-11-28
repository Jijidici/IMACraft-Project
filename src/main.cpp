#include <iostream>
#include <cstdlib>
#include <cmath>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imac2gl3/shader_tools.hpp"
#include "imac2gl3/shapes/Sphere.hpp"
#include "imac2gl3/shapes/Cone.hpp"
#include "imac2gl3/shapes/gl_shapeinstance.hpp"
#include "imac2gl3/MatrixStack.hpp"
#include "imac2gl3/cameras/FreeFlyCamera.hpp"

#define FRAME_RATE 60

static const Uint32 MIN_LOOP_TIME = 1000/FRAME_RATE;
static const size_t WINDOW_WIDTH = 512, WINDOW_HEIGHT = 512;
static const size_t BYTES_PER_PIXEL = 32;

static const size_t NB_PIC = 200;

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
    
    /** PLACEZ VOTRE CODE DE CREATION DES VBOS/VAOS/SHADERS/... ICI **/
    imac2gl3::Sphere planet_sphere(1.f, 20, 20);
    imac2gl3::GLShapeInstance planet(planet_sphere);    
  	
    imac2gl3::Cone pic_cone(1.f, 0.2f, 20, 5);
    imac2gl3::GLShapeInstance pic(pic_cone);

    GLuint program = imac2gl3::loadProgram("shaders/transform.vs.glsl", "shaders/normalcolor.fs.glsl");
    if(!program){
    	return (EXIT_FAILURE);
    }
    glUseProgram(program);
    
    //MATRIX
    GLint MVPLocation = glGetUniformLocation(program, "uMVPMatrix");
    
    glm::mat4 P = glm::perspective(70.f, WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 1000.f);
    
    
    glEnable(GL_DEPTH_TEST);
    
    //VARIABLE DE MOUVEMENT
    imac2gl3::FreeFlyCamera cam;
    bool start_rotation = false;

    bool is_left_pressed = false;
    bool is_right_pressed = false;
    bool is_up_pressed = false;
    bool is_down_pressed = false;

    float angle_y = 0;
    float angle_x = 0;

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
        
        //Pre-idle
        
        // Dessin
        
        /** PLACEZ VOTRE CODE DE DESSIN ICI **/
        glm::mat4 V = cam.getViewMatrix(); 
        MatrixStack ms;
        ms.set(P);
        ms.mult(V);

        ms.push();
            glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(ms.top()));

            planet.draw();

            ms.push();
                srand(0);

                for(size_t i=0;i<NB_PIC;++i){
                    glm::vec3 random_r_vec = glm::normalize(glm::vec3(0., 0., 1.));
                    GLint random_angle = rand()%360;
                    ms.push();
                        ms.rotate(random_angle, random_r_vec); //Place the pic and the planet
                        ms.translate(glm::vec3(0.f, 0.5f, 0.f)); //place the pic in the planett
                        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(ms.top()));

                        pic.draw();
                    ms.pop();    
                }
                ms.rotate(90, glm::vec3(1.f, 0.f, 0.f)); //Place the pic and the planet
                ms.translate(glm::vec3(0.f, 0.5f, 0.f)); //place the pic in the planett
                glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(ms.top()));

                pic.draw();
            ms.pop();
        ms.pop();
        
        // Mise à jour de l'affichage
        SDL_GL_SwapBuffers();
        
        // Boucle de gestion des évenements
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            // Traitement de l'évenement fermeture de fenêtre
            if(e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                done = true;
                break;
            }
            
            // Traitement des autres évenements:
            switch(e.type){
                case SDL_MOUSEBUTTONDOWN:
                    switch(e.button.button){
                        case SDL_BUTTON_WHEELUP:
                        break;
                        
                        case SDL_BUTTON_WHEELDOWN:
                        break;

                        case SDL_BUTTON_RIGHT:
                        break;

                        default:
                        break;
                    }
                break;

                case SDL_MOUSEBUTTONUP:
                    switch(e.button.button){
                        case SDL_BUTTON_RIGHT:
                        break;

                        default:
                        break;
                    }
                break;

                case SDL_MOUSEMOTION:
                    angle_x = 0.5f*(WINDOW_WIDTH/2. - e.motion.x);
                    angle_y = 0.5f*(WINDOW_HEIGHT/2. - e.motion.y);
                    cam.rotateLeft(angle_x);
                    cam.rotateUp(angle_y);
                break;

                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){

                        case SDLK_z:
                            is_up_pressed = true;
                        break;

                        case SDLK_s:
                            is_down_pressed = true;
                        break;

                        case SDLK_q:
                            is_left_pressed = true;
                        break;

                        case SDLK_d:
                            is_right_pressed = true;
                        break;

                        default:
                        break;
                    }
                break;

                case SDL_KEYUP:
                    switch(e.key.keysym.sym){

                        case SDLK_z:
                            is_up_pressed = false;
                        break;

                        case SDLK_s:
                            is_down_pressed = false;
                        break;

                        case SDLK_q:
                            is_left_pressed = false;
                        break;

                        case SDLK_d:
                            is_right_pressed = false;
                        break;

                        default:
                        break;
                    }
                break;

                default:
                break;
            }

            /** PLACEZ VOTRE CODE DE TRAITEMENT DES EVENTS ICI **/
        }
        
        //idle
        if(is_up_pressed){ cam.moveFront(0.2);}
        if(is_down_pressed){ cam.moveFront(-0.2);}
        if(is_left_pressed){ cam.moveLeft(0.2);}
        if(is_right_pressed){ cam.moveLeft(-0.2);}
        
        
        	// Gestion compteur
        	end = SDL_GetTicks();
        	ellapsedTime = end - start;
        	if(ellapsedTime < MIN_LOOP_TIME){
				SDL_Delay(MIN_LOOP_TIME - ellapsedTime);
			}
    }
    
    // Destruction des ressources OpenGL
    
    /** PLACEZ VOTRE CODE DE DESTRUCTION DES VBOS/VAOS/SHADERS/... ICI **/
    
    
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
