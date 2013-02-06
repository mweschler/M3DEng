#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <glm/glm.hpp>
#include "Material.h"
#include "Mesh.h"

namespace M3D{
	class Entity {
	public:
		Entity(void);
		~Entity(void);
		void setPosition(float x, float y, float z);
		void setPostion(const glm::vec3 newPos);
		void setRotation(const glm::vec3 newRot);
		void setScale(const glm::vec3 newScale);
		void setPosX(float x);
		void setPosY(float y);
		void setPosZ(float z);
		void setMaterial(Material* mat);
		void setMesh(Mesh* mesh);
		void setVisible(bool visible);
		glm::vec3 getPosition(void);
		glm::vec3 getRotation(void);
		glm::vec3 getScale(void);
		Material* getMaterial(void);
		glm::mat4 getModelViewMatrix(void) const;
		Mesh* getMesh();
	private:
		void updateModelMatrix(void);
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 modelViewMatrix;
		Material* material;
		Mesh* mesh;
		bool visible;
	};
}

#endif