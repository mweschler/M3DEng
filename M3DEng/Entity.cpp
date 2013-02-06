#include "Entity.h"

namespace M3D{
	Entity::Entity(void){
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->visible = false;
	}

	Entity::~Entity(void){
	
	}

	void Entity::setPosition(float x, float y, float z){
		this->position = glm::vec3(x, y, z);
	}

	void Entity::setPostion(const glm::vec3 newPos){
		this->position = glm::vec3(newPos.x, newPos.y, newPos.z);
	}

	void Entity::setPosX(float x){
		this->position.x = x;
	}

	void Entity::setPosY(float y){
		this->position.y = y;
	}

	void Entity::setPosZ(float z){
		this->position.z = z;
	}

	void Entity::setMaterial(Material* mat){
		this->material = mat;
	}

	void Entity::setMesh(Mesh* mesh){
		this->mesh = mesh;
	}

	void Entity::setVisible(bool visible){
		this->visible = visible;
	}

	Material* Entity::getMaterial(void){
		return this->material;
	}

	Mesh* Entity::getMesh(void){
		return this->mesh;
	}

	glm::vec3 Entity::getPosition(void){
		return glm::vec3(this->position);
	}
}