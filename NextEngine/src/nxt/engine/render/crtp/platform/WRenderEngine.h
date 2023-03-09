#pragma once

#include "../RenderEngine.h"

#ifdef NXT_PLATFORM_WINDOWS

constexpr const wchar_t* GWindowClassName{ L"NxtWindowClass" };
constexpr const wchar_t* GWindowDescription{ L"WINDOW HEHE" };

constexpr int GWidth{ 1920 };
constexpr int GHeight{ 1080 };

// OpenGL headers
#include <glad/glad.h>

namespace nxt::render
{

	class GLRender : public RenderEngine<GLRender>
	{
	public:
		bool IInit(HWND& windowHandle)
		{

			PIXELFORMATDESCRIPTOR pixelFormatDesc{
			sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
			1,                     // version number  
			PFD_DRAW_TO_WINDOW |   // support window  
			PFD_SUPPORT_OPENGL |   // support OpenGL  
			PFD_DOUBLEBUFFER,      // double buffered  
			PFD_TYPE_RGBA,         // RGBA type  
			24,                    // 24-bit color depth  
			0, 0, 0, 0, 0, 0,      // color bits ignored  
			0,                     // no alpha buffer  
			0,                     // shift bit ignored  
			0,                     // no accumulation buffer  
			0, 0, 0, 0,            // accum bits ignored  
			32,                    // 32-bit z-buffer      
			0,                     // no stencil buffer  
			0,                     // no auxiliary buffer  
			PFD_MAIN_PLANE,        // main layer  
			0,                     // reserved  
			0, 0, 0                // layer masks ignored  
			};

			mDeviceContext = GetDC(windowHandle);
			int pixelFormat{ ChoosePixelFormat(mDeviceContext, &pixelFormatDesc) };
			SetPixelFormat(mDeviceContext, pixelFormat, &pixelFormatDesc);
			mRenderingContext = wglCreateContext(mDeviceContext);
			wglMakeCurrent(mDeviceContext, mRenderingContext);
			gladLoadGL();
			glViewport(0, 0, GWidth, GHeight);

			return true;
		}
		bool IRelease()
		{
			return true;
		}
	protected:
		HDC mDeviceContext;
		HGLRC mRenderingContext;
	};

}
#endif
