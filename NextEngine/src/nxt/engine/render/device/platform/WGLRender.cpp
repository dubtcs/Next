
#include "../GLRender.h"

#include <iostream>

#ifdef NXT_PLATFORM_WINDOWS

#include <glad/glad.h>

// rendering contexts
static HWND* gWindowHandle{ nullptr };
static HDC gDeviceContext;
static HGLRC gRenderingContext;

constexpr const wchar_t* GWindowClassName{ L"NxtWindowClass" };
constexpr const wchar_t* GWindowDescription{ L"WINDOW HEHE" };

constexpr int GWidth{ 1920 };
constexpr int GHeight{ 1080 };

namespace nxt::render
{

	bool Init(HWND* windowHandle)
	{
		gWindowHandle = windowHandle;

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

		gDeviceContext = GetDC(*gWindowHandle);
		int pixelFormat{ ChoosePixelFormat(gDeviceContext, &pixelFormatDesc) };
		SetPixelFormat(gDeviceContext, pixelFormat, &pixelFormatDesc);
		gRenderingContext = wglCreateContext(gDeviceContext);
		wglMakeCurrent(gDeviceContext, gRenderingContext);
		gladLoadGL();
		glViewport(0, 0, GWidth, GHeight);

		return true;
	}

	bool Release()
	{
		ReleaseDC(*gWindowHandle, gDeviceContext);
		wglDeleteContext(gRenderingContext);

		return true;
	}

	void FrameStart()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(gDeviceContext);
	}

	void FrameEnd()
	{

	}

	void SetClearColor(float& r, float& g, float& b)
	{

	}

}

#endif
