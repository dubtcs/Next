
#include "Material.h"

namespace nxt
{

	Material::Material()
	{

	}

	Material::Material(const PBRProperties& props, const PBRTextures& tex):
		Properties{ props },
		Textures{ tex }
	{

	}

	Material::~Material()
	{

	}

}
