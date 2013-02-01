#include <GL\glew.h>
#include <GL\glfw.h>
#include <stdlib.h>
#include <stdio.h>

#include "M3DApp.h"
#include "ResourceManager.h"

namespace M3D{
	M3DApp::M3DApp(){
		initialized = false;
		running = false;
	}

	M3DApp::~M3DApp(){
	
	}

	bool M3DApp::initialize(int width, int height){
		if(initialized){
			printf("Application already initilized!\n");
			return false;
		}

		printf("Starting initialization.\n");

		if(glfwInit() == GL_FALSE){
			printf("Could not initialize GLFW!\n");
			return false;
		}

		printf("GLFW has been initialized.\n");

		if(glfwOpenWindow(width, height, 0, 0, 0 ,0 ,0 ,0 ,GLFW_WINDOW) == GL_FALSE){
			printf("Could not create window!\n");
			glfwTerminate();
			return false;
		}

		printf("Window opened.\n");

		if(glewInit() != GLEW_OK){
			printf("Could not initialize GLEW!\n");
			glfwTerminate();
			return false;
		}

		printf("GLEW has been initialized\n");

		if(!GLEW_VERSION_2_0){
			printf("OpenGL version 2.0 must be supported!\n");
			glfwTerminate();
			return false;
		}

		printf("OpenGL 2.0 is supported\n");

		if(!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)){
			printf("Shader support not found.\n");
			glfwTerminate();
			return false;
		}

		printf("Vertex and Fragment shaders are supported.\n");

		if(!resourceManager.initialize()){
			printf("ResourceManager initialization failed!\n");
			glfwTerminate();
			return false;
		}

		printf("ResourceManager initialized.\n");

		printf("Initialization complete!\n");
		initialized = true;
		return true;
	}

	int M3DApp::run(){
		if(!initialized){
			printf("Application has not been initialized first!\n");
			return EXIT_FAILURE;
		}

		printf("Application starting.\n");

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
		printf("Shutting down application.\n");
		if(initialized){
			glfwTerminate();

			initialized = false;
		}

		running = false;
	}
}