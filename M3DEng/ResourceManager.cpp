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
		int fileLength = file.tellg();

		char* data = new char[fileLength + 1];
		memset(data, 0, sizeof(char) * fileLength + 1);

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

	void parseStringPart(std::string data,GLshort &vertexIndex, GLshort &normalIndex){
		//find vertex index section
		int position = data.find("/");
		if(position == std::string::npos){
			std::istringstream vertstream(data);
			vertstream >> vertexIndex;
			normalIndex = 0;
			return;
		} else {
			std::string sub = data.substr(0, position);
			std::istringstream vertstream(sub);
			int vert;
			vertstream >> vert;
			vertexIndex = vert;
			position = data.find("/", position);
			//ignore texture index

			//rest is normal index;
			position = data.find("/", position + 1);
			sub = data.substr(position + 1);
			std::istringstream normstream(data.substr(position + 1));
			int norm;
			normstream >> norm;
			normalIndex = norm;
		}
	}

	Mesh* ResourceManager::loadObjFile(const std::string filename){
		std::ifstream file;
		file.open(filename, std::ifstream::in);

		if(!file.is_open()){
			std::cout<<"Could not open OBJ file "<<filename<<std::endl;
			return NULL;
		}
		Mesh* mesh = new Mesh();
		std::vector<glm::vec4> *verticies = mesh->getVerticies();
		std::vector<GLushort> *elements = mesh->getElements();
		std::vector<glm::vec3> *normals = mesh->getNormals();
		std::vector<glm::vec3> normalTemp;
		std::vector<GLushort> normalIndexs;
		bool normalsIncluded = false;

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
			} else if(line.substr(0,3) == "vn "){
				normalsIncluded = true;
				std::istringstream stream(line.substr(3));
				glm::vec3 normal;
				stream >> normal.x;
				stream >> normal.y;
				stream >> normal.z;
				normal = glm::normalize(normal); //obj does not gurantee unit length
				normalTemp.push_back(normal);
			} else if (line.substr(0,2) == "f "){
				//found a face
				
				GLshort vert1, vert2, vert3, norm1, norm2, norm3;
				int start = 2; //line.find(" ", 3);
				while(start < line.size()){
					if(line[start] == ' ') {
						start++;
					} else
						break;
				}
				int position = line.find(" ", start);
				std::string sub = line.substr(start, position - start);
				parseStringPart(sub, vert1, norm1);
				int nextSpace = line.find(" ", position + 1);
				
				parseStringPart(line.substr(position + 1, nextSpace - position - 1), vert2, norm2);
				parseStringPart(line.substr(nextSpace + 1), vert3, norm3);

				//offset indicies to start at 0, not 1
				vert1--;
				vert2--;
				vert3--;
				norm1--;
				norm2--;
				norm3--;
				if(norm1 >0)
					normalsIncluded = true;
				
				elements->push_back(vert1);
				elements->push_back(vert2);
				elements->push_back(vert3);
				normalIndexs.push_back(norm1);
				normalIndexs.push_back(norm2);
				normalIndexs.push_back(norm3);
			} else { 
				//Only verticies and faces will be processed
			}
		}

		file.close();
		if(!normalsIncluded)
			mesh->calculateNormals();
		else{
			std::vector<glm::vec3> *normals = mesh->getNormals();
			normals->resize(verticies->size(), glm::vec3(0.0f, 0.0f, 0.0f));

			for(int i = 0; i < elements->size(); ++i){
				glm::vec3 normal = normalTemp[normalIndexs[i]];
				glm::vec4 vert = (*verticies)[(*elements)[i]];
				if((*normals)[(*elements)[i]] == glm::vec3(0.0f)){
					(*normals)[(*elements)[i]] = normal;
				}else if((*normals)[(*elements)[i]] != normal){
					verticies->push_back(vert);
					normals->push_back(normal);
					(*elements)[i] = verticies->size() - 1;
				}
			}
		}

		mesh->setupBuffers();
		return mesh;
	}
}