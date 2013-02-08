#include <stdexcept>
#include <iostream>
#include "SceneManager.h"

namespace M3D{
	SceneManager::SceneManager(void){
		mainCamera = NULL;
		initialized = false;
		this->ambientLightIntensity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		this->globalLightDir = glm::vec3(0.0f);
		this->globalLightIntensity = glm::vec4(0.0f);
	}


	SceneManager::~SceneManager(void)
	{
		if(initialized)
			shutdown();
	}

	bool SceneManager::initialize(RenderSystem* renderSystem){
		if(initialized){
			throw new std::runtime_error("Cannot initialize SceneManager, already initialized.");
		}

		if(renderSystem == NULL)
			throw new std::runtime_error("RenderSystem cannot be null");

		std::cout<<"DEBUG: SCENEMGR INIT START"<<std::endl;

		this->renderSystem = renderSystem;
		std::cout<<"DEBUG: RS SET"<<std::endl;
		mainCamera = new Camera();
		initialized = true;
		
		return true;
	}

	void SceneManager::shutdown(void){
		if(mainCamera != NULL){
			delete mainCamera;
			mainCamera = NULL;
		}		
		entities.clear();

		initialized = false;
	}

	void SceneManager::renderScene(){
		for(std::vector<Entity*>::iterator it = entities.begin();
			it != entities.end(); ++it){
			if((*it)->isVisible())
				renderSystem->renderEntity(*it, this->mainCamera, this->globalLightDir, 
											this->globalLightIntensity, this->ambientLightIntensity);
		}
	}

	void SceneManager::setMainCamera(Camera* camera){
		if(camera == NULL)
			throw std::runtime_error("Camera cannot be a null value");

		this->mainCamera = camera;
	}

	Camera* SceneManager::getMainCamera(void){
		return this->mainCamera;
	}

	void SceneManager::addEntity(Entity* entity){
		entities.push_back(entity);
	}

	void SceneManager::setAmbientLightIntensity(const glm::vec4 intensity){
		this->ambientLightIntensity = glm::vec4(intensity);
	}

	void SceneManager::setGlobalLightDir(const glm::vec3 dir){
		this->globalLightDir = glm::vec3(dir);
	}

	void SceneManager::setGlobalLightIntensity(const glm::vec4 intensity){
		this->globalLightIntensity = glm::vec4(intensity);
	}

	glm::vec3 SceneManager::getGlobalLightDir(void) const{
		return glm::vec3(this->globalLightDir);
	}

	glm::vec4 SceneManager::getGlobalLightIntensity(void) const{
		return glm::vec4(this->globalLightIntensity);
	}

	glm::vec4 SceneManager::getAambientLightIntensity(void) const{
		return glm::vec4(this->ambientLightIntensity);
	}
}
