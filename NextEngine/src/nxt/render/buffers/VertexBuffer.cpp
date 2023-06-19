
#include "VertexBuffer.h"

#include <glad/glad.h>

namespace nxt::render
{

	Shared<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
	{
		return NewShared<VertexBuffer>(size, vertices);
	}

	VertexBuffer::VertexBuffer(uint32_t size, float* vertices)
	{
		glCreateVertexArrays(1, &mVertexArrayId);
		glCreateBuffers(1, &mId);
		glBindBuffer(GL_ARRAY_BUFFER, mId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &mId);
		glDeleteVertexArrays(1, &mVertexArrayId);
	}

	bool VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mId);
		glBindVertexArray(mVertexArrayId);
		return true;
	}

	bool VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return true;
	}

}
