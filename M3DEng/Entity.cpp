#include "Entity.h"

namespace M3D{
	Entity::Entity(void){
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
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

	void Entity::setMaterial(Material const* mat){
		this->material = mat;
	}

	void Entity::setMesh(Mesh const* mesh){
		this->mesh = mesh;
	}
}