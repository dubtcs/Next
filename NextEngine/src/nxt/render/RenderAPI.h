#pragma once

#include <nxt/EngineCore.h>
#include <nxt/render/buffers/ArrayObject.h>

namespace nxt::render
{

	// Direct RenderAPI calls
	// eg. Set the viewport, clear color, clear the screen...
	namespace command
	{

		void Init();
		void SetViewport(int32_t width, int32_t height);
		void SetClearColor(float red, float green, float blue, float alpha = 1.f);
		void Clear(int32_t buffer_bits = 0);

		void SwapBuffers();

	}

}
