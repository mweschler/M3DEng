#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <GL/glew.h>

namespace M3D{
	class Material{
	public:
		Material(void);
		~Material(void);
		void setProgram(const GLuint program);
		GLuint getProgram(void);
	private:
		GLuint program;
	};

}

#endif