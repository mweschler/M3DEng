#include <stdlib.h>
#include <iostream>
#include "M3DApp.h"

int main(int argc, char *argv[]){
	
	M3D::M3DApp app;
	if(!app.initialize(800, 600)){
		std::cout<<"Could not initilize, shutting down!\n";
		return EXIT_FAILURE;
	}

	return app.run();
}