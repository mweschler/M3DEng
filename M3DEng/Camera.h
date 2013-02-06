#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Entity.h"

namespace M3D{
	class Camera: public Entity{
	public:
		Camera(void);
		~Camera(void);
		glm::vec3 getTarget(void) const;
		void setTarget(const glm::vec3 newTarget);
	private:
		glm::vec3 target;
	};
}

#endif