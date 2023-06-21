#pragma once

#include <NextEngine.h>

namespace nxt
{
	class Editor : public nxt::AppInterface
	{
	public:
		Editor();
		virtual void OnUpdate(double& dt) override;
		virtual void OnEvent(events::Event& ev) override;
	};
}

