
#include "Window.h"
#include <nxt/core/app/App.h>

#include <nxt/render/RenderAPI.h>

#include <nxt/core/input/InputEnums.h>

#include <nxt/core/log/Log.h>

#ifdef NXT_PLATFORM_WINDOWS

constexpr const wchar_t* GWindowClassName{ L"NxtWindowClass" };

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

	Window::Window(const std::string& name, int32_t width, int32_t height)
	{
		Init(name, width, height);
	}

	Window::~Window()
	{
		Release();
	}
	
	uint32_t Window::GetHeight() const
	{
		return mHeight;
	}

	uint32_t Window::GetWidth() const
	{
		return mWidth;
	}

	bool Window::Init(const std::string& name, int32_t width, int32_t height)
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
		fClass.style = CS_DBLCLKS;

		fClass.lpfnWndProc = NxtWindowProcedure;

		// All window classes that an application registers are unregistered when it terminates.
		if (!RegisterClassEx(&fClass))
		{
			return false;
		}

		std::wstring wName{ StringToWide(name) };

		DWORD windowStyle{ WS_OVERLAPPEDWINDOW };
		DWORD windowStyleEx{ WS_EX_OVERLAPPEDWINDOW };

		// This will actually make the client area 1920x1080
		RECT asize{ .left{0}, .top{0}, .right{width}, .bottom{height} };
		if (AdjustWindowRectEx(&asize, windowStyle, false, windowStyleEx))
		{
			width = asize.right - asize.left;
			height = asize.bottom - asize.top;
		}

		mWindowHandle = CreateWindowEx(windowStyleEx, GWindowClassName, wName.c_str(), windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, NULL, this);
		if (!mWindowHandle)
		{
			return false;
		}

		device::Init(&mWindowHandle);
		render::command::Init();

		mWidth = static_cast<uint32_t>(width);
		mHeight = static_cast<uint32_t>(height);

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
		return true;
	}

	bool Window::OnResize(events::WindowResized& ev)
	{
		NXT_LOG_TRACE("Resizing to: {0}, {1}", ev.Width, ev.Height);
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
		if (this != nullptr)
		{
			events::Handler handler{ ev };
			handler.Fire<events::WindowResized>(NXT_CALLBACK(Window::OnResize));
			return true;
		}
		return false;
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

void FireSizeEvent(LPARAM& lparam)
{
	UINT width{ LOWORD(lparam) };
	UINT height{ HIWORD(lparam) };
	nxt::events::WindowResized ev{ width, height };
	nxt::app::OnEvent(ev);
}

void FireMouseDownEvent(nxt::nxtKeycode keycode, bool isDouble)
{
	nxt::events::MouseButtonPressed ev{ keycode, isDouble };
	nxt::app::OnEvent(ev);
	return;
}

void FireMouseUpEvent(nxt::nxtKeycode keycode)
{
	nxt::events::MouseButtonReleased ev{ keycode };
	nxt::app::OnEvent(ev);
	return;
}

LRESULT CALLBACK NxtWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	using namespace nxt;
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
				default:
					break;
				case(SIZE_MAXIMIZED):
				{
					FireSizeEvent(lparam);
					break;
				}
				case(SIZE_MINIMIZED):
				{
					FireSizeEvent(lparam);
					break;
				}
				case(SIZE_RESTORED):
				{
					//FireSizeEvent(lparam);
					break;
				}
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
			nxt::events::KeyboardPressed ev{ static_cast<nxtKeycode>(wparam) };
			nxt::app::OnEvent(ev);
			break;
		}
		case(WM_LBUTTONDOWN):
		{
			// can also get x/y coords from lparam
			FireMouseDownEvent(nxtKeycode_Mouse1, false);
			break;
		}
		case(WM_LBUTTONUP):
		{
			FireMouseUpEvent(nxtKeycode_Mouse1);
			break;
		}
		case(WM_LBUTTONDBLCLK):
		{
			FireMouseDownEvent(nxtKeycode_Mouse1, true);
			break;
		}
		case(WM_RBUTTONDOWN):
		{
			FireMouseDownEvent(nxtKeycode_Mouse2, false);
			break;
		}
		case(WM_RBUTTONUP):
		{
			FireMouseUpEvent(nxtKeycode_Mouse2);
			break;
		}
		case(WM_RBUTTONDBLCLK):
		{
			FireMouseDownEvent(nxtKeycode_Mouse2, true);
			break;
		}
		case(WM_MOUSEWHEEL):
		{
			float delta{ static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam)) };
			nxt::events::MouseScroll ev{ delta };
			nxt::app::OnEvent(ev);
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

#endif