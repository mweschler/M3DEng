#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include <vector>

namespace M3D{
	class SceneManager
	{
	public:
		SceneManager(void);
		~SceneManager(void);
		bool initialize(void);
		void shutdown(void);
	private:
		bool initalized;
		std::vector<Entity> entities;
	};

}

#endif

