#include <iostream>
#include <OpenMesh\Core\IO\MeshIO.hh>
#include <glm\glm.hpp>
#include <math.h>
#include <stdexcept>

#include "hw2.h"

namespace HW2{

	HW2Driver::HW2Driver(void){
	
	}
	HW2Driver::~HW2Driver(void){
	
	}

    /*!
     * \brief HW2Driver::loadMesh loads a mesh into open meshe's half edge structure
     * \param filename name of the file to load
     * \return true on success
     */
	bool HW2Driver::loadMesh(std::string filename){
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

        return true;
	}

    /*!
     * \brief HW2Driver::calcEulerFromMesh calculates the euler number from a loaded mesh
     * \return euler number or the mesh
     */
	int HW2Driver::calcEulerFromMesh(){
		int vertices = myMesh.n_vertices();
		int faces = myMesh.n_faces();
		int edges = myMesh.n_edges();
        int euler = vertices + faces - edges; //X = v + f - e
		return euler;
	}

    /*!
     * \brief HW2Driver::calcEulerFromGenus calculates the euler numer based on a genus
     * \param genus genus to calculate from
     * \return euler number of a mesh with genus supplied
     */
	int HW2Driver::calcEulerFromGenus(int genus){
        int euler = 2 - (2 * genus); //e = 2-2g
		return euler;
	}

    /*!
     * \brief HW2Driver::calcGausCurveFromMesh calculates the gaussian curvature of the loaded mesh
     * \return gaussian curvature of the mesh
     */
	double HW2Driver::calcGausCurveFromMesh(){
		double totalCurve = 0;
		MyMesh::VertexIter v_it, v_end(myMesh.vertices_end());

        //iterate through all of the verticies in a mesh
		for(v_it = myMesh.vertices_begin(); v_it != v_end; ++v_it){
			MyMesh::VertexHandle vert = v_it.handle();
            double vertCurve = calcGaussAtVertex(vert); //calculate the gauss curve at vertex
            totalCurve += vertCurve; //total the vertex curvatures
		}
		
		return totalCurve;
	}

    /*!
     * \brief HW2Driver::calcGausCurveFromEuler calculates the guassian curvature from a euler number
     * \param euler euler number to calculate from
     * \return gaussian curvature for suplied euler number
     */
	double HW2Driver::calcGausCurveFromEuler(int euler){
        return 2 * M_PI * euler; //curve = 2PI*X
	}

    /*!
     * \brief pointToVec3 convertes an openmesh point to a glm vec3
     * \param point openMesh point to convert
     * \return vec3 version of suplied point
     */
    static glm::vec3 pointToVec3(MyMesh::Point point){
        if(point.size() != 3)
            throw std::runtime_error("Point was not equal to 3");

		const float* data = point.data();
		return glm::vec3(data[0], data[1], data[2]);
	}

    /*!
     * \brief magOfVec calculates the magnatidue of a vector
     * \param vector vector to calculate for
     * \return magnitude of supplied vector
     */
    static double magOfVec(glm::vec3 vector){
		return sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
	}

    /*!
     * \brief angleFromVecs gets the inner angle from the vectors AB and AC
     * \param vecA vector of point A
     * \param vecB vector of point B
     * \param vecC vector of point C
     * \return inner angle
     */
    static double angleFromVecs(glm::vec3 vecA, glm::vec3 vecB, glm::vec3 vecC){
        glm::vec3 u = vecA - vecB; //AB
        glm::vec3 v = vecA - vecC; //AC
		 
        double magU = magOfVec(u); //|U|
        double magV = magOfVec(v); //|V|

        //check to see if magnitues were 0. Only happens if A=B or A=C from invalid data
        if(magU == 0 || magV == 0)
            throw std::runtime_error("Magnitude of a vector was 0!");

        //get cos(theta) by U*V / |U|*|V|
		double cosAngle = glm::dot(u,v) / (magU * magV);

        //clamp the value incase of double precision
		if(cosAngle< -1)
			cosAngle = -1;

		if(cosAngle> 1)
			cosAngle = 1;

        //find angle through arccos
		double angle = acos(cosAngle);

		return angle;
	}

    /*!
     * \brief HW2Driver::calcGaussAtVertex calculates the gaussian curvature for a specific vertex
     * \param vertex vertex to calculate curvature for
     * \return curvature at the supplied vertex
     */
	double HW2Driver::calcGaussAtVertex(MyMesh::VertexHandle vertex){
		double totalAngle = 0;
		glm::vec3 vecA = pointToVec3(myMesh.point(vertex));

		//find the toal angle for all inner angles at this vertex
		MyMesh::VertexVertexIter vv_it(myMesh.vv_begin(vertex)), vv_end(myMesh.vv_end(vertex));

        //get first point B
		MyMesh::VertexHandle vertHandle = vv_it.handle();
		glm::vec3 vecB = pointToVec3(myMesh.point(vertHandle));
		glm::vec3 first = vecB;

        //iterate through remaining neighboring verticies and find angles
		for(++vv_it; vv_it != vv_end; ++vv_it){
			vertHandle = vv_it.handle();
            glm::vec3 vecC = pointToVec3(myMesh.point(vertHandle));
            double angle = angleFromVecs(vecA, vecB, vecC);
            totalAngle += angle;

            //rotate points for next iteration
            vecB = vecC;
		}

		//get angle between first and last set of vecs
		glm::vec3 vecC = first;
		totalAngle += angleFromVecs(vecA, vecB, vecC);

		//calculate the curveature at this vertex
		double curve = (2 * M_PI) - totalAngle;

		return curve;
	}
}
