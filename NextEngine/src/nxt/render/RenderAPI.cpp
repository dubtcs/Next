
#include "RenderAPI.h"

#include <nxt/core/device/Device.h>
#include <glad/glad.h>

#include <iostream>

namespace nxt::render::command
{

	void Init()
	{
		gladLoadGL();
	}

	void SetViewport(int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void Clear(int32_t buffer_bits)
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	// Use device::SwapBuffers instead
	void SwapBuffers()
	{
		device::SwapBuffers();
	}

}
