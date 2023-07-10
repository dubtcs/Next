
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

	void DataBuffer::CopyBufferData(SDataBuffer other, size_t readOffset, size_t writeOffset, size_t byteSize)
	{
		glBindBuffer(GL_COPY_READ_BUFFER, other->mID);
		glBindBuffer(GL_COPY_WRITE_BUFFER, mID);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, byteSize);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
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

	void DataBuffer::BindIndexed(int32_t index) const
	{
		glBindBufferBase(mTarget, index, mID);
	}

	void DataBuffer::Unbind() const
	{
		glBindBuffer(mTarget, 0);
	}

}
