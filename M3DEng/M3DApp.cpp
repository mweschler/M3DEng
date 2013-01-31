#include <GL\glew.h>
#include <GL\glfw.h>
#include <stdlib.h>
#include <stdio.h>

#include "M3DApp.h"

namespace M3D{
	M3DApp::M3DApp(){
		initialized = false;
	}

	M3DApp::~M3DApp(){
	
	}

	bool M3DApp::initialize(){
		if(initialized){
			printf("Application already initilized!\n");
			return false;
		}

		if(glfwInit() == GL_FALSE){
			printf("Could not initilize GLFW!\n");
			return false;
		}

		initialized = true;
		return true;
	}

	int M3DApp::run(){
		if(!initialized){
			printf("Application has not ben initialized first!\n");
			return EXIT_FAILURE;
		}

		//do stuff

		shutdown();
		return EXIT_SUCCESS;
	}

	void M3DApp::shutdown(){
		if(initialized){
			glfwTerminate();

			initialized = false;
		}
	}
}