#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <glm\glm.hpp>
#include <GL/glew.h>

namespace M3D{
	class Mesh{
	public:
		Mesh();
		~Mesh();
		std::vector<glm::vec4>* getVerticies(void);
		std::vector<glm::vec3>* getNormals(void);
		std::vector<GLushort>* getElements(void);
		void calculateNormals(void);
		void setupBuffers(void);
		void setOriginOffset(const glm::vec3 offset);
		const GLuint getVertsVBO(void);
		const GLuint getNormalsVBO(void);
		const GLuint getIBO(void);
		glm::vec3 getOriginOffset(void) const;
	private:
		bool normalsUpToDate;
		std::vector<glm::vec4> verticies;
		std::vector<glm::vec3> normals;
		std::vector<GLushort> elements;
		GLuint vbo_verts;
		GLuint vbo_normals;
		GLuint ibo;
		glm::vec3 originOffset;
	};
}

#endif