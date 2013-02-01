#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

namespace M3D{
	class ResourceManager{
	public:
		ResourceManager();
		~ResourceManager();
		bool initialize();
	private:
		bool initialized;
	};

}

#endif