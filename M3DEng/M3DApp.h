#ifndef _M3DAPP_H_
#define _M3DAPP_H_

#include "ResourceManager.h"
#include "SceneManager.h"
#include "RenderSystem.h"

namespace M3D{
	class M3DApp{
	public:
		M3DApp();
		~M3DApp();
		bool initialize(int width, int height);
		int run();
	private:
		void update(double time);
		void render(double time);
		void shutdown();
		bool initialized;
		bool running;
		ResourceManager resourceManager;
		SceneManager sceneManager;
		RenderSystem renderSystem;
		int screenWidth;
		int screenHeight;
	};
}


#endif