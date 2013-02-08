#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_

#include "Entity.h"
#include "Camera.h"

namespace M3D{
	class RenderSystem{
	public:
		RenderSystem(void);
		~RenderSystem(void);
		bool initialize(int screenWidth, int screenHeight);
		void shutdown(void);
		void renderEntity(Entity* entity, Camera* camera, glm::vec3 globalLightDir, 
							glm::vec4 globalLightIntensity, glm::vec4 ambientLightIntensity);
	private:
		bool initialized;
		glm::mat4 perspective;
	};

}

#endif