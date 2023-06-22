
#include "App.h"

#include <nxt/render/RenderAPI.h>
#include <nxt/core/log/Log.h>
#include <nxt/core/clock/Clock.h>

#ifdef NXT_PLATFORM_WINDOWS

nxt::clock::time_point gPreviousTime;

namespace nxt
{

	App::App(const std::string& title)
	{
		log::Init();
		mWindow = NewUnique<Window>(title, NXT_CALLBACK(App::OnEvent));
		NXT_LOG_INFO("App Created");
		gPreviousTime = clock::GetTime();
	}

	App::~App()
	{

	}

	void App::AddInterface(const Shared<AppInterface>& ap)
	{
		mInterfaces.push_back(ap);
	}

	void App::Run()
	{
		while (mRunning)
		{
			mWindow->ProcessMessages();
			double dt{ clock::GetDuration(gPreviousTime, clock::GetTime()) };
			for (Shared<AppInterface>& app : mInterfaces)
			{
				app->OnUpdate(dt);
			}
			gPreviousTime = clock::GetTime();
			Sleep(10);
		}
	}

	bool App::OnEvent(events::Event& ev)
	{
		events::Handler handler{ ev };
		handler.Fire<events::WindowClosed>(NXT_CALLBACK(App::OnWindowClose));
		handler.Fire<events::WindowResized>(NXT_CALLBACK(App::OnWindowResize));

		// reverse this for top to bottom
		for (Shared<AppInterface>& app : mInterfaces)
		{
			app->OnEvent(ev);
			//if (ev.Handled)
				//break;
		}

		// loop through layers
		return true;
	}

	bool App::OnWindowClose(events::WindowClosed& ev)
	{
		mRunning = false;
		return true;
	}

	bool App::OnWindowResize(events::WindowResized& ev)
	{
		NXT_LOG_TRACE("Window Resize: {0} {1}", ev.Width, ev.Height);
		render::command::SetViewport(ev.Width, ev.Height);
		return true;
	}

}

#endif
