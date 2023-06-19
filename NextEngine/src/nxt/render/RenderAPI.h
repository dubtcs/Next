#pragma once

#include <nxt/EngineCore.h>

namespace nxt::render
{

	namespace api
	{

		void Init();
		void SetViewport(int32_t width, int32_t height);
		void SetClearColor(float red, float green, float blue, float alpha = 1.f);
		void Clear(int32_t buffer_bits = 0);

		// Use device::SwapBuffers instead
		void SwapBuffers();

	}

}
