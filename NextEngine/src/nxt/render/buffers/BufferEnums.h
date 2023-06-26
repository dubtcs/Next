#pragma once

#include <nxt/EngineCore.h>
#include <nxt/core/GenericEnums.h>

namespace nxt::buffers
{

	enum BUFFER_USAGE_ : nxt_enum
	{
		BUFFER_USAGE_STATIC = 0x88E4, // GL_STATIC_DRAW
		BUFFER_USAGE_STREAM = 0x88E0,
		BUFFER_USAGE_DYNAMIC = 0x88E8
	};

	enum DRAW_MODE_ : nxt_enum
	{
		DRAW_MODE_POINTS = 0x0000,
		DRAW_MODE_LINES = 0x0001,
		DRAW_MODE_LINE_LOOP = 0x0002,
		DRAW_MODE_LINE_STRIP = 0x0003,
		DRAW_MODE_TRIANGLES = 0x0004,
		DRAW_MODE_TRIANGLE_STRIP = 0x0005,
		DRAW_MODE_TRIANGLE_FAN = 0x0006,
		DRAW_MODE_QUADS = 0x0007
	};

}
