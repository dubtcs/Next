
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
		mTarget{ bufferTarget },
		mByteSize{ byteSize }
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

	void DataBuffer::CopyBufferData(SDataBuffer readBuffer, size_t readOffset, size_t writeOffset, size_t byteSize)
	{
		glCopyNamedBufferSubData(readBuffer->mID, mID, readOffset, writeOffset, byteSize);
	}

	BufferMapPtr::BufferMapPtr(nxtBufferTarget target, size_t byteOffset, size_t byteSize, nxt_enum usage) :
		mTarget{ target }
	{
		mPtr = glMapBufferRange(mTarget, byteOffset, byteSize, usage);
	}

	BufferMapPtr::~BufferMapPtr()
	{
		glUnmapBuffer(mTarget);
	}

	void* BufferMapPtr::Get()
	{
		return mPtr;
	}

	void* DataBuffer::MapRange(size_t byteOffset, size_t byteSize, nxt_enum usage)
	{
		// Must bind buffer before usage
		return glMapBufferRange(mTarget, byteOffset, byteSize, usage);
	}

	void DataBuffer::Unmap()
	{
		glUnmapBuffer(mTarget);
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
