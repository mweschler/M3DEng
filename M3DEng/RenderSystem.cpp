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
		/*glm::mat4 model = glm::mat4(1.0);
			
		model *= glm::lookAt( glm::vec3(2.0f,2.0f, -10.0f), 
		                 glm::vec3(0.0f,0.0f,-1.0f), 
                         glm::vec3(0.0f,1.0f, 0.0f));*/
		perspective = perspective;
		//std::cout<<"MVP: "<<glm::value_ptr(perspective)[0]<<" "<<glm::value_ptr(perspective)[1]<<" "<<glm::value_ptr(perspective)[2];
		glViewport(0, 0, screenWidth, screenHeight);

		//gluPerspective(45,ratio, 1, 1000);

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
		if(projLoc == -1){
			//std::cout<<"Invlalid uniform location!"<<std::endl;
		}
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(modelToClip));

		//set modelToWorld for normals
		glm::mat3 modelToCamera = glm::mat3(worldToCamera * modelToWorld);
		
		//if there is a non uniform scale do an inverse transpose for correct lighting normals
		glm::vec3 scale = entity->getScale();
		if(scale.x != scale.y || scale.x != scale.z)
			modelToCamera = glm::transpose(glm::inverse(modelToCamera));

		//GLint MTCLoc = glGetUniformLocation(material->getProgram(), "modelToCamera");
		//glUniformMatrix3fv(MTCLoc, 1, GL_FALSE, glm::value_ptr(modelToCamera));

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
		glm::mat3 worldToModel = glm::mat3(glm::inverse(modelToWorld));
		glm::vec3 lightDir = worldToModel * globalLightDir;
		lightDir = glm::normalize(lightDir);

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