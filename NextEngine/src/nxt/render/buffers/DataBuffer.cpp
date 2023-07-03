
#include "DataBuffer.h"

#include <nxt/core/log/Log.h>
#include <glad/glad.h>

namespace nxt::buffers
{

	Shared<DataBuffer> DataBuffer::Create(size_t byteSize, void* data,
		nxtBufferTarget bufferTarget, nxtBufferUsage bufferUsage)
	{
		return NewShared<DataBuffer>(byteSize, data, bufferTarget, bufferUsage);
	}

	DataBuffer::DataBuffer(size_t byteSize, void* data,
		nxtBufferTarget bufferTarget, nxtBufferUsage bufferUsage) :
		mTarget{ bufferTarget }
	{
		glCreateBuffers(1, &mID);
		glBindBuffer(mTarget, mID);
		glBufferData(mTarget, byteSize, data, bufferUsage);
	}

	void DataBuffer::SetSubData(size_t byteSize, size_t byteOffset, void* data)
	{
		Bind();
		glBufferSubData(mTarget, byteOffset, byteSize, data);
	}

	void DataBuffer::Draw(nxtDrawMode drawMode, uint32_t count, uint32_t offset, nxtDataType dataType) const
	{
		if (mTarget == nxtBufferTarget_ArrayBuffer)
		{
			glDrawArrays(drawMode, offset, count);
		}
		else
		{
			glDrawElements(drawMode, count, dataType, (void*)offset);
		}
	}

	const nxtBufferTarget& DataBuffer::GetTarget() const
	{
		return mTarget;
	}

	DataBuffer::~DataBuffer()
	{
		glDeleteBuffers(1, &mID);
	}

	void DataBuffer::Bind() const
	{
		glBindBuffer(mTarget, mID);
	}

	void DataBuffer::Unbind() const
	{
		glBindBuffer(mTarget, 0);
	}

}
