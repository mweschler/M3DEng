#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <glm\glm.hpp>
#include <GL/glew.h>

namespace M3D{
	class Mesh{
	public:
		Mesh(void);
		~Mesh(void);
		std::vector<glm::vec4>* getVerticies(void);
		std::vector<glm::vec3>* getNormals(void);
		std::vector<GLushort>* getElements(void);
		void calculateNormals(void);
	private:
		bool normalsUpToDate;
		std::vector<glm::vec4> verticies;
		std::vector<glm::vec3> normals;
		std::vector<GLushort> elements;
	};
}

#endif