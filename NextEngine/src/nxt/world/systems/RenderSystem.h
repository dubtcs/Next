#pragma once

#include <nxt/EngineCore.h>

#include "../World.h"

namespace nxt
{

	class NXT_API RenderSystem
	{
	public:
		RenderSystem();
	protected:
		SWorld mWorld;
	};

}
