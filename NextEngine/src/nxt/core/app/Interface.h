#pragma once

// nxt::app Interface

#include <nxt/EngineCore.h>
#include <nxt/core/events/Event.h>

namespace nxt
{

	class NXT_API AppInterface
	{
	public:
		virtual void OnUpdate(float& dt, bool isFocused) = 0;
		virtual bool OnEvent(events::Event& ev) = 0;
	};

}
