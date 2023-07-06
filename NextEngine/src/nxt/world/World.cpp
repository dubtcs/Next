
#include "World.h"

#include <nxt/core/log/Log.h>
#include "components/all.h"

namespace nxt
{

	using namespace necs;

	void World::CreateRootNode(const nxtid& worldRootID)
	{
		if (!mRootCreated)
		{
			mRootCreated = true;
			mWorldRoot = mScene->CreateEntity();
			cmp::GraphData& gdata{ mScene->Attach<cmp::GraphData>(mWorldRoot) };
			gdata.ID = worldRootID;
			mMap.emplace(worldRootID, mWorldRoot);
		}
		else
		{
			NXT_LOG_WARN("Root node already created for this world");
		}
	}

	World::World() :
		mScene{ NewShared<Scene>() }
	{
		nxtid rootID{ GenerateID() };
		CreateRootNode(rootID);

		// Create a basic camera
		Entity camera{ CreateEntity() };
		mScene->Attach<cmp::Camera>(camera, (1920.f / 1080.f));
		mScene->Attach<cmp::Transform>(camera);
		mScene->Attach<cmp::Description>(camera, { "StarterCamera" });
		mActiveCamera = camera;

		Entity cube{ CreateEntity() };
		Attach<cmp::Sprite>(cube, { { 0.1f, 0.7f, 0.7f, 1.f } });
		Attach<cmp::Transform>(cube);
		Attach<cmp::Description>(cube, { "StarterEntity" });
	}

	World::World(const nxtid& worldRootID) :
		mScene{ NewShared<Scene>() }
	{
		CreateRootNode(worldRootID);
	}

	Entity World::CreateEntity()
	{
		return CreateEntity(mWorldRoot);
	}

	Entity World::CreateEntity(const Entity& parent)
	{
		nxtid id{ GenerateID() };
		return CreateEntityWithUUID(parent, id);
	}

	Entity World::CreateEntityWithUUID(const Entity& parent, const nxtid& id)
	{
		Entity e{ mScene->CreateEntity() };

		mMap.emplace(id, e);
		cmp::GraphData& pdata{ mScene->GetComponent<cmp::GraphData>(parent) };
		pdata.Children.push_back(id);

		mScene->Attach<cmp::GraphData>(e, cmp::GraphData{ pdata.ID, id });

		return e;
	}

	Entity World::LoadEntity(const nxtid& parent, const nxtid& id)
	{
		Entity e{ mScene->CreateEntity() };

		const Entity& pid{ GetEntityFromUUID(parent) };

		cmp::GraphData& pdata{ mScene->GetComponent<cmp::GraphData>(pid) };
		pdata.Children.push_back(id);

		mScene->Attach<cmp::GraphData>(e, cmp::GraphData{ parent, id });

		mMap.emplace(id, e);

		return e;
	}

	void World::DestroyEntity(const Entity& e)
	{
		if (mActiveCamera == e)
		{
			mActiveCamera = nil;
		}

		cmp::GraphData& gdata{ mScene->GetComponent<cmp::GraphData>(e) };
		for (nxtid& id : gdata.Children)
		{
			DestroyEntity(GetEntityFromUUID(id));
		}
		mMap.erase(gdata.ID);

		Entity pid{ GetEntityFromUUID(gdata.Parent) };
		cmp::GraphData& parent{ mScene->GetComponent<cmp::GraphData>(pid) };

		std::vector<nxtid>::iterator position{ std::find(parent.Children.begin(), parent.Children.end(), gdata.ID) };
		if (position != parent.Children.end())
		{
			parent.Children.erase(position);
		}

		mScene->DestroyEntity(e);
	}

	// Used for editor deletions where the iterator is known before the call
	/*void World::DestroyEntity(const Entity& e, const std::vector<Entity>::iterator& position)
	{

	}*/

}
