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

	void RenderSystem::renderEntity(Entity* entity){
		Material* material = entity->getMaterial();
		Mesh* mesh = entity->getMesh();

		glUseProgram(material->getProgram());
		glBindVertexArray(mesh->getVAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIBO());

		int size;
		//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		size = (mesh->getElements())->size();
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
}