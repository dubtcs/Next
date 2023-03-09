
#include "App.h"

#include <nxt/engine/render/crtp/RenderEngine.h>

#ifdef NXT_PLATFORM_WINDOWS

namespace nxt
{

	App::App()
	{
		mWindow = NewUnique<Window>(NXT_BIND_FN(App::OnEvent));
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
		events::Handler fHandler{ ev };
		fHandler.Fire<events::WindowClosed>(NXT_BIND_FN(App::OnWindowClose));

		// loop through layers
		return true;
	}

	bool App::OnWindowClose(events::WindowClosed& ev)
	{
		mRunning = false;
		return true;
	}

}

#endif
