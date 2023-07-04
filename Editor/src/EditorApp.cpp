
#include "EditorApp.h"

#include <necs/include.h>

namespace nxt
{

	Editor::Editor()
	{
		//Model m{ "assets/models/Triangle.gltf" };
		necs::Scene scene{};
		necs::Entity e1{ scene.CreateEntity() };
		scene.Attach<int>(e1);
		if (scene.HasComponent<int>(e1))
		{
			NXT_LOG_TRACE("BRUH!");
		}
	}

	void Editor::OnUpdate(float& dt)
	{
		mRender.OnUpdate(dt);
	}

	bool Editor::OnEvent(nxt::events::Event& ev)
	{
		events::Handler handler{ ev };
		handler.Fire<events::KeyboardPressed>(NXT_CALLBACK(Editor::OnKeyPressed));
		handler.Fire<events::MouseButtonPressed>(NXT_CALLBACK(Editor::OnMouseButtonPressed));
		handler.Fire<events::MouseButtonReleased>(NXT_CALLBACK(Editor::OnMouseButtonReleased));
		mRender.OnEvent(ev);
		return false;
	}

	bool Editor::OnKeyPressed(events::KeyboardPressed& ev)
	{

		return false;
	}

	bool Editor::OnMouseButtonPressed(events::MouseButtonPressed& ev)
	{
		//NXT_LOG_TRACE("MouseButton Pressed. Double click: {0}", ev.IsDoubleClick ? "yes" : "no");

		//float rep[]{ 0.25f, 0.9f, 0.f };
		//gVB->SetBufferSubData(sizeof(rep), 0, &rep);
		return false;
	}

	bool Editor::OnMouseButtonReleased(events::MouseButtonReleased& ev)
	{

		return false;
	}

}

