
#include "Window.h"

#include <nxt/render/RenderAPI.h>

#ifdef NXT_PLATFORM_WINDOWS

#include <glad/glad.h>

constexpr const wchar_t* GWindowClassName{ L"NxtWindowClass" };
constexpr const wchar_t* GWindowDescription{ L"WINDOW HEHE" };

constexpr int GWidth{ 1920 };
constexpr int GHeight{ 1080 };

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace nxt
{

	Window::Window()
	{
		Init();
	}

	Window::Window(std::function<bool(events::Event& ev)> func) :
		mCallback{ func }
	{
		Init();
	}

	Window::~Window()
	{
		Release();
	}

	bool Window::Init()
	{
		render::api::Init();

		mHinstance = GetModuleHandle(nullptr);

		WNDCLASSEX fClass{};
		fClass.cbClsExtra = NULL;
		fClass.cbSize = sizeof(WNDCLASSEX);
		fClass.cbWndExtra = NULL;
		fClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		fClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		fClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		fClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		fClass.hInstance = NULL;
		fClass.lpszClassName = GWindowClassName;
		fClass.lpszMenuName = L"";
		fClass.style = NULL;

		fClass.lpfnWndProc = WindowProcedure;

		// All window classes that an application registers are unregistered when it terminates.
		if (!RegisterClassEx(&fClass))
		{
			return false;
		}

		mWindowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, GWindowClassName, GWindowDescription, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, GWidth, GHeight, NULL, NULL, NULL, this);
		if (!mWindowHandle)
		{
			return false;
		}

		device::Init(&mWindowHandle);
		render::api::Init();

		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);

		render::api::SetClearColor(0.25f, 0.25f, 0.25f, 1.f);

		return true;
	}

	bool Window::OnCreate()
	{
		return true;
	}

	bool Window::OnUpdate(float dt)
	{
		device::SwapBuffers();
		render::api::Clear();

		return true;
	}

	bool Window::OnClose()
	{
		events::WindowClosed ev{};
		mCallback(ev);
		return true;
	}

	bool Window::OnResize(uint32_t x, uint32_t y)
	{
		std::cout << "resize\n";
		// first WM_SIZE is called before the callback is set
		events::WindowResized ev{ x, y };
		mCallback(ev);
		return true;
	}

	void Window::SetEventCallback(std::function<bool(events::Event& ev)> func)
	{
		mCallback = func;
	}

	bool Window::Release()
	{
		device::Release();
		if (mWindowHandle)
		{
			if (!DestroyWindow(mWindowHandle))
			{
				return false;
			}
		}
		return true;
	}

	bool Window::ProcessMessages()
	{
		MSG msg{};
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		OnUpdate(0.f);

		return false;
	}

}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case(WM_CREATE):
		{
			nxt::Window* fWindow{ (nxt::Window*)((LPCREATESTRUCT)lparam)->lpCreateParams };
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)fWindow);
			fWindow->OnCreate();
			break;
		}
		case(WM_SIZE):
		{
			nxt::Window* fWindow{ reinterpret_cast<nxt::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) };
			UINT fWidth{ LOWORD(lparam) };
			UINT fHeight{ HIWORD(lparam) };

			// UINT and uint32_t are the same thing, so no casting needed
			fWindow->OnResize(fWidth, fHeight);

			break;
		}
		case(WM_CLOSE): 
		{
			DestroyWindow(hwnd);
			break;
		};
		case(WM_DESTROY):
		{
			nxt::Window* fWindow{ reinterpret_cast<nxt::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) };
			fWindow->OnClose();
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

#endif