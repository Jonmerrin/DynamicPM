#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h, float t, std::string path){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;

    // Initialize our variables
    filePath = path;
    tolerance = t;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Lab",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                w,
                                h,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();


    // Setup our Renderer
    m_renderer = new Renderer(w,h);    
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    if(m_renderer!=nullptr){
        delete m_renderer;
    }


    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;

	return success;
}

// ====================== Create the planets =============
// NOTE: I will admit it is a little lazy to have these as globals,
//       we could build on our ObjectManager structure to manage the
//       each object that is inserted and automatically create a 'SceneNode'
//       for it for example. Or otherwise, build a 'SceneTree' class that
//       manages creating nodes and setting their type.
//       (e.g. A good use of the Factory Design Pattern if you'd like to
//             invetigate further--but it is beyond the scope of this assignment).

// ====================== Create the planets =============

//Loops forever!
void SDLGraphicsProgram::Loop(){
    bool rotating = false;
    bool followCamera = true;
    Object* reducingObj = new Object();
    Object* sceneCenter = new Object();
    // Object* referenceObj = new Object();

    float distanceFromViewFrustum = 0.0f; // Replace this with an object in the seen under node.
                                          // Just a drawn rect would do fine.
    std::cout << "TOLERANCE BEFORE MAKEOBJECTFROMFILE: " << tolerance << std::endl;
    reducingObj->MakeObjectFromFile(filePath, tolerance);
    
    std::cout << "TOLERANCE AFTER MAKEOBJECTFROMFILE: " << tolerance << std::endl;
    std::cout << "ARBITRARY SUCCESS CHECK 1" << std::endl;
    // referenceObj->MakeObjectFromFile(filePath, tolerance);
    std::cout << "ARBITRARY SUCCESS CHECK 2" << std::endl;
    SceneNode* node = new SceneNode(sceneCenter);
    SceneNode* reduceObjNode = new SceneNode(reducingObj);
    // SceneNode* refObjNode = new SceneNode(referenceObj);
    
    // node->AddChild(refObjNode);
    node->AddChild(reduceObjNode);
    reduceObjNode->GetLocalTransform().Translate(0,0,-reducingObj->GetWidth()/2);
    // refObjNode->GetLocalTransform().Translate(referenceObj->GetWidth()/2 + 1, 0, 0);

    m_renderer->setRoot(node);
    // Set a default position for our camera
    m_renderer->GetCamera(0)->SetCameraEyePosition(0.0f,0.5f,50.0f);
    m_renderer->GetCamera(1)->SetCameraEyePosition(0.0f,0.5f,50.0f);

    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 0.5f;
    // std::cout << "STARTING MAIN LOOP" << std::endl;
    // While application is running
    while(!quit){

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboad input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
            //  m_renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
            }
            switch(e.type){
                // Handle keyboard presses
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_LEFT:
                            m_renderer->GetCamera(0)->MoveLeft(cameraSpeed);
                            break;
                        case SDLK_RIGHT:
                            m_renderer->GetCamera(0)->MoveRight(cameraSpeed);
                            break;
                        case SDLK_UP:
                            m_renderer->GetCamera(0)->MoveForward(cameraSpeed);
                            break;
                        case SDLK_DOWN:
                            m_renderer->GetCamera(0)->MoveBackward(cameraSpeed);
                            break;
                        case SDLK_RSHIFT:
                            m_renderer->GetCamera(0)->MoveUp(cameraSpeed);
                            break;
                        case SDLK_RCTRL:
                        case SDLK_LCTRL:
                            m_renderer->GetCamera(0)->MoveDown(cameraSpeed);
                            break;
                        case SDLK_w:                  
                            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                            break;
                        case SDLK_e:                  
                            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                            break;
                        case SDLK_SPACE:
                            rotating = !rotating;
                            break;
                        case SDLK_f:
                            followCamera = !followCamera;
                            break;
                        case SDLK_i:
                            m_renderer->GetCamera(1)->MoveForward(cameraSpeed);
                            break;
                        case SDLK_j:
                            m_renderer->GetCamera(1)->MoveLeft(cameraSpeed);
                            break;
                        case SDLK_k:
                            m_renderer->GetCamera(1)->MoveBackward(cameraSpeed);
                            break;
                        case SDLK_l:
                            m_renderer->GetCamera(1)->MoveRight(cameraSpeed);
                            break;
                        case SDLK_u:
                            m_renderer->GetCamera(1)->MoveUp(cameraSpeed);
                            break;
                        case SDLK_o:
                            m_renderer->GetCamera(1)->MoveDown(cameraSpeed);
                            break;
                        // case SDLK_MINUS:
                        //     reducingObj->ContractVertexPair();
                        //     break;
                        // case SDLK_EQUALS:
                        //     reducingObj->SplitVertex();
                        //     break;
                        // case SDLK_9:
                        //     reducingObj->DecimateByRatio(0.1f);
                        //     break;
                        // case SDLK_8:
                        //     reducingObj->DecimateByRatio(0.2f);
                        //     break;
                        // case SDLK_7:
                        //     reducingObj->DecimateByRatio(0.3f);
                        //     break;
                        // case SDLK_6:
                        //     reducingObj->DecimateByRatio(0.4f);
                        //     break;
                        // case SDLK_5:
                        //     reducingObj->DecimateByRatio(0.5f);
                        //     break;
                        // case SDLK_4:
                        //     reducingObj->DecimateByRatio(0.6f);
                        //     break;
                        // case SDLK_3:
                        //     reducingObj->DecimateByRatio(0.7f);
                        //     break;
                        // case SDLK_2:
                        //     reducingObj->DecimateByRatio(0.8f);
                        //     break;
                        // case SDLK_1:
                        //     reducingObj->DecimateByRatio(0.9f);
                        //     break;
                        // case SDLK_0:
                        //     reducingObj->DecimateByRatio(0.01f);
                        //     break;
                        // case SDLK_k:
                        //     if(followCamera) break;
                        //     distanceFromViewFrustum = std::clamp(distanceFromViewFrustum+cameraSpeed, 0.0f, 50.0f);
                        //     reducingObj->SetDistanceFromView(distanceFromViewFrustum);
                        //     break;
                        // case SDLK_l:
                        //     if(followCamera) break;
                        //     distanceFromViewFrustum = std::clamp(distanceFromViewFrustum-cameraSpeed, 0.0f, 50.0f);
                        //     reducingObj->SetDistanceFromView(distanceFromViewFrustum);
                        //     break;
                        case SDLK_r:
                            reducingObj->Reset();
                            break;
                    }
                    if(followCamera) {
                        Camera* mainCamera = m_renderer->GetCamera(0);
                        Camera* unhookedCamera = m_renderer->GetCamera(1);
                        unhookedCamera->SetCameraEyePosition(mainCamera->GetEyeXPosition(),
                                mainCamera->GetEyeYPosition(), mainCamera->GetEyeZPosition());
                    }
                break;
            }
        } 
        if(rotating){
            reduceObjNode->GetLocalTransform().Rotate(0.01f, 0, 1, 0);
            // refObjNode->GetLocalTransform().Rotate(0.01f, 0, 1, 0);
        }
        // Update our scene through our renderer
        m_renderer->Update();
        // Render our scene using our selected renderer
        m_renderer->Render();
        // Delay to slow things down just a bit!
        // SDL_Delay(25);  // TODO: You can change this or implement a frame
                        // independent movement method if you like.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
	}
    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
