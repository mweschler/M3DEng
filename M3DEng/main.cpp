#include <stdlib.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include "M3DApp.h"
#include "hw2.h"

std::string openFileDialog(){
	OPENFILENAME openFileName;
	char filename[MAX_PATH] = "";
	ZeroMemory(&openFileName, sizeof(openFileName));

	openFileName.lStructSize = sizeof(openFileName);
	openFileName.hwndOwner = NULL;
	openFileName.lpstrFilter = "Waveform Mesh(.OBJ)\0*.obj\0\0";
	openFileName.lpstrFile = filename;
	openFileName.nMaxFile = MAX_PATH;
	openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	openFileName.lpstrDefExt = "obj";

	if( GetOpenFileName(&openFileName))
		return std::string(filename);

	return std::string("");
}

int main(int argc, char *argv[]){
	
	/*M3D::M3DApp app;
	if(!app.initialize(1024, 768)){
		std::cout<<"Could not initilize, shutting down!\n";
		return EXIT_FAILURE;
	}

	return app.run();*/

	std::string filename = openFileDialog();

	HW2::HW2Driver driver;
	driver.loadMesh(filename);
	int euler = driver.calcEulerFromMesh();
	std::cout<<"The euler number found using v+f-e is "<<euler<<std::endl;
	
	euler = driver.calcEulerFromGenus(2);
	std::cout<<"The euler number found using 2-2g is "<<euler<<std::endl;
	
	double curve = driver.calcGausCurveFromMesh();
	std::cout<<"The Gaussian curvature using left side of Gauss-Bonnet theorem is "<<curve<<std::endl;
	
	curve = driver.calcGausCurveFromEuler(euler);
	std::cout<<"The Gaussian curvate using the right side of Gauss-Bonnet theorem is "<<curve<<std::endl;

	std::cout<<std::endl<<"Press any enter to exit";
	std::cin.get();
}