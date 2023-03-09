#pragma once

#include <nxt/engine/EngineCore.h>

#include <iostream>

namespace nxt
{

	namespace types
	{
		enum RenderApi
		{
			OpenGL = BIT(0),
		};
	}

	namespace render
	{
		#ifdef NXT_PLATFORM_WINDOWS

		template<typename derived>
		class RenderEngine
		{
		public:
			bool Init(HWND& windowHandle)
			{
				return (static_cast<derived*>(this))->IInit(windowHandle);
			}
			bool Release()
			{
				return (static_cast<derived*>(this))->IRelease();
			}
		};

		#elif NXT_PLATFORM_LINUX

		template<typename derived>
		class RenderEngine
		{
		public:
			bool Init()
			{
				return (static_cast<derived*>(this))->IInit();
			}
			bool Release()
			{
				return (static_cast<derived*>(this))->IRelease();
			}
		};

		#endif

	}

}
