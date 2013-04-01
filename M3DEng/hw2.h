#pragma once

#include <string>
#include <OpenMesh\Core\Mesh\TriMesh_ArrayKernelT.hh>
namespace HW2{
	typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

	class HW2Driver
	{
	public:
		HW2Driver(void);
		~HW2Driver(void);
		bool loadMesh(std::string filename);
		int calcEulerFromMesh();
		int calcEulerFromGenus(int genus);
		double calcGausCurveFromMesh();
		double calcGausCurveFromEuler(int euler);
	private:
		double calcGaussAtVertex(MyMesh::VertexHandle vertex);
		MyMesh myMesh;
	};
}
