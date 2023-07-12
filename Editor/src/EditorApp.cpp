
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
		for (float i{ 0 }; i < 5; i++)
		{
			necs::Entity e{ mWorld.CreateEntity() };
			mWorld.Attach<cmp::WorldModel>(e, modelInstance);
			mWorld.Attach<cmp::Transform>(e, { glm::vec3{ i } });
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

