
#include "VertexBuffer.h"

#include <nxt/core/log/Log.h>
#include <glad/glad.h>

namespace nxt::buffers
{

	Shared<VertexBuffer> VertexBuffer::Create(size_t byteSize, void* data, BUFFER_USAGE_ bufferUsage)
	{
		return NewShared<VertexBuffer>(byteSize, data, bufferUsage);
	}

	// void* switch to std::any??
	VertexBuffer::VertexBuffer(size_t byteSize, void* data, BUFFER_USAGE_ bufferUsage)
	{
		glCreateVertexArrays(1, &mVertexArrayId);
		glCreateBuffers(1, &mID);
		glBindBuffer(GL_ARRAY_BUFFER, mID);
		glBufferData(GL_ARRAY_BUFFER, byteSize, data, bufferUsage);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &mID);
		glDeleteVertexArrays(1, &mVertexArrayId);
	}

	void VertexBuffer::SetBufferSubData(size_t byteSize, size_t byteOffset, void* data) const
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, byteOffset, byteSize, data);
	}

	void VertexBuffer::SetLayoutPosition(uint32_t position, uint32_t amount, DATA_TYPE_ dataType, uint32_t stride, uint32_t offset, bool normalized)
	{
		Bind();
		switch (dataType)
		{
			case(DATA_TYPE_FLOAT):
			{
				glVertexAttribPointer(position, amount, dataType, (normalized) ? GL_TRUE : GL_FALSE, stride, (void*)offset);
				glEnableVertexAttribArray(position);
				break;
			}
			default:
				break;
		}
	}

	void VertexBuffer::Draw(DRAW_MODE_ mode) const
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
