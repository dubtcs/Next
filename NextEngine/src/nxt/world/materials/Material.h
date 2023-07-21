#pragma once

#include <nxt/EngineCore.h>

#include "PBRProperties.h"

#include <nxt/render/texture/Texture.h>
#include <glm/glm.hpp>

namespace nxt
{

	class NXT_API Material
	{
	public:
		Material();
		Material(const PBRProperties& properties, const PBRTextures& textures);
		~Material();
	public:
		PBRProperties Properties;
		PBRTextures Textures;
	};

	using SMaterial = Shared<Material>;

}
