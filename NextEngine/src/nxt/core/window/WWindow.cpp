
#include "Window.h"
#include <nxt/core/app/App.h>

#include <nxt/render/RenderAPI.h>

#include <nxt/core/input/InputEnums.h>

#ifdef NXT_PLATFORM_WINDOWS

constexpr const wchar_t* GWindowClassName{ L"NxtWindowClass" };

constexpr int GWidth{ 1920 };
constexpr int GHeight{ 1080 };

/*

IF I EVER COME BACK BECAUSE PIXELS ARE OFF BY A FEW VERTICALLY!!!
Win32 includes the title bar in it's window construction.
So (1920 x 1080) is actually (1900 x 1037) in my case atm

*/

LRESULT CALLBACK NxtWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace nxt
{

	static std::wstring StringToWide(const std::string& str)
	{
		std::wstring wStr{ str.begin(), str.end() };
		return wStr;
	}

	Window::Window()
	{
		Init();
	}

	Window::Window(const std::string& name, std::function<bool(events::Event& ev)> func) :
		mCallback{ func }
	{
		Init(name);
	}

	Window::~Window()
	{
		Release();
	}

	bool Window::Init(const std::string& name)
	{
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

		fClass.lpfnWndProc = NxtWindowProcedure;

		// All window classes that an application registers are unregistered when it terminates.
		if (!RegisterClassEx(&fClass))
		{
			return false;
		}

		std::wstring wName{ StringToWide(name) };
		mWindowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, GWindowClassName, wName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, GWidth, GHeight, NULL, NULL, NULL, this);
		if (!mWindowHandle)
		{
			return false;
		}

		device::Init(&mWindowHandle);
		render::command::Init();

		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);

		render::command::SetClearColor(0.25f, 0.25f, 0.25f, 1.f);

		return true;
	}

	bool Window::OnCreate()
	{
		return true;
	}

	bool Window::OnUpdate(float dt)
	{
		device::SwapBuffers();
		//render::api::Clear();

		return true;
	}

	bool Window::OnClose(events::WindowClosed& ev)
	{
		//events::WindowClosed ev{};
		//app::OnEvent(ev);
		return true;
	}

	bool Window::OnResize(events::WindowResized& ev)
	{
		// first WM_SIZE is called before the callback is set
		mWidth = ev.Width;
		mHeight = ev.Height;
		return true;
	}

	bool Window::OnKeyPressed(events::KeyboardPressed& ev)
	{
		return false;
	}

	bool Window::OnEvent(events::Event& ev)
	{
		events::Handler handler{ ev };
		handler.Fire<events::WindowResized>(NXT_CALLBACK(Window::OnResize));
		//handler.Fire<events::WindowClosed>(NXT_CALLBACK(Window::OnClose));
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

LRESULT CALLBACK NxtWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
			switch (wparam)
			{
				case(SIZE_MAXIMIZED):
				{
					nxt::Window* window{ reinterpret_cast<nxt::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) };
					UINT width{ LOWORD(lparam) };
					UINT height{ HIWORD(lparam) };
					//window->OnResize(width, height);
					nxt::events::WindowResized ev{ width, height };
					nxt::app::OnEvent(ev);
					break;
				}
				case(SIZE_MINIMIZED):
				{
					nxt::Window* window{ reinterpret_cast<nxt::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) };
					UINT width{ LOWORD(lparam) };
					UINT height{ HIWORD(lparam) };
					//window->OnResize(width, height);
					nxt::events::WindowResized ev{ width, height };
					nxt::app::OnEvent(ev);
					break;
				}
				default:
					break;
			}
			break;
		}
		case(WM_EXITSIZEMOVE): // used to be WM_SIZE. CHanged to prevent spamming
		{
			nxt::Window* window{ reinterpret_cast<nxt::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) };
			uint32_t width{ 0 };
			uint32_t height{ 0 };
			RECT windowSize;
			if (GetClientRect(window->mWindowHandle, &windowSize))
			{
				width = windowSize.right - windowSize.left;
				height = windowSize.bottom - windowSize.top;
				if (width != window->mWidth || height != window->mHeight)
				{
					nxt::events::WindowResized ev{ width, height };
					nxt::app::OnEvent(ev);
				}
			}
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

			nxt::events::WindowClosed ev{};
			nxt::app::OnEvent(ev);

			PostQuitMessage(0);
			break;
		}
		case(WM_KEYDOWN):
		{
			nxt::Window* window{ reinterpret_cast<nxt::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) };
			switch (msg)
			{
				case(WM_KEYDOWN):
				{
					nxt::events::KeyboardPressed ev{ static_cast<nxt::input::KEYCODE_>(wparam) };
					nxt::app::OnEvent(ev);
				}
			}
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

#endif