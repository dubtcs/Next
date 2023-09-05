#pragma once

#include <nxt/EngineCore.h>
#include <nxt/core/GenericEnums.h>

namespace nxt
{

	enum nxtBufferUsage : nxt_enum
	{
		nxtBufferUsage_Static = 0x88E4, // GL_STATIC_DRAW
		nxtBufferUsage_Stream = 0x88E0,
		nxtBufferUsage_Dynamic = 0x88E8
	};

	enum nxtDrawMode : nxt_enum
	{
		nxtDrawMode_Points = 0x0000,
		nxtDrawMode_Lines = 0x0001,
		nxtDrawMode_LineLoop = 0x0002,
		nxtDrawMode_LineStrip = 0x0003,
		nxtDrawMode_Triangles = 0x0004,
		nxtDrawMode_TriangleStrip = 0x0005,
		nxtDrawMode_TriangleFan = 0x0006,
		nxtDrawMode_Quads = 0x0007
	};

	enum nxtBufferTarget : nxt_enum
	{
		nxtBufferTarget_ArrayBuffer = 0x8892,
		nxtBufferTarget_ElementArray = 0x8893,
		nxtBufferTarget_UniformBuffer = 0x8A11,
		nxtBufferTarget_CopyRead = 0x8F36,
		nxtBufferTarget_CopyWrite = 0x8F37
	};

	enum nxtBufferAccess : nxt_enum
	{
		nxtBufferAccess_Read = 0x0001,
		nxtBufferAccess_Write = 0x0002,
	};

}
