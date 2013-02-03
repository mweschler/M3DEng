#include <GL\glew.h>
#include <GL\glfw.h>
#include <stdlib.h>
#include <iostream>

#include "M3DApp.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "RenderSystem.h"

namespace M3D{
	M3DApp::M3DApp(){
		initialized = false;
		running = false;
	}

	M3DApp::~M3DApp(){
	
	}

	bool M3DApp::initialize(int width, int height){
		using std::cout;

		if(initialized){
			cout<<"Application already initilized!\n";
			return false;
		}

		cout<<"Starting initialization.\n";

		if(glfwInit() == GL_FALSE){
			cout<<"Could not initialize GLFW!\n";
			return false;
		}

		cout<<"GLFW has been initialized.\n";

		if(glfwOpenWindow(width, height, 0, 0, 0 ,0 ,0 ,0 ,GLFW_WINDOW) == GL_FALSE){
			cout<<"Could not create window!\n";
			glfwTerminate();
			return false;
		}

		cout<<"Window opened.\n";

		if(glewInit() != GLEW_OK){
			cout<<"Could not initialize GLEW!\n";
			glfwTerminate();
			return false;
		}

		cout<<"GLEW has been initialized\n";

		if(!GLEW_VERSION_2_1){
			cout<<"OpenGL version 2.0 must be supported!\n";
			glfwTerminate();
			return false;
		}

		cout<<"OpenGL 2.0 is supported\n";

		if(!resourceManager.initialize()){
			cout<<"ResourceManager initialization failed!\n";
			glfwTerminate();
			return false;
		}

		cout<<"ResourceManager initialized.\n";

		if(!renderSystem.initialize()){
			cout<<"RenderSystem initialization failed!\n";
			glfwTerminate();
			return false;
		}

		cout<<"RenderSystem initialized.\n";

		if(!sceneManager.initialize(&renderSystem)){
			cout<<"SceneManager initialization failed\n";
			glfwTerminate();
			return false;
		}

		cout<<"SceneManager initialized.\n";

		cout<<"Initialization complete!\n";
		initialized = true;
		return true;
	}

	int M3DApp::run(){
		if(!initialized){
			std::cout<<"Application has not been initialized first!\n";
			return EXIT_FAILURE;
		}

		std::cout<<"Application starting.\n";

		running = true;
		//do stuff
		while(running){
			glClear( GL_COLOR_BUFFER_BIT );
			// Swap front and back rendering buffers
			glfwSwapBuffers();


			if(glfwGetKey( GLFW_KEY_ESC) || !glfwGetWindowParam( GLFW_OPENED )){
				shutdown();
			}
		}

		return EXIT_SUCCESS;
	}

	void M3DApp::shutdown(){
		std::cout<<"Shutting down application.\n";
		if(initialized){
			glfwTerminate();

			initialized = false;
		}

		running = false;
	}
}