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
		bool initialize(RenderSystem const* renderSystem);
		void shutdown(void);
		void renderScene(void);
		void setMainCamera(Camera *camera);
		Camera* getMainCamera(void);
	private:
		bool initialized;
		std::vector<Entity> entities;
		Camera *mainCamera;
		RenderSystem const* renderSystem;
	};

}

#endif

