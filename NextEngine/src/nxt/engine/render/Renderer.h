#pragma once

#include <nxt/engine/EngineCore.h>

namespace nxt
{

	class NXT_API Renderer
	{
	public:
		virtual bool Init() = 0;
		virtual void Release() = 0;
		virtual void AddBuffer() = 0;
		virtual void AddShader() = 0;
		virtual void AddTexture() = 0;
		virtual ~Renderer() {};
	};

}
