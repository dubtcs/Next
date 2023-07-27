#pragma once

#include <NextEngine.h>

#include "render/RenderSystem.h"

namespace nxt
{
	class Editor : public nxt::AppInterface
	{
	public:
		Editor();
		virtual void OnUpdate(float& dt, bool isFocused) override;
		virtual bool OnEvent(events::Event& ev) override;
	protected:
		bool OnKeyPressed(events::KeyboardPressed& ev);
		bool OnMouseButtonPressed(events::MouseButtonPressed& ev);
		bool OnMouseButtonReleased(events::MouseButtonReleased& ev);
		bool OnWindowResize(events::WindowResized& ev);
		bool OnDragReceived(events::DragFileReceived& ev);
	protected:
		RenderSystem mRender;
		World mWorld;
	};
}

