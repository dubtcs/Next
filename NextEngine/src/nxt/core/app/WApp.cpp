
#include "App.h"

#include <nxt/render/RenderAPI.h>
#include <nxt/core/log/Log.h>
#include <nxt/core/clock/Clock.h>
#include <nxt/core/window/Window.h>
#include <nxt/core/input/Input.h>

static class nxtCursorCleaner
{
public:
	nxtCursorCleaner() = default;
	~nxtCursorCleaner()
	{
		std::cout << "Showing cursor.";
		nxt::input::ShowCursor();
	}
} gCleaner{};

#ifdef NXT_PLATFORM_WINDOWS

namespace nxt::app
{

	static bool gRunning{ false };
	static clock::time_point gPreviousTime;

	static Unique<Window> gWindow;
	static std::vector<Shared<AppInterface>> gInterfaces;

	static bool OnWindowClose(events::WindowClosed& ev)
	{
		gRunning = false;
		return false;
	}

	static bool OnWindowResize(events::WindowResized& ev)
	{
		render::command::SetViewport(ev.Width, ev.Height);
		return false;
	}

	void Launch(const std::string& appName, int32_t width, int32_t height)
	{
		log::Init();
		gWindow = NewUnique<Window>(appName, width, height);
		gPreviousTime = clock::GetTime();
		gRunning = true;
	}

	void Run()
	{
		while (gRunning)
		{
			gWindow->ProcessMessages();
			double dt{ clock::GetDuration(gPreviousTime, clock::GetTime()) };
			for (Shared<AppInterface>& app : gInterfaces)
			{
				app->OnUpdate(dt);
			}
			gPreviousTime = clock::GetTime();
			Sleep(10);
		}
	}

	void AddInterface(const Shared<AppInterface>& appInter)
	{
		gInterfaces.push_back(appInter);
	}

	bool OnEvent(events::Event& ev)
	{
		events::Handler handler{ ev };
		handler.Fire<events::WindowClosed>(NXT_CALLBACK_STATIC(app::OnWindowClose));
		handler.Fire<events::WindowResized>(NXT_CALLBACK_STATIC(app::OnWindowResize));
		gWindow->OnEvent(ev);

		for (Shared<AppInterface>& app : gInterfaces)
		{
			app->OnEvent(ev);
		}

		return true;
	}

}

#endif
