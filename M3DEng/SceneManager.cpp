#include <stdexcept>
#include "SceneManager.h"

namespace M3D{
	SceneManager::SceneManager(void){
		mainCamera = NULL;
	}


	SceneManager::~SceneManager(void)
	{
		if(initialized)
			shutdown();
	}

	bool SceneManager::initialize(RenderSystem const* renderSystem){
		if(initialized){
			throw new std::runtime_error("Cannot initialize SceneManager, already initialized.");
		}

		if(renderSystem == NULL)
			throw new std::runtime_error("RenderSystem cannot be null");

		this->renderSystem = renderSystem;
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
		for(std::vector<Entity>::iterator it = entities.begin();
			it != entities.end(); ++it){
				renderSystem->renderEntity(&(*it));
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
}
