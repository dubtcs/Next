#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/texture/Texture.h>
#include <glm/glm.hpp>

namespace nxt
{

	// Should TEXTURE be switched to Shared<> to make sure they aren't deleted mid use or somn??

	struct NXT_API PBRColorProperties
	{
		glm::vec4 BaseColor{ 1.f };
		int32_t Texture{ -1 };
	};

	struct NXT_API PBRRoughnessProperties
	{
		float Value{ 0.f };
		int32_t Texture{ -1 };
	};

	struct NXT_API PBRMetallicProperties
	{
		float Value{ 0.f };
		int32_t Texture{ -1 };
	};

	struct NXT_API PBRProperties
	{
		PBRColorProperties Color;
		PBRRoughnessProperties Roughness;
		PBRMetallicProperties Metallic;
	};

	// Emissive, Occlusion, Normal
	struct NXT_API PBRTextures
	{
		int32_t Emissive{ -1 };
		int32_t Occlusion{ -1 };
		int32_t Normal{ -1 };
	};

}
