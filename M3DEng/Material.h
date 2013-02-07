#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace M3D{
	class Material{
	public:
		Material(void);
		~Material(void);
		void setProgram(const GLuint program);
		GLuint getProgram(void);
		glm::vec4 getDiffuseColor(void) const;
		void setDiffuseColor(const glm::vec4 color);
	private:
		GLuint program;
		glm::vec4 diffuseColor;
	};

}

#endif