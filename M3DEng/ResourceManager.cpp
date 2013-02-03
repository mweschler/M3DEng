#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>

#include "ResourceManager.h"

namespace M3D{
	ResourceManager::ResourceManager(void){
	
	}

	ResourceManager::~ResourceManager(void){
	
	}

	bool ResourceManager::initialize(void){
	
		return true;
	}

	char *ResourceManager::loadFile(const std::string filename){
		if (filename.empty()){
			throw std::runtime_error("Could not load empty filename");
		}

		std::ifstream file;
		file.open(filename, std::ios::in);
		if(!file.is_open()){
			std::cout<<"Could not load from file "<<filename<<std::endl;
			return NULL;
		}

		//get size of file
		file.seekg(0, std::ios::end);
		int fileLength = file.tellg();

		char* data = new char[fileLength];
		memset(data, 0, sizeof(char) * fileLength);

		file.seekg(0, std::ios::beg);
		file.read(data, fileLength);

		file.close();
		return data;
	}

	GLuint ResourceManager::loadShader(const GLenum type, const std::string filename){
		//load the source from file
		const char* data = loadFile(filename);

		if(data == NULL){
			std::cout<<"Could not get shader filedata from "<<filename<<std::endl;
			return RM_SHADER_FAILURE;
		}

		//compile the shader from source
		GLuint shader = glCreateShader(type);
		GLint sourceLength = strlen(data);
		glShaderSource(shader, 1, &data, &sourceLength);
		glCompileShader(shader);

		delete[] data;

		//check to see if there was an compile errors
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if(status == GL_FALSE){
			//Shader failed to compile, get the log
			GLint logLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			GLchar* log = new GLchar[logLength + 1];
			glGetShaderInfoLog(shader, logLength, NULL, log);

			std::string shaderType;
			switch(type){
			case GL_VERTEX_SHADER: shaderType = "vertex";break;
			case GL_FRAGMENT_SHADER: shaderType = "fragment"; break;
			}

			std::cout<<"Failed to compile "<<shaderType<<" shader from "<<filename<<":\n"<<log<<std::endl;
			delete[] log;

			return RM_SHADER_FAILURE;
		}

		return shader;
	}

	GLuint ResourceManager::createProgram(const std::vector<GLuint> shaders){
		if(shaders.empty()){
			throw std::runtime_error("Empty shader list in createProgram");
		}

		GLuint program = glCreateProgram();

		//add shader to the program
		for(int i = 0; i < shaders.size(); ++i){
			glAttachShader(program, shaders[i]);
		}

		//link
		glLinkProgram(program);

		//check to see if program linked correctly
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if(status == GL_FALSE){
			GLint logLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

			GLchar *log = new GLchar[logLength + 1];
			glGetProgramInfoLog(program,  logLength, NULL, log);

			std::cout<<"Failed to link program:\n"<<log<<std::endl;
			delete[] log;

			return RM_SHADER_FAILURE;
		}

	}
}