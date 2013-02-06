#include "Camera.h"

namespace M3D{
	Camera::Camera(void){
		target = glm::vec3(0.0f, 0.0f, 10.0f);
	}

	Camera::~Camera(void){
		
	}

	glm::vec3 Camera::getTarget(void) const{
		return glm::vec3(this->target);
	}

	void Camera::setTarget(const glm::vec3 newTarget){
		this->target = glm::vec3(newTarget);
	}
}