#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_

#include "Entity.h"

namespace M3D{
	class RenderSystem{
	public:
		RenderSystem(void);
		~RenderSystem(void);
		bool initialize(void);
		void shutdown(void);
		void renderEntity(Entity* entity);
	private:
		bool initialized;
	};

}

#endif