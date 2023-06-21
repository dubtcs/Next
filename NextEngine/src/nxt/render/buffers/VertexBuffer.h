#pragma once

#include <nxt/EngineCore.h>

namespace nxt::render
{

	// Draw Mode Enums from OpenGL
	enum BUFFERUSAGE_ : uint32_t
	{
		BUFFERUSAGE_STATIC = 0x88E4,
		BUFFERUSAGE_STREAM = 0x88E0,
		BUFFERUSAGE_DYNAMIC = 0x88E8
	};

	enum DATATYPE_ : uint32_t
	{
		DATATYPE_BYTE = 0x1400, // Byte
		DATATYPE_UBYTE = 0x1401, // Unsigned Byte
		DATATYPE_SHORT = 0x1402, // Short
		DATATYPE_USHORT = 0x1403, // Unsigned Short
		DATATYPE_INT = 0x1404, // Integer
		DATATYPE_UINT = 0x1405, // Unsigned Integer
		DATATYPE_FLOAT = 0x1406, // Float
	};

	enum DRAWMODE_ : uint32_t
	{
		DRAWMODE_POINTS = 0x0000,
		DRAWMODE_LINES = 0x0001,
		DRAWMODE_LINE_LOOP = 0x0002,
		DRAWMODE_LINE_STRIP = 0x0003,
		DRAWMODE_TRIANGLES = 0x0004,
		DRAWMODE_TRIANGLE_STRIP = 0x0005,
		DRAWMODE_TRIANGLE_FAN = 0x0006,
		DRAWMODE_QUADS = 0x0007
	};

	// make this abstract if/when adding Vulkan/DX
	class NXT_API VertexBuffer
	{
	public:
		static Shared<VertexBuffer> Create(size_t byteSize, BUFFERUSAGE_ drawMode, void* data = nullptr);
		void Draw(DRAWMODE_ mode = DRAWMODE_TRIANGLES) const;
		bool Bind() const;
		bool Unbind() const;
		void SetLayoutPosition(uint32_t position, uint32_t amount, DATATYPE_ dataType, uint32_t stride = 0, uint32_t offset = 0, bool normalized = false);
		VertexBuffer(size_t size, BUFFERUSAGE_ drawMode, void* data = nullptr);
		~VertexBuffer();
	protected:
		uint32_t mID;
		uint32_t mVertexArrayId; // GL SPECIFIC >:(
	};

}
