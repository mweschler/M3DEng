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
		void setGlobalLightDir(const glm::vec3 dir);
		void setGlobalLightIntensity(const glm::vec4 intensity);
		void setAmbientLightIntensity(const glm::vec4 intensity);
		glm::vec3 getGlobalLightDir(void) const;
		glm::vec4 getGlobalLightIntensity(void) const;
		glm::vec4 getAambientLightIntensity(void) const;
	private:
		bool initialized;
		std::vector<Entity*> entities;
		Camera *mainCamera;
		RenderSystem* renderSystem;
		glm::vec3 globalLightDir;
		glm::vec4 globalLightIntensity;
		glm::vec4 ambientLightIntensity;
	};

}

#endif

