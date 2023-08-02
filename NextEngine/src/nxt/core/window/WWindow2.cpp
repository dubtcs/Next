
#include "Window2.h"
#include <nxt/core/utility/TypeConversions.h>
#include <nxt/core/device/Device2.h>

static const wchar_t* GWindowClassName{ L"NxtWindowClass" };

namespace nxt
{

	LRESULT CALLBACK NxtWindowProcedure2(HWND, UINT, WPARAM, LPARAM);

	Window2::Window2()
	{
		Init();
	}

	void Window2::Init(const std::string& name, int32_t w, int32_t h)
	{
		mHInstance = GetModuleHandle(nullptr);

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
		fClass.lpfnWndProc = NxtWindowProcedure2;

		// All window classes that an application registers are unregistered when it terminates.
		if (!RegisterClassEx(&fClass))
		{
			return;
		}

		std::wstring wName{ nxt::ToWideString(name) };

		DWORD windowStyle{ WS_OVERLAPPEDWINDOW };
		DWORD windowStyleEx{ WS_EX_OVERLAPPEDWINDOW };

		// This will actually make the client area 1920x1080
		RECT asize{ .left{0}, .top{0}, .right{w}, .bottom{h} };
		if (AdjustWindowRectEx(&asize, windowStyle, false, windowStyleEx))
		{
			w = asize.right - asize.left;
			h = asize.bottom - asize.top;
		}

		mWidth = w;
		mHeight = h;

		mWindowHandle = CreateWindowEx(windowStyleEx, GWindowClassName, wName.c_str(), windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, w, h, NULL, NULL, NULL, this);
		if (!mWindowHandle)
		{
			return;
		}

		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);
	}

	void Window2::CreateRenderContext()
	{

	}

}
