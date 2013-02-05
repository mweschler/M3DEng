#include "RenderSystem.h"

namespace M3D{
	RenderSystem::RenderSystem(void){
	}

	RenderSystem::~RenderSystem(void){
	
	}

	bool RenderSystem::initialize(int screenWidth, int screenHeight){
		float ratio = 1.0 * screenWidth / screenHeight;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glViewport(0, 0, screenWidth, screenHeight);

		//gluPerspective(45,ratio, 1, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
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
		
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertsVBO());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIBO());
		int size;
		//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		size = (mesh->getElements())->size();
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}
}