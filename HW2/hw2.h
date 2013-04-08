#ifndef _HW2_
#define _HW2_

#include <string>
#include <OpenMesh\Core\Mesh\TriMesh_ArrayKernelT.hh>

namespace HW2{
	typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

    /*!
     * \brief The HW2Driver class a simple class that calculates the various requirements of HW2 as well as handels mesh loading
     */
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

#endif
