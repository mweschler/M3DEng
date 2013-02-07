#include <stdexcept>
#include <iostream>
#include "SceneManager.h"

namespace M3D{
	SceneManager::SceneManager(void){
		mainCamera = NULL;
		initialized = false;
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
				renderSystem->renderEntity(*it, this->mainCamera);
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
}
