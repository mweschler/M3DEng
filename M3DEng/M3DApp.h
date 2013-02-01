#ifndef _M3DAPP_H_
#define _M3DAPP_H_

#include "ResourceManager.h"

namespace M3D{
	class M3DApp{
	public:
		M3DApp();
		~M3DApp();
		bool initialize(int width, int height);
		int run();
	private:
		void shutdown();
		bool initialized;
		bool running;
		ResourceManager resourceManager;
	};
}


#endif