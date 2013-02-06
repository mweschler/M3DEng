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
		void setPosX(float x);
		void setPosY(float y);
		void setPosZ(float z);
		void setMaterial(Material* mat);
		void setMesh(Mesh* mesh);
		void setVisible(bool visible);
		glm::vec3 getPosition(void);
		Material* getMaterial(void);
		Mesh* getMesh();
	private:
		glm::vec3 position;
		Material* material;
		Mesh* mesh;
		bool visible;
	};
}

#endif