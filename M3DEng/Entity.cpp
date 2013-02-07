#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"

namespace M3D{
	Entity::Entity(void){
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
		updateModelMatrix();
		this->visible = false;
	}

	Entity::~Entity(void){
	
	}

	void Entity::setPosition(float x, float y, float z){
		this->position = glm::vec3(x, y, z);
		updateModelMatrix();
	}

	void Entity::setPosition(const glm::vec3 newPos){
		this->position = glm::vec3(newPos);
		updateModelMatrix();
	}
	
	void Entity::setRotation(const glm::vec3 newRot){
		this->rotation = glm::vec3(newRot);
		updateModelMatrix();
	}

	void Entity::setScale(const glm::vec3 newScale){
		this->scale = glm::vec3(newScale);
		updateModelMatrix();
	}

	void Entity::setPosX(float x){
		this->position.x = x;
		updateModelMatrix();
	}

	void Entity::setPosY(float y){
		this->position.y = y;
		updateModelMatrix();
	}

	void Entity::setPosZ(float z){
		this->position.z = z;
		updateModelMatrix();
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

	glm::vec3 Entity::getRotation(void){
		return glm::vec3(this->rotation);
	}

	glm::vec3 Entity::getScale(void){
		return glm::vec3(this->scale);
	}

	glm::mat4 Entity::getModelViewMatrix(void) const{
		return glm::mat4(this->modelViewMatrix);
	}
	
	void Entity::updateModelMatrix(void){
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), this->position);
		glm::mat4 rotateX = glm::rotate(translation, this->rotation.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotateY = glm::rotate(rotateX, this->rotation.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 view = glm::rotate(rotateY, this->rotation.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 model = glm::scale(glm::mat4(1.0f), this->scale);

		this->modelViewMatrix = view * model;

	}

	bool Entity::isVisible(void) const{
		return this->visible;
	}
}