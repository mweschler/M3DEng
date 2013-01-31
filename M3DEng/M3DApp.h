#ifndef _M3DAPP_H_
#define _M3DAPP_H_

namespace M3D{
	class M3DApp{
	public:
		M3DApp();
		~M3DApp();
		bool initialize();
		int run();
	private:
		void shutdown();
		bool initialized;
	};
}


#endif