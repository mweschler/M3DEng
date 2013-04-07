#include <iostream>
#include <OpenMesh\Core\IO\MeshIO.hh>
#include <glm\glm.hpp>
#include <math.h>

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
		int i = 0;
		for(v_it = myMesh.vertices_begin(); v_it != v_end; ++v_it){
			MyMesh::VertexHandle vert = v_it.handle();
			double vertCurve = calcGaussAtVertex(vert);
			
			totalCurve += calcGaussAtVertex(vert);
			++i;
			if(i % 100 == 0)
				std::cerr<<"vertCurve: "<<vertCurve<<" totalCurve: "<<totalCurve<<std::endl;
		}
		
		return totalCurve;
	}

	double HW2Driver::calcGausCurveFromEuler(int euler){
		return 2 * M_PI * euler;
	}

	//convertes an openmesh point to a glm vec3
	glm::vec3 pointToVec3(MyMesh::Point point){
		const float* data = point.data();
		return glm::vec3(data[0], data[1], data[2]);
	}

	double magOfVec(glm::vec3 vector){
		return sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
	}

	double angleFromVecs(glm::vec3 vecA, glm::vec3 vecB, glm::vec3 vecC){
		glm::vec3 u = vecA - vecB;
		glm::vec3 v = vecA - vecC;

		//std::cerr<<"VertB: ("<<vecB.x<<", "<<vecB.y<<", "<<vecB.z<<")"<<std::endl;
		//std::cerr<<"VertC: ("<<vecC.x<<", "<<vecC.y<<", "<<vecC.z<<")"<<std::endl;

		//std::cerr<<"U(A-B): ("<<u.x<<", "<<u.y<<", "<<u.z<<")"<<std::endl;
		//std::cerr<<"V(A-C): ("<<v.x<<", "<<v.y<<", "<<v.z<<")"<<std::endl;
		 
		double magU = magOfVec(u);
		double magV = magOfVec(v);

		//std::cerr<<"U*V = "<<glm::dot(u,v)<<std::endl;
		//std::cerr<<"|U| = "<<magU<<std::endl;
		//std::cerr<<"|V| = "<<magV<<std::endl;
		double cosAngle = glm::dot(u,v) / (magU * magV);

		//std::cerr<<"Cos of angle: "<<cosAngle<<std::endl;
		if(cosAngle< -1)
			cosAngle = -1;

		if(cosAngle> 1)
			cosAngle = 1;

		double angle = acos(cosAngle);

		//std::cerr<<"angle is "<<angle<<" deg: "<<(angle *180.0 / M_PI)<<std::endl;
		return angle;
	}

	double HW2Driver::calcGaussAtVertex(MyMesh::VertexHandle vertex){
		double totalAngle = 0;
		glm::vec3 vecA = pointToVec3(myMesh.point(vertex));

		//std::cerr<<"Finding curveature at vertex: ("<<vecA.x<<", "<<vecA.y<<", "<<vecA.z<<")"<<std::endl;

		//find the toal angle for all inner angles at this vertex
		MyMesh::VertexVertexIter vv_it(myMesh.vv_begin(vertex)), vv_end(myMesh.vv_end(vertex));

		MyMesh::VertexHandle vertHandle = vv_it.handle();
		
		glm::vec3 vecB = pointToVec3(myMesh.point(vertHandle));
		glm::vec3 first = vecB;

		for(++vv_it; vv_it != vv_end; ++vv_it){
			vertHandle = vv_it.handle();
			 glm::vec3 vecC = pointToVec3(myMesh.point(vertHandle));
			 double angle = angleFromVecs(vecA, vecB, vecC);
			 totalAngle += angle;

			 vecB = vecC;
		}

		//get angle between first and last set of vecs
		glm::vec3 vecC = first;
		totalAngle += angleFromVecs(vecA, vecB, vecC);

		//std::cerr<<"Total angle: "<<totalAngle<<" deg: "<<(totalAngle *180.0 / M_PI)<<std::endl;
		//calculate the curveature at this vertex
		double curve = (2 * M_PI) - totalAngle;
		//std::cerr<<"Curve at vertex: "<<curve<<std::endl;
		return curve;
	}
}