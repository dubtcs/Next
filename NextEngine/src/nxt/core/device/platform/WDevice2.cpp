
#include "../Device2.h"

#ifdef NXT_PLATFORM_WINDOWS
namespace nxt
{

	Device::Device(HWND* windowHandle) :
		mWindowHandle{ windowHandle }
	{
		//PIXELFORMATDESCRIPTOR pixelFormatDesc{
		//	sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
		//	1,                     // version number  
		//	PFD_DRAW_TO_WINDOW |   // support window  
		//	PFD_SUPPORT_OPENGL |   // support OpenGL  
		//	PFD_DOUBLEBUFFER,      // double buffered  
		//	PFD_TYPE_RGBA,         // RGBA type  
		//	24,                    // 24-bit color depth  
		//	0, 0, 0, 0, 0, 0,      // color bits ignored  
		//	0,                     // no alpha buffer  
		//	0,                     // shift bit ignored  
		//	0,                     // no accumulation buffer  
		//	0, 0, 0, 0,            // accum bits ignored  
		//	32,                    // 32-bit z-buffer      
		//	0,                     // no stencil buffer  
		//	0,                     // no auxiliary buffer  
		//	PFD_MAIN_PLANE,        // main layer  
		//	0,                     // reserved  
		//	0, 0, 0                // layer masks ignored  
		//};

		//HDC deviceContext = GetDC(*mWindowHandle);
		//int pixelFormat{ ChoosePixelFormat(deviceContext, &pixelFormatDesc) };
		//SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDesc);
		//mRenderContext = wglCreateContext(deviceContext);
		//wglMakeCurrent(deviceContext, mRenderContext);
	}

	bool Device::Release()
	{
		//RevokeDragDrop(*mWindowHandle);

		// This will invalidate other windows
		// Need another intermediate class for individual rendering contexts.
		// Device should be application lifespan/namespace

		//ReleaseDC(*mWindowHandle, gDeviceContext);
		//wglDeleteContext(mRenderContext);

		return true;
	}

}
#endif