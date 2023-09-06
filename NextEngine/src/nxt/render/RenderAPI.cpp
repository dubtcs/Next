
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
		glEnable(GL_MULTISAMPLE);
		glDepthFunc(GL_LEQUAL);
		glLineWidth(5.f);
	}

	void SetViewport(int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void Clear(uint32_t bufferBits)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(bufferBits);
	}

	void SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void DrawArrays(nxtDrawMode mode, uint32_t count, uint32_t firstItem)
	{
		glDrawArrays(mode, firstItem, count);
	}

	void DrawElements(nxtDrawMode mode, uint32_t count, nxtDataType dataType, void* offset)
	{
		glDrawElements(mode, count, dataType, offset);
	}

	void DrawInstances(nxtDrawMode mode, uint32_t count, uint32_t instanceCount, nxtDataType dataType, void* offset)
	{
		glDrawElementsInstanced(mode, count, dataType, offset, instanceCount);
	}

	void SetFaceCullingMode(nxtCullingMode mode)
	{
		glCullFace(mode);
	}

	void SetRenderFeature(nxtRenderFeature feature, bool toggle)
	{
		toggle ? glEnable(feature) : glDisable(feature);
	}

	nxt_enum GrabError()
	{
		return glGetError();
	}

	void SwapBuffers()
	{
		device::SwapBuffers();
	}

}
