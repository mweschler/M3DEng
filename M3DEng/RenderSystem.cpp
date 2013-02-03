#include "RenderSystem.h"

namespace M3D{
	RenderSystem::RenderSystem(void){
	
	}

	RenderSystem::~RenderSystem(void){
	
	}

	bool RenderSystem::initialize(void){
		initialized = true;
		return true;
	}

	void RenderSystem::shutdown(void){
		initialized = false;
	}

	void RenderSystem::renderEntity(Entity* entity)const{
	
	}
}