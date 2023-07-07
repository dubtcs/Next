
#include "RenderAPI.h"

#include <nxt/core/device/Device.h>

#include <glad/glad.h>
#include <iostream>

namespace nxt::render::command
{

	void Init()
	{
		gladLoadGL();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	void SetViewport(int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void Clear(int32_t buffer_bits)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void DrawElements(nxtDrawMode mode, uint32_t count, nxtDataType dataType, void* offset)
	{
		glDrawElements(mode, count, dataType, offset);
	}

	void SwapBuffers()
	{
		device::SwapBuffers();
	}

}
