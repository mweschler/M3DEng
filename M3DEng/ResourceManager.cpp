#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
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
		int fileLength = (int)file.tellg();

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
		for(unsigned i = 0; i < shaders.size(); ++i){
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

		return program;
	}


	Mesh* ResourceManager::loadObjFile(const std::string filename){
		std::ifstream file;
		file.open(filename, std::ios::in);

		if(!file.is_open()){
			std::cout<<"Could not open OBJ file "<<filename<<std::endl;
			return NULL;
		}
		Mesh* mesh = new Mesh();
		std::vector<glm::vec4> *verticies = mesh->getVerticies();
		std::vector<GLushort> *elements = mesh->getElements();

		//parse file
		std::string line;
		while(std::getline(file, line)){
			if(line.substr(0,2) == "v "){
				//found a verticies
				std::istringstream stream(line.substr(2));
				glm::vec4 vertex;
				stream >> vertex.x;
				stream >> vertex.y;
				stream >> vertex.z;
				vertex.w = 1.0f;

				verticies->push_back(vertex);
			} else if (line.substr(0,2) == "f "){
				//found a face
				std::istringstream stream(line.substr(2));
				GLshort vert1, vert2, vert3;
				stream >> vert1;
				stream >> vert2;
				stream >> vert3;
				std::cout<<"VERT INDICIES: "<<vert1<<" "<<vert2<<" "<<vert3<<std::endl;
				//offset indicies to start at 0, not 1
				vert1--;
				vert2--;
				vert3--;
				
				elements->push_back(vert1);
				elements->push_back(vert2);
				elements->push_back(vert3);
			} else { 
				//Only verticies and faces will be processed
			}
		}

		file.close();

		mesh->calculateNormals();

		mesh->setupBuffers();
		return mesh;
	}
}