#pragma once

#include <nxt/EngineCore.h>

#include <necs/include.h>

namespace nxt
{

	class NXT_API GameWorld
	{
	public:
		GameWorld() = default;
		necs::Entity CreateEntity();
	protected:
		necs::Scene mScene;
	};

}
