#include "Material.h"

namespace M3D{
	Material::Material(void){

	}

	Material::~Material(void){
	
	}

	void Material::setProgram(const GLuint program){
		this->program = program;
	}
}