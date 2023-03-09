#pragma once

#include <nxt/engine/EngineCore.h>

/*


consider the curiously recurring template pattern for compile time functions
https://en.cppreference.com/w/cpp/language/crtp


*/

namespace nxt
{

	namespace types
	{
		enum RenderApi
		{
			None = 0,
			OpenGL = BIT(1),
		};
	}

	namespace render
	{

		class NXT_API RenderDevice
		{
			virtual bool Init() = 0;
			virtual bool Release() = 0;
			virtual ~RenderDevice();
			static Shared<RenderDevice> GetApi() { return sInstance; }
		protected:
			static Shared<RenderDevice> sInstance;
		protected:
		#ifdef NXT_PLATFORM_WINDOWS
			HDC mDeviceContext;
			HGLRC mRenderingContext;
		#else
			#error "Only windows OpenGL supported"
		#endif
		};

		class NXT_API GLRenderDevice : public RenderDevice
		{
		public:
			virtual bool Init() override;
			virtual bool Release() override;
			virtual ~GLRenderDevice();
		};

	}
}
