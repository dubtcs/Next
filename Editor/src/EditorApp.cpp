
#include "EditorApp.h"

namespace nxt
{

	Editor::Editor()
	{
		
	}

	void Editor::OnUpdate(float& dt)
	{
		mRender.OnUpdate(dt);
	}

	bool Editor::OnEvent(nxt::events::Event& ev)
	{
		events::Handler handler{ ev };
		NXT_LOG_TRACE(ev.GetName());
		handler.Fire<events::KeyboardPressed>(NXT_CALLBACK(Editor::OnKeyPressed));
		handler.Fire<events::MouseButtonPressed>(NXT_CALLBACK(Editor::OnMouseButtonPressed));
		//handler.Fire<events::WindowResized>(NXT_CALLBACK(Editor::OnWindowResize));
		mRender.OnEvent(ev);
		return false;
	}

	bool Editor::OnKeyPressed(events::KeyboardPressed& ev)
	{
		if (ev.Keycode == input::KEYCODE_MOUSE_LEFT)
		{
			NXT_LOG_CRIT("BRUH!");
		}
		return false;
	}

	bool Editor::OnMouseButtonPressed(events::MouseButtonPressed& ev)
	{
		NXT_LOG_TRACE("MouseButton Pressed. Double click: {0}", ev.IsDoubleClick ? "yes" : "no");
		//float rep[]{ 0.25f, 0.9f, 0.f };
		//gVB->SetBufferSubData(sizeof(rep), 0, &rep);
		return false;
	}
	//bool Editor::OnWindowResize(events::WindowResized& ev)
	//{
	//	//return mRender.OnWindowResize(ev);
	//}

}

