#include "SceneManager.h"

namespace M3D{
	SceneManager::SceneManager(void)
	{
	}


	SceneManager::~SceneManager(void)
	{
	}

	bool SceneManager::initialize(void){
		initalized = true;

		return true;
	}

	void SceneManager::shutdown(void){
		initalized = false;
	}

}
