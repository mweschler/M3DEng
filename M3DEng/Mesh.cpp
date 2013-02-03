#include "Mesh.h"

namespace M3D{
	Mesh::Mesh(void){
		normalsUpToDate = false;
	}

	Mesh::~Mesh(void){
	
	}
	
	void Mesh::calculateNormals(void)
	{
		//initialize the normals withe one for every vertex
		normals.resize(verticies.size(), glm::vec3(0.0, 0.0, 0.0));

		//iterate through each face and calculate normals
		for(int i = 0; i < elements.size(); i+=3){
			glm::vec3 vert1 = glm::vec3(verticies[elements[i]]);
			glm::vec3 vert2 = glm::vec3(verticies[elements[i + 1]]);
			glm::vec3 vert3 = glm::vec3(verticies[elements[i + 2]]);

			//calculate normal for this face
			glm::vec3 normal = glm::normalize(glm::cross(vert3 - vert1, vert2 - vert1));

			//apply normal for each vert index;
			normals[elements[i]] = normal;
			normals[elements[i + 1]] = normal;
			normals[elements[i + 2]] = normal;
		}
	}

	std::vector<glm::vec4>* Mesh::getVerticies(void){
		return &verticies;
	}

	std::vector<glm::vec3>* Mesh::getNormals(void){
		return &normals;
	}

	std::vector<GLushort>*  Mesh::getElements(void){
		return &elements;
	}
}

