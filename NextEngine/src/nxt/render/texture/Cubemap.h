#pragma once

#include <nxt/EngineCore.h>

#include "TextureEnums.h"

#include <filesystem>

namespace nxt
{

	class NXT_API Cubemap
	{
	public:
		// Cubemap textures must be supplied in the order of {RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK}
		static Shared<Cubemap> Create(const std::initializer_list<std::filesystem::path>& texture_locations);
		// Cubemap textures must be supplied in the order of {RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK}
		Cubemap(const std::initializer_list<std::filesystem::path>& texture_locations);
		void Bind() const;
		static void Unbind();
	protected:
		uint32_t mID;
	};

	using SCubemap = Shared<Cubemap>;

}
