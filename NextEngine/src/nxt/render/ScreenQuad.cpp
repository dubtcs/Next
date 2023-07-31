
#include "ScreenQuad.h"

#include "RenderAPI.h"

namespace nxt
{

	ScreenQuad::ScreenQuad() :
		mArrayObject{ buffers::ArrayObject::Create() },
		mShader{ new Shader{"assets/shaders/hdr/hdr.vert", "assets/shaders/hdr/hdr.frag"} }
	{
		using namespace buffers;
		mArrayObject->Bind();
		mShader->SetValue("hdr", 0);
		
		float verts[12]
		{
			-1.f, -1.f, 0.f,
			1.f, -1.f, 0.f,
			1.f, 1.f, 0.f,
			-1.f, 1.f, 0.f
		};
		SDataBuffer db{ DataBuffer::Create(sizeof(verts), verts, nxtBufferTarget_ArrayBuffer)};
		mArrayObject->SetLayoutPosition(0, 3, nxtDataType_Float, 12);

		uint32_t ind[6]
		{
			0, 1, 2,
			0, 2, 3
		};
		SDataBuffer db2{ DataBuffer::Create(sizeof(ind), ind, nxtBufferTarget_ElementArray)};

		mArrayObject->Unbind();
	}

	void ScreenQuad::Draw() const
	{
		render::command::Clear();
		mArrayObject->Bind();
		mShader->Bind();
		render::command::DrawElements(nxtDrawMode_Triangles, 6, nxtDataType_UInt);
	}

}
