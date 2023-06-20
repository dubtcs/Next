
#include "App.h"

#include <nxt/render/RenderAPI.h>
#include <nxt/core/log/Log.h>

#ifdef NXT_PLATFORM_WINDOWS

namespace nxt
{

	App::App()
	{
		mWindow = NewUnique<Window>(NXT_CALLBACK(App::OnEvent));
		log::Init();
		NXT_LOG_INFO("App Created");
	}

	App::~App()
	{

	}

	void App::Run()
	{
		while (mRunning)
		{
			mWindow->ProcessMessages();
			Sleep(10);
		}
	}

	bool App::OnEvent(events::Event& ev)
	{
		events::Handler handler{ ev };
		handler.Fire<events::WindowClosed>(NXT_CALLBACK(App::OnWindowClose));
		handler.Fire<events::WindowResized>(NXT_CALLBACK(App::OnWindowResize));

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
		render::api::SetViewport(ev.Width, ev.Height);
		return true;
	}

}

#endif
