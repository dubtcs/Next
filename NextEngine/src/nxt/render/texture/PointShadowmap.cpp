
#include "PointShadowmap.h"

#include "TextureEnums.h"

#include <glad/glad.h>

static constexpr uint32_t gShadowResolution{ 1024U };

namespace nxt
{

	PointShadowmap::PointShadowmap()
	{

		glCreateFramebuffers(1, &mBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, mBuffer);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mID);

		for (int32_t i{ 0 }; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, nxtTextureFormatInternal_Depth24Stencil8, gShadowResolution, gShadowResolution, 
				0, nxtTextureFormat_DepthStencil, nxtDataType_Float, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mID, 0);

	}

	void PointShadowmap::BindDepth() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mBuffer);
	}

	void PointShadowmap::Bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
	}

	void PointShadowmap::Unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void PointShadowmap::UnbindDepth()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	PointShadowmap::~PointShadowmap()
	{
		glDeleteTextures(1, &mID);
		glDeleteBuffers(1, &mBuffer);
		//glDeleteTextures(6, &mSubtextures.at(0));
	}

}
