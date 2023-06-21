
#include "ElementBuffer.h"

#include <glad/glad.h>

namespace nxt::buffers
{

	Shared<ElementBuffer> ElementBuffer::Create(size_t byteSize, uint32_t* indices, BUFFER_USAGE_ usage)
	{
		return NewShared<ElementBuffer>(byteSize, indices, usage);
	}

	ElementBuffer::ElementBuffer(size_t byteSize, uint32_t* indices, BUFFER_USAGE_ usage)
	{
		glCreateBuffers(1, &mID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, indices, usage);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	ElementBuffer::~ElementBuffer()
	{
		glDeleteBuffers(1, &mID);
	}

	bool ElementBuffer::AddVertexBuffer(Shared<VertexBuffer>& buffer)
	{
		if (buffer != nullptr)
		{
			mVertexBuffer = buffer;
			return true;
		}
		return false;
	}

	void ElementBuffer::Bind() const
	{
		if (mVertexBuffer)
		{
			mVertexBuffer->Bind();
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
	}

	void ElementBuffer::Draw(DRAW_MODE_ mode, uint32_t count, uint32_t offset) const
	{
		Bind();
		glDrawElements(mode, count, DATA_TYPE_UINT, (void*)offset);
	}

}
