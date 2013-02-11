#include <stdexcept>

#include "Mesh.h"

namespace M3D{
	Mesh::Mesh(){
		normalsUpToDate = false;
		vbo_verts = 0;
		vbo_normals = 0;
		ibo = 0;
		this->originOffset = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	Mesh::~Mesh(){
	
	}

	void setNormal(std::vector<glm::vec3> &normals, std::vector<GLushort> elements, glm::vec3 normal, int eleIndex,
		std::vector<glm::vec4> &verticies){
		
		if(normals[elements[eleIndex]] == glm::vec3(0.0f)){
			normals[elements[eleIndex]] = normal;
		} else if(normals[elements[eleIndex]] != normal)
		{
			glm::vec4 vert = verticies[elements[eleIndex]];
			verticies.push_back(vert);
			normals.push_back(normal);
			elements[eleIndex] = verticies.size() - 1;
		}
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
			setNormal(normals, elements, normal, i, verticies);
			setNormal(normals, elements, normal, i + 1, verticies);
			setNormal(normals, elements, normal, i + 2, verticies);
			//apply normal for each vert index;
			
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

	void Mesh::setupBuffers(void){
		if(vbo_verts != 0 || vbo_normals != 0 || ibo != 0 ){
			throw std::runtime_error("Buffers already setup!");
		}

	int vertsize = sizeof(glm::vec4) * verticies.size();
	glGenBuffers(1, &vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, vertsize, &verticies[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);


	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * elements.size(), &elements[0], GL_STATIC_DRAW);
	


	//glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	const GLuint Mesh::getVertsVBO(void) {
		return this->vbo_verts;
	}
	
	const GLuint Mesh::getNormalsVBO(void) {
		return this->vbo_normals;
	}

	const GLuint Mesh::getIBO(void) {
		return this->ibo;
	}

	void Mesh::setOriginOffset(const glm::vec3 offset){
		this->originOffset = glm::vec3(offset);
	}

	glm::vec3 Mesh::getOriginOffset(void) const{
		return glm::vec3(this->originOffset);
	}

	/*
	const GLuint Mesh::getVAO(void) {
		return this->vao;
	}
	*/
}

