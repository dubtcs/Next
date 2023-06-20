
#include "EditorApp.h"

void Editor::OnUpdate(double& dt)
{
	NXT_LOG_TRACE("Framerate: {0}", static_cast<int32_t>((1.0 / dt)));
}

void Editor::OnEvent(nxt::events::Event& ev)
{
	NXT_LOG_INFO("Event Received: {0}", ev.GetName());
}
