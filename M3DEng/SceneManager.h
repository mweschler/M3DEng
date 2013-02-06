#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include <vector>
#include "Entity.h"
#include "Camera.h"
#include "RenderSystem.h"

namespace M3D{
	class SceneManager
	{
	public:
		SceneManager(void);
		~SceneManager(void);
		bool initialize(RenderSystem* renderSystem);
		void shutdown(void);
		void renderScene(void);
		void setMainCamera(Camera *camera);
		Camera* getMainCamera(void);
		void addEntity(Entity* entity);
	private:
		bool initialized;
		std::vector<Entity*> entities;
		Camera *mainCamera;
		RenderSystem* renderSystem;
	};

}

#endif

