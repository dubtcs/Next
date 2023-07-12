#pragma once

#include <nxt/EngineCore.h>
#include <nxt/render/buffers/ArrayObject.h>

namespace nxt
{

	// Direct RenderAPI calls
	// eg. Set the viewport, clear color, clear the screen...

	enum nxtBufferBit : nxt_enum
	{
		nxtBufferBit_Depth = 0x00000100,
		nxtBufferBit_Stencil = 0x00000400,
		nxtBufferBit_Color = 0x00004000
	};

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
		nxtRenderFeature_Multisample = 0x809D,
		nxtRenderFeature_PointSize = 0x8642,
	};

	namespace render::command
	{

		void Init();
		void SetViewport(int32_t width, int32_t height);
		void SetClearColor(float red, float green, float blue, float alpha = 1.f);
		void Clear(uint32_t buffer_bits = nxtBufferBit_Color | nxtBufferBit_Depth);

		void DrawElements(nxtDrawMode mode, uint32_t count, nxtDataType dataType = nxtDataType_Float, void* offset = nullptr);
		void DrawInstances(nxtDrawMode mode, uint32_t count, uint32_t instanceCount, nxtDataType dataType = nxtDataType_Float, void* offset = nullptr);

		void SetRenderFeature(nxtRenderFeature feature, bool toggle);
		void SetFaceCullingMode(nxtCullingMode mode);

		void SwapBuffers();

	}

}
