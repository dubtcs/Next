
#include "ScreenQuad.h"

#include "RenderAPI.h"

namespace nxt
{

	ScreenQuad::ScreenQuad() :
		mArrayObject{ buffers::ArrayObject::Create() },
		mShader{ new Shader{"assets/shaders/screen/def.vert", "assets/shaders/screen/def.frag"} }
	{
		using namespace buffers;
		mArrayObject->Bind();
		mShader->Bind();
		std::vector<int32_t> ts{ 0, 1, 2 };
		mShader->SetArrayValue("gTextures", ts);
		
		float verts[20]
		{
			-1.f, -1.f, 0.f,	0.f, 0.f,
			1.f, -1.f, 0.f,		1.f, 0.f,
			1.f, 1.f, 0.f,		1.f, 1.f,
			-1.f, 1.f, 0.f,		0.f, 1.f
		};
		SDataBuffer db{ DataBuffer::Create(sizeof(verts), verts, nxtBufferTarget_ArrayBuffer) };
		mArrayObject->SetLayoutPosition(0, 3, nxtDataType_Float, 20);
		mArrayObject->SetLayoutPosition(3, 2, nxtDataType_Float, 20, 12);

		uint32_t ind[6]
		{
			0, 1, 2,
			0, 2, 3
		};
		SDataBuffer db2{ DataBuffer::Create(sizeof(ind), ind, nxtBufferTarget_ElementArray) };

		mArrayObject->Unbind();
	}

	void ScreenQuad::Draw() const
	{
		mShader->Bind();
		DrawNoShader();
	}

	void ScreenQuad::DrawNoShader() const
	{
		render::command::Clear();
		mArrayObject->Bind();
		render::command::DrawElements(nxtDrawMode_Triangles, 6, nxtDataType_UInt);
		mArrayObject->Unbind();
	}

}
