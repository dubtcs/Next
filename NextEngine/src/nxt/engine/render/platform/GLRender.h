#pragma once

#include <nxt/engine/EngineCore.h>
#include "../Renderer.h"

namespace nxt
{

	class NXT_API GLRender : public Renderer
	{
	public:
		virtual bool Init() override;
		virtual void Release() override;
		virtual void AddBuffer() override;
		virtual void AddShader() override;
		virtual void AddTexture() override;
		virtual ~GLRender() override;
		GLRender();
	};

}
