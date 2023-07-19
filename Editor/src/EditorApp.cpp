
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
		
		necs::Entity e{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(e, {glm::vec3{0.f, -2.f, 0.f}});
		mWorld.Attach<cmp::WorldModel>(e, modelInstance);

		necs::Entity e2{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(e2, { glm::vec3{0.f, -0.6f, 0.f}, glm::vec3{0.f}, glm::vec3{20.f, 0.1f, 20.f} });
		mWorld.Attach<cmp::WorldModel>(e2, modelInstance);

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

