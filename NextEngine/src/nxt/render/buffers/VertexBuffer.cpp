
#include "VertexBuffer.h"

#include <nxt/core/log/Log.h>
#include <glad/glad.h>

namespace nxt::render
{

	Shared<VertexBuffer> VertexBuffer::Create(size_t byteSize, BUFFERUSAGE_ glTarget, void* data)
	{
		return NewShared<VertexBuffer>(byteSize, glTarget, data);
	}

	// void* switch to std::any??
	VertexBuffer::VertexBuffer(size_t byteSize, BUFFERUSAGE_ drawMode, void* data)
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

	void VertexBuffer::SetLayoutPosition(uint32_t position, uint32_t amount, DATATYPE_ dataType, uint32_t stride, uint32_t offset, bool normalized)
	{
		Bind();
		if (dataType & DATATYPE_FLOAT)
		{
			NXT_LOG_TRACE("HUH");
			glVertexAttribPointer(position, amount, dataType, (normalized) ? GL_TRUE : GL_FALSE, stride, (void*)offset);
			glEnableVertexAttribArray(position);
		}
	}

	void VertexBuffer::Draw(DRAWMODE_ mode) const
	{
		Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
