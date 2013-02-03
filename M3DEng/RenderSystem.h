#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_

namespace M3D{
	class RenderSystem{
	public:
		RenderSystem(void);
		~RenderSystem(void);
		bool initialize(void);
		void shutdown(void);
	private:
		bool initialized;
	};

}

#endif