#pragma once

#include <nxt/EngineCore.h>

#include "nxtid.h"

#include <necs/include.h>

namespace nxt
{

	// All game objects have a GraphData REQUIRED

	struct NXT_API EntityData
	{
		necs::Entity entity;
		nxtid id;
	};

	class NXT_API World
	{
	public:
		World();
		World(const nxtid& worldRootID);
	protected:
		void CreateRootNode(const nxtid& id);
	public:
		necs::Entity CreateEntity();
		necs::Entity CreateEntity(const necs::Entity& parent);
		necs::Entity CreateEntityWithUUID(const necs::Entity& parent, const nxtid& id);

		// Load an existing entity from an axt scene file
		necs::Entity LoadEntity(const nxtid& parent, const nxtid& id);

		void DestroyEntity(const necs::Entity& id);

		void SetActiveCamera(const necs::Entity& id) { mActiveCamera = id; }

		necs::Entity GetEntityFromUUID(const nxtid& id) { return mMap[id]; }
		necs::Entity GetActiveCamera() const { return mActiveCamera; }
		const necs::Entity& GetWorldRoot() { return mWorldRoot; }
		Shared<necs::Scene>& GetScene() { return mScene; }
	protected:
		Shared<necs::Scene> mScene;
		necs::Entity mWorldRoot;
		necs::Entity mActiveCamera;

		bool mRootCreated{ false };

		using IDMap = std::unordered_map<nxtid, necs::Entity>;
		IDMap mMap{};
	public:

		// Attach a component to an entity
		template<typename T>
		T& Attach(const necs::Entity& entity)
		{
			return mScene->Attach<T>(entity);
		}


		/*
		@brief Attach a component to an entity
		@param entity: Entity id
		@param other: object of type T to set as initial value
		*/
		template<typename T>
		T& Attach(const necs::Entity& entity, const T& other)
		{
			return mScene->Attach<T>(entity, other);
		}


		/*
		@brief Attach a component to an entity
		@param entity: Entity id
		@param initializer_list: Initializer list of type T to set as initial value
		*/
		template<typename T>
		T& Attach(const necs::Entity& entity, std::initializer_list<T>&& initializer_list)
		{
			return mScene->Attach<T>(entity, std::forward<std::initializer_list<T>>(initializer_list));
		}


		// Removes a component instance from the entity
		template<typename T>
		void Detach(const necs::Entity& entity)
		{
			return mScene->Detach<T>(entity);
		}


		// Check if an entity has a component attached
		template<typename T>
		bool HasComponent(const necs::Entity& entity)
		{
			return mScene->HasComponent<T>(entity);
		}


		/*
		@brief Get the component data attached to the entity
		@brief Does *NOT* check if component is attached. Use HasComponent<T>(entity) before calling this
		*/
		template<typename T>
		T& GetComponent(const necs::Entity& entity)
		{
			return mScene->GetComponent<T>(entity);
		}

	};

	class NXT_API WorldEntity
	{
	public:
		WorldEntity(Shared<World>& world) : mID{ world->CreateEntity() }, mWorld{ world } {}
		~WorldEntity();
		//WorldEntity(necs::Entity& id) : mID{ id } {}
		//WorldEntity(const nxtid& guid);
	public:
		template<typename T>
		T& Attach()
		{
			return mWorld->Attach<T>(mID);
		}
		template<typename T>
		T& Attach(const T& other)
		{
			return mWorld->Attach<T>(mID, other);
		}
		template<typename T>
		T& Attach(std::initializer_list<T>&& initializer)
		{
			return mWorld->Attach<T>(mID, initializer);
		}

		template<typename T>
		void Detach()
		{
			return mWorld->Detach<T>(mID);
		}

		template<typename T>
		bool HasComponent()
		{
			return mWorld->HasComponent<T>(mID);
		}

		template<typename T>
		T& GetComponent()
		{
			return mWorld->GetComponent(mID);
		}
	protected:
		necs::Entity mID;
		Shared<World> mWorld;
	};

	using SWorld = Shared<World>;

}
