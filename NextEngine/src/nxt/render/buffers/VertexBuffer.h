#pragma once

#include <nxt/EngineCore.h>

namespace nxt::render
{

	// make this abstract if/when adding Vulkan/DX
	class NXT_API VertexBuffer
	{
	public:
		static Shared<VertexBuffer> Create(uint32_t size, float* vertices = nullptr);
		bool Bind() const;
		bool Unbind() const;
		bool AddChildBuffer(Shared<VertexBuffer>& otherVertexBuffer) const;
		VertexBuffer(uint32_t size, float* vertices);
		~VertexBuffer();
	protected:
		uint32_t mId;
		uint32_t mVertexArrayId; // GL SPECIFIC >:(
	};

}
