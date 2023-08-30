#pragma once

#include <nxt/EngineCore.h>

#include "buffers/ArrayObject.h"
#include "buffers/DataBuffer.h"
#include "shader/Shader.h"

namespace nxt
{

	// Used to draw buffers to screen with a basic quad
	class NXT_API ScreenQuad
	{
	public:
		ScreenQuad();
		void Draw() const;
		void DrawNoShader() const;

		void toggleao(int32_t value) const;
	protected:
		buffers::SArrayObject mArrayObject;
		SShader mShader;
	};
	using SScreenQuad = Shared<ScreenQuad>;

}
