#include "Material.h"

namespace M3D{
	Material::Material(void){
		this->program = 0;
		this->diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->shininessFactor = 0;
	}

	Material::~Material(void){
	
	}

	void Material::setProgram(const GLuint program){
		this->program = program;
	}

	GLuint Material::getProgram(void) {
		return this->program;
	}

	glm::vec4 Material::getDiffuseColor(void) const{
		return glm::vec4(this->diffuseColor);
	}

	void Material::setDiffuseColor(const glm::vec4 color){
		this->diffuseColor = glm::vec4(color);
	}

	void Material::setShininessFactor(const float value){
		this->shininessFactor = value;
	}

	float Material::getShininessFactor(void) const{
		return this->shininessFactor;
	}

}