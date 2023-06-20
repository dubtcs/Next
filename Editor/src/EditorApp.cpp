
#include "EditorApp.h"

#include <nxt/render/RenderAPI.h>

namespace nxt
{

	void Editor::OnUpdate(double& dt)
	{
		render::api::Clear();
		NXT_LOG_TRACE("Framerate: {0}", static_cast<int32_t>((1.0 / dt)));
	}

	void Editor::OnEvent(nxt::events::Event& ev)
	{
		NXT_LOG_INFO("Event Received: {0}", ev.GetName());
	}

}

