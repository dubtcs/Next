#pragma once

#include <nxt/EngineCore.h>
#include <nxt/render/buffers/ArrayObject.h>

namespace nxt
{

	// Direct RenderAPI calls
	// eg. Set the viewport, clear color, clear the screen...

	enum nxtCullingMode : nxt_enum
	{
		nxtCullingMode_Front = 0x0404,
		nxtCullingMode_Back = 0x0405,
		nxtCullingMode_FrontBack = 0x0408
	};

	enum nxtRenderFeature : nxt_enum
	{
		nxtRenderFeature_DepthTest = 0x0B71,
		nxtRenderFeature_CullFace = 0x0B44,
	};

	namespace render::command
	{

		void Init();
		void SetViewport(int32_t width, int32_t height);
		void SetClearColor(float red, float green, float blue, float alpha = 1.f);
		void Clear(int32_t buffer_bits = 0);

		void DrawElements(nxtDrawMode mode, uint32_t count, nxtDataType dataType = nxtDataType_Float, void* offset = nullptr);

		void SetRenderFeature(nxtRenderFeature feature, bool toggle);
		void SetFaceCullingMode(nxtCullingMode mode);

		void SwapBuffers();

	}

}
