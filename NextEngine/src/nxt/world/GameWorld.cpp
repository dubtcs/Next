
#include "GameWorld.h"

namespace nxt
{

	necs::Entity GameWorld::CreateEntity()
	{
		return mScene.CreateEntity();
	}

	necs::Entity GameWorld::CreateEntity(necs::Entity& parent)
	{
		// parent with heirarchy
		return CreateEntity();
	}

}
