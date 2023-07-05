#pragma once

#include <nxt/EngineCore.h>

#include "nxtid.h"

#include <necs/include.h>

namespace nxt
{

	struct NXT_API EntityData
	{
		necs::Entity entity;
		nxtid id;
	};

	class NXT_API GameWorld
	{
	public:
		GameWorld();
	public:
		necs::Entity CreateEntity();
		necs::Entity CreateEntity(necs::Entity& parent);
		necs::Entity CreateEntityWithID(necs::Entity& parent, const nxtid& id);

	protected:
		necs::Scene mScene;
	};

}
