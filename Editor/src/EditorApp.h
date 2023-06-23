#pragma once

#include <NextEngine.h>

namespace nxt
{
	class Editor : public nxt::AppInterface
	{
	public:
		Editor();
		virtual void OnUpdate(double& dt) override;
		virtual bool OnEvent(events::Event& ev) override;
	protected:
		bool OnKeyPressed(events::KeyboardPressed& ev);
		bool OnMouseButtonPressed(events::MouseButtonPressed& ev);
	};
}

