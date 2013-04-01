#include <stdlib.h>
#include <iostream>
#include "M3DApp.h"
#include "hw2.h"

int main(int argc, char *argv[]){
	
	/*M3D::M3DApp app;
	if(!app.initialize(1024, 768)){
		std::cout<<"Could not initilize, shutting down!\n";
		return EXIT_FAILURE;
	}

	return app.run();*/

	HW2::HW2Driver driver;
	driver.loadMesh("torus.obj");
	int euler = driver.calcEulerFromMesh();
	std::cout<<"The euler number found using v+f-e is "<<euler<<std::endl;
	
	euler = driver.calcEulerFromGenus(1);
	std::cout<<"The euler number found using 2-2g is "<<euler<<std::endl;
	
	double curve = driver.calcGausCurveFromMesh();
	std::cout<<"The Gaussian curvature using left side of Gauss-Bonnet theorem is "<<curve<<std::endl;
	
	curve = driver.calcGausCurveFromEuler(euler);
	std::cout<<"The Gaussian curvate using the right side of Gauss-Bonnet theorem is "<<curve<<std::endl;

	std::cout<<std::endl<<"Press any enter to exit";
	std::cin.get();
}