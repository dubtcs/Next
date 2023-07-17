
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

		necs::Entity e1{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(e1, { .Position{ glm::vec3{2.f, 0.75f, 1.f} }, .Scale{ glm::vec3{1.f} } });
		mWorld.Attach<cmp::WorldModel>(e1, modelInstance);
		
		necs::Entity e2{ mWorld.CreateEntity() };
		mWorld.Attach<cmp::Transform>(e2, { .Position{ glm::vec3{0.f} }, .Scale{ glm::vec3{ 15.f, 0.5f, 15.f } } });
		mWorld.Attach<cmp::WorldModel>(e2, modelInstance);

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

