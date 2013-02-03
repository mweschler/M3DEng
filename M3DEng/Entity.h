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
		void setMaterial(Material const* mat);
		void setMesh(Mesh const* mesh);
	private:
		glm::vec3 position;
		Material const* material;
		Mesh const* mesh;
	};
}

#endif