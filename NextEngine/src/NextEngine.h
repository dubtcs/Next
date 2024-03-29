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
#include <nxt/core/utility/random.h>
#include <nxt/core/utility/lerp.h>
#include <nxt/core/utility/TypeConversions.h>
#include <nxt/core/utility/TypeToSize.h>

#include <nxt/render/RenderAPI.h>
#include <nxt/render/buffers/VertexBuffer.h>
#include <nxt/render/buffers/ElementBuffer.h>
#include <nxt/render/buffers/DataBuffer.h>
#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/FrameBuffer.h>

#include <nxt/render/texture/Texture.h>
#include <nxt/render/texture/FrameTexture.h>
#include <nxt/render/texture/Cubemap.h>
#include <nxt/render/shader/Shader.h>

#include <nxt/world/World.h>
#include <nxt/world/env/Skybox.h>
#include <nxt/world/model/Model2.h>
#include <nxt/world/model/Animation.h>
#include <nxt/world/model/Primitive.h>
#include <nxt/world/components/all.h>
