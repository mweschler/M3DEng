#include <iostream>
#include <OpenMesh\Core\IO\MeshIO.hh>
#include <glm\glm.hpp>

#include "hw2.h"

namespace HW2{
bool loadMesh(std::string filename);
	HW2Driver::HW2Driver(void){
	
	}
	HW2Driver::~HW2Driver(void){
	
	}

	bool HW2Driver::loadMesh(std::string filename){
		std::cout<<"Starting loadMesh operations"<<std::endl;
		int status = OpenMesh::IO::read_mesh(myMesh, filename);
		if(!status){
			std::cerr<<"Could not open mesh "<<filename<<std::endl;
			return false;
		}

		std::cout<<"Mesh loaded"<<std::endl
			<<"Number of vertices: "<<myMesh.n_vertices()<<std::endl
			<<"Number of edges: "<<myMesh.n_edges()<<std::endl
			<<"Number of faces: "<<myMesh.n_faces()<<std::endl
			<<"Number of half-edges: "<<myMesh.n_halfedges()<<std::endl;

	}

	int HW2Driver::calcEulerFromMesh(){
		//MyMesh::VertexIter v_it, v_end(myMesh.vertices_end());
		//v_it = myMesh.vertices_begin();
		int vertices = myMesh.n_vertices();
		int faces = myMesh.n_faces();
		int edges = myMesh.n_edges();
		int euler = vertices + faces - edges;
		return euler;
	}

	int HW2Driver::calcEulerFromGenus(int genus){
		int euler = 2 - (2 * genus);
		return euler;
	}

	double HW2Driver::calcGausCurveFromMesh(){
		double totalCurve = 0;
		MyMesh::VertexIter v_it, v_end(myMesh.vertices_end());

		for(v_it = myMesh.vertices_begin(); v_it != v_end; ++v_it){
			MyMesh::VertexHandle vert = v_it.handle();
			totalCurve += calcGaussAtVertex(vert);
			
		}

		MyMesh::VertexHandle vert = v_it.handle();
		MyMesh::Point point = myMesh.point(vert);
		
		return totalCurve;
	}

	double HW2Driver::calcGausCurveFromEuler(int euler){
		return 2 * M_PI * euler;
	}

	double HW2Driver::calcGaussAtVertex(MyMesh::VertexHandle vertex){
		double totalAngle = 0;
		MyMesh::Point vertPoint = myMesh.point(vertex);
		const float* data = vertPoint.data();
		glm::vec3 vec1 = glm::vec3(data[0], data[1], data[2]);

		//find the toal angle for all inner angles at this vertex
		MyMesh::VertexOHalfedgeIter he_it(myMesh.voh_begin(vertex)), he_end(myMesh.voh_end(vertex));
		MyMesh::Halfedge he = myMesh.halfedge(he_it);
		vertPoint = //TODO: Vertex to Vec3, find angles
		glm::vec3 vec2;
		
		for(; he_it != he_end; ++he_it){
			MyMesh::Halfedge 
		}


		//calculate the curveature at this vertex
		double curve = (2 * M_PI) - totalAngle;
		return curve;
	}
}