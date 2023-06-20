
#include "VertexBuffer.h"

#include <glad/glad.h>

namespace nxt::render
{

	Shared<VertexBuffer> VertexBuffer::Create(size_t byteSize, uint32_t glTarget, void* data)
	{
		return NewShared<VertexBuffer>(byteSize, glTarget);
	}

	// void* switch to std::any??
	VertexBuffer::VertexBuffer(size_t byteSize, uint32_t drawMode, void* data)
	{
		glCreateVertexArrays(1, &mVertexArrayId);
		glCreateBuffers(1, &mID);
		glBindBuffer(GL_ARRAY_BUFFER, mID);
		glBufferData(GL_ARRAY_BUFFER, byteSize, data, drawMode);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &mID);
		glDeleteVertexArrays(1, &mVertexArrayId);
	}

	bool VertexBuffer::Bind() const
	{
		glBindVertexArray(mVertexArrayId);
		glBindBuffer(GL_ARRAY_BUFFER, mID);
		return true;
	}

	bool VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return true;
	}

}
