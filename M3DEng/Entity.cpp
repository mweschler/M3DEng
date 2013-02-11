#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"

namespace M3D{
	Entity::Entity(void){
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
		this->visible = false;
		this->mesh = NULL;
		this->material = NULL;

		updateModelMatrix();

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
		updateModelMatrix();
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
		glm::vec3 offset;
		if(mesh != NULL){
			offset = mesh->getOriginOffset();
		}else{
			offset = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		
		glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), this->rotation.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotateY = glm::rotate(rotateX, this->rotation.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotateZ = glm::rotate(rotateY, this->rotation.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 model = glm::scale(rotateZ, this->scale);

		glm::mat4 offTrans = glm::translate(glm::mat4(1.0f), offset);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), position);

		this->modelViewMatrix = view * model *offTrans;

	}

	bool Entity::isVisible(void) const{
		return this->visible;
	}
}