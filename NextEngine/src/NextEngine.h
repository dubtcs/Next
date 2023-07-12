#pragma once

/*

All functionaility is in namespace nxt

*/

// all includes to use the engine

#include <nxt/EngineCore.h>

#include <nxt/core/app/App.h>
#include <nxt/core/app/Interface.h>
#include <nxt/core/log/Log.h>
#include <nxt/core/input/Input.h>
#include <nxt/core/events/Event.h>
#include <nxt/core/clock/Clock.h>
#include <nxt/core/random/random.h>

#include <nxt/render/RenderAPI.h>
#include <nxt/render/buffers/VertexBuffer.h>
#include <nxt/render/buffers/ElementBuffer.h>
#include <nxt/render/buffers/DataBuffer.h>
#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/FrameBuffer.h>

#include <nxt/render/texture/Texture.h>
#include <nxt/render/texture/Cubemap.h>
#include <nxt/render/shader/Shader.h>

#include <nxt/world/World.h>
#include <nxt/world/Model.h>
#include <nxt/world/components/all.h>
