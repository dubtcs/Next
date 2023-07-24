
#include "EditorApp.h"

#include <necs/include.h>

#include <nxt/core/events/ScriptEvents.h>
#include <nxt/render/texture/Cubemap.h>

#include <nxt/core/events/ScriptEvents.h>

namespace nxt
{

	Editor::Editor() :
		mWorld{}
	{

		SModel modelInstance{ Model::Create("assets/models/BoxTextured.gltf") };
		SModel avo{ Model::Create("assets/models/Avocado.gltf") };
		
		necs::Entity e{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(e, {glm::vec3{0.f, -2.f, 0.f}});
		mWorld.Attach<cmp::WorldModel>(e, modelInstance);

		necs::Entity e2{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(e2, { glm::vec3{0.f, -0.6f, 0.f}, glm::vec3{0.f}, glm::vec3{20.f, 0.1f, 20.f} });
		mWorld.Attach<cmp::WorldModel>(e2, modelInstance);

		necs::Entity e3{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(e3, { glm::vec3{-9.5f, 2.5f, 0.f}, glm::vec3{0.f}, glm::vec3{0.5f, 5.f, 20.f} });
		mWorld.Attach<cmp::WorldModel>(e3, modelInstance);

		necs::Entity e4{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(e4, { glm::vec3{9.5f, 2.5f, 0.f}, glm::vec3{0.f}, glm::vec3{0.5f, 45.f, 20.f} });
		mWorld.Attach<cmp::WorldModel>(e4, modelInstance);

		necs::Entity eb{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(eb, { glm::vec3{ -2.f, 2.f, 0.f } });
		mWorld.Attach<cmp::WorldModel>(eb, modelInstance);

		necs::Entity ea{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(ea, { glm::vec3{ -5.f, 2.f, -1.f } });
		mWorld.Attach<cmp::WorldModel>(ea, modelInstance);

		necs::Entity ec{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(ec, { glm::vec3{3.f, 0.f, 0.f}, glm::vec3{0.f}, glm::vec3{25.f} });
		mWorld.Attach<cmp::WorldModel>(ec, avo);

		for (float i{ 0 }; i < 5; i++)
		{
			necs::Entity e3{ mWorld.CreateEntity() };
			mWorld.Attach<cmp::Transform>(e3, { glm::vec3{ i * 3.f } });
			mWorld.Attach<cmp::WorldModel>(e3, modelInstance);
		}

	}

	void Editor::OnUpdate(float& dt)
	{
		mRender.OnUpdate(dt, mWorld);
	}

	bool Editor::OnEvent(nxt::events::Event& ev)
	{
		//events::Handler handler{ ev };
		//handler.Fire<events::KeyboardPressed>(NXT_CALLBACK(Editor::OnKeyPressed));
		//handler.Fire<events::MouseButtonPressed>(NXT_CALLBACK(Editor::OnMouseButtonPressed));
		//handler.Fire<events::MouseButtonReleased>(NXT_CALLBACK(Editor::OnMouseButtonReleased));
		//handler.Fire<events::WindowResized>(NXT_CALLBACK(Editor::OnWindowResize));
		mRender.OnEvent(ev);
		return false;
	}

}

