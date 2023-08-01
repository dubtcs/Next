#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/events/Event.h>
#include <nxt/core/window/Window.h>
#include <nxt/render/RenderAPI.h>

namespace nxt
{

	// Interface is wrapper for custom functionality
	// and windowing processes

	class NXT_API AppInterface2
	{
	public:
		AppInterface2();
		virtual void OnUpdate(float dt) = 0;
		virtual void OnEvent(events::Event& ev) = 0;
	protected:
		SWindow mWindow;
		Render mRender;		// Rendering Context for thread
	};

}
