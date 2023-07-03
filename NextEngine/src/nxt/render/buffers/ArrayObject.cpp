
#include "ArrayObject.h"

#include <nxt/core/log/Log.h>
#include <glad/glad.h>

namespace nxt::buffers
{

	Shared<ArrayObject> ArrayObject::Create()
	{
		return NewShared<ArrayObject>();
	}

	ArrayObject::ArrayObject()
	{
		glCreateVertexArrays(1, &mID);
	}

	ArrayObject::~ArrayObject()
	{
		glDeleteVertexArrays(1, &mID);
	}

	void ArrayObject::SetLayoutPosition(uint32_t index, uint32_t amount, nxtDataType dataType, uint32_t byteStride, uint32_t byteOffset, bool normalized)
	{
		Bind();
		switch (dataType)
		{
			case(nxtDataType_Float):
			{
				glVertexAttribPointer(index, amount, dataType, (normalized) ? GL_TRUE : GL_FALSE, byteStride, (void*)byteOffset);
				glEnableVertexAttribArray(index);
				break;
			}
			default:
				NXT_LOG_WARN("Data type not supported: {0}", static_cast<uint32_t>(dataType));
				break;
		}
	}

	void ArrayObject::Bind() const
	{
		glBindVertexArray(mID);
	}

	// Static
	void ArrayObject::Unbind()
	{
		glBindVertexArray(0);
	}

}
