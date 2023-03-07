
#include "Window.h"

#ifdef NXT_PLATFORM_WINDOWS

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

	Window::Window(std::function<bool(events::Event& ev)> func)
	{
		SetEventCallback(func);
		Init();
	}

	Window::~Window()
	{
		Release();
	}

	bool Window::Init()
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

		fClass.lpfnWndProc = WindowProcedure;

		std::cout << 1;
		if (!RegisterClassEx(&fClass))
		{
			return false;
		}

		std::cout << 2;
		mHWND = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, GWindowClassName, GWindowDescription, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, GWidth, GHeight, NULL, NULL, NULL, this);
		if (!mHWND)
		{
			return false;
		}

		std::cout << 3;
		ShowWindow(mHWND, SW_SHOW);
		UpdateWindow(mHWND);

		return true;
	}

	bool Window::OnCreate()
	{

		return true;
	}

	bool Window::OnUpdate(float dt)
	{
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
		// first WM_SIZE is called before the callback is set
		//if (mCallback)
		//{
		events::WindowResized ev{ x, y };
		mCallback(ev);
		//}
		return true;
	}

	void Window::SetEventCallback(std::function<bool(events::Event& ev)> func)
	{
		mCallback = func;
	}

	bool Window::Release()
	{
		if (mHWND)
		{
			if (!DestroyWindow(mHWND))
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