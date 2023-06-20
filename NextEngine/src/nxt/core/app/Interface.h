#pragma once

// nxt::app Interface

#include <nxt/EngineCore.h>
#include <nxt/core/events/Event.h>

namespace nxt
{

	class NXT_API AppInterface
	{
	public:
		virtual void OnUpdate(double& dt) = 0;
		virtual void OnEvent(events::Event& ev) = 0;
	};

}
