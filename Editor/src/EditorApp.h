#pragma once

#include <NextEngine.h>

class Editor : public nxt::AppInterface
{
public:
	virtual void OnUpdate(double& dt) override;
	virtual void OnEvent(nxt::events::Event& ev) override;
};
