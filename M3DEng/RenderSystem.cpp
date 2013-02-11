#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "RenderSystem.h"

namespace M3D{
	RenderSystem::RenderSystem(void){
	}

	RenderSystem::~RenderSystem(void){
	
	}

	bool RenderSystem::initialize(int screenWidth, int screenHeight){
		glEnable(GL_DEPTH_TEST); // enable depth-testing
		glDepthMask(GL_TRUE); // turn back on
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		float ratio = 1.0 * screenWidth / screenHeight;
		perspective = glm::mat4(1.0);;
		perspective *= glm::perspective(45.0f , ratio, 0.1f, 100.0f);
		
		perspective = perspective;

		glViewport(0, 0, screenWidth, screenHeight);

		initialized = true;
		return true;
	}

	void RenderSystem::shutdown(void){
		initialized = false;
	}

	void RenderSystem::renderEntity(Entity* entity, Camera* camera, glm::vec3 globalLightDir, 
									glm::vec4 globalLightIntensity, glm::vec4 ambientLightIntensity){
		Material* material = entity->getMaterial();
		Mesh* mesh = entity->getMesh();
		glm::mat4 modelToWorld = entity->getModelViewMatrix();
		glm::mat4 worldToCamera = glm::lookAt( camera->getPosition(), camera->getTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 cameraToClip = this->perspective;
		glm::mat4 modelToClip = cameraToClip * worldToCamera * modelToWorld;
		
		glUseProgram(material->getProgram());

		//set modelToClip for verticies
		GLint projLoc = glGetUniformLocation(material->getProgram(), "modelToClip");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(modelToClip));

		//set modelToWorld for normals
		glm::mat3 modelToCamera = glm::mat3(worldToCamera * modelToWorld);
		
		//do an inverse transpose for correct lighting normals
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelToCamera)));
		GLint normMatLoc = glGetUniformLocation(material->getProgram(), "normMatrix");
		glUniformMatrix3fv(normMatLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		GLint MTCLoc = glGetUniformLocation(material->getProgram(), "modelToCamera");
		glUniformMatrix3fv(MTCLoc, 1, GL_FALSE, glm::value_ptr(modelToCamera));

		//set diffuseColor
		glm::vec4 diffuseColor = material->getDiffuseColor();
		GLint diffLoc = glGetUniformLocation(material->getProgram(), "diffuseColor");
		glUniform4fv(diffLoc, 1, glm::value_ptr(diffuseColor));
		
		//set position for vertex data
		GLint posLoc = glGetAttribLocation(material->getProgram(), "position");
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertsVBO());
		glEnableVertexAttribArray(posLoc);
		glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);

		//set normal for modelspace normals
		GLint normLoc = glGetAttribLocation(material->getProgram(), "normal");
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalsVBO());
		glEnableVertexAttribArray(normLoc);
		glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//get light dir and prepare it for uniform
		glm::vec3 lightDir = globalLightDir;
		lightDir = glm::normalize(glm::mat3(worldToCamera) * lightDir);

		//set global light direction uniform
		GLint lightDirLoc = glGetUniformLocation(material->getProgram(), "lightDirection");
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

		//set global light inensity
		glm::vec4 intensity = glm::vec4(globalLightIntensity);
		GLint lightIntensityLoc = glGetUniformLocation(material->getProgram(), "lightIntensity");
		glUniform4fv(lightIntensityLoc, 1, glm::value_ptr(intensity));

		//set ambient light intensity
		glm::vec4 ambient = glm::vec4(ambientLightIntensity);
		GLint ambientLoc = glGetUniformLocation(material->getProgram(), "ambientIntensity");
		glUniform4fv(ambientLoc, 1, glm::value_ptr(ambient));

		float shininess = material->getShininessFactor();
		GLint shineLoc = glGetUniformLocation(material->getProgram(), "shininessFactor");
		glUniform1f(shineLoc, shininess);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIBO());

		int size;

		size = (mesh->getElements())->size();
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}
}