
#include "../Device.h"

#ifdef NXT_PLATFORM_WINDOWS

#include "WDropTarget.h"

#include <iostream>
#include <shellapi.h>

// rendering contexts
static HWND* gWindowHandle{ nullptr };
static HDC gDeviceContext;
static HGLRC gRenderingContext;

static nxt::device::DropTarget gDropTarget{};

namespace nxt::device
{

	bool Init(HWND* windowHandle)
	{
		gWindowHandle = windowHandle;

		// vkCreateWin32SurfaceKHR if I ever use vulkan
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

		HRESULT oleStatus{ OleInitialize(NULL) };
		HRESULT dropStatus{ RegisterDragDrop(*gWindowHandle, &gDropTarget) };

		return true;
	}

	bool Release()
	{
		RevokeDragDrop(*gWindowHandle);

		ReleaseDC(*gWindowHandle, gDeviceContext);
		wglDeleteContext(gRenderingContext);

		return true;
	}

	void SwapBuffers()
	{
		::SwapBuffers(gDeviceContext);
	}

	bool IsKeyDown(nxtKeycode keycode)
	{
		return (::GetKeyState(keycode) & 0x8000);
	}

	// Maybe use high definition mouse movement. Raw input

	// Returns window space coordinates
	glm::vec2 GetMousePosition()
	{
		POINT p;
		if (GetCursorPos(&p))
		{
			if (ScreenToClient(*gWindowHandle, &p))
			{
				return glm::vec2{ p.x, p.y };
			}
		}
		return glm::vec2{ 0.f };
	}

	glm::vec2 GetMouseScreenPosition()
	{
		POINT p;
		if (::GetCursorPos(&p))
		{
			return glm::vec2{ p.x, p.y };
		}
		return glm::vec2{ 0.f };
	}

	// Window space coordinates
	int SetCursorPosition(glm::vec2 pos)
	{
		/*RECT windowBounds{};
		if (::GetWindowRect(*gWindowHandle, &windowBounds))
		{
			return ::SetCursorPos(windowBounds.left + static_cast<int>(pos.x), windowBounds.top + static_cast<int>(pos.y));
		}
		return false;*/
		return ::SetCursorPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
	}

	void ShowCursor()
	{
		while (::ShowCursor(true) < 0);
	}

	void HideCursor()
	{
		//::ShowCursor(false);
		while (::ShowCursor(false) > 0);
	}

}

#endif
