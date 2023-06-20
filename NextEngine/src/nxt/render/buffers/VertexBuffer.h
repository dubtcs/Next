#pragma once

#include <nxt/EngineCore.h>

namespace nxt::render
{

	// make this abstract if/when adding Vulkan/DX
	class NXT_API VertexBuffer
	{
	public:
		static Shared<VertexBuffer> Create(size_t byteSize, uint32_t drawMode, void* data = nullptr);
		bool Bind() const;
		bool Unbind() const;
		VertexBuffer(size_t size, uint32_t drawMode, void* data = nullptr);
		~VertexBuffer();
	protected:
		uint32_t mID;
		uint32_t mVertexArrayId; // GL SPECIFIC >:(
	};

}
