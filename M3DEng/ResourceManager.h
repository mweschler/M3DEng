#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

namespace M3D{
	class ResourceManager{
	public:
		ResourceManager(void);
		~ResourceManager(void);
		bool initialize(void);
		void shutdown(void);
	private:
		bool initialized;
	};

}

#endif