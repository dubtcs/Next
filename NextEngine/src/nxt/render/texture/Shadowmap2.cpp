
#include "Shadowmap2.h"

#include "TextureEnums.h"
#include <nxt/core/log/Log.h>

#include <nxt/render/RenderAPI.h>

#include <glad/glad.h>

namespace nxt
{

	Shadowmap2::Shadowmap2(const uint32_t& resolution) :
		mResolution{ resolution }
	{

		glCreateFramebuffers(1, &mBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mTextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

		for (uint32_t i{ 0 }; i < 6; i++)
		{
			glTexImage2D(nxtTextureTarget_CubemapX + i, 0, GL_DEPTH_COMPONENT16, mResolution, mResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTextureID, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			NXT_LOG_CRIT("Shadowmap buffer creation failure. Code {0}", (std::stringstream{} << std::hex << glCheckFramebufferStatus(GL_FRAMEBUFFER)).str());
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void Shadowmap2::BeginRenderPass()
	{
		glViewport(0, 0, mResolution, mResolution);
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Shadowmap2::EndRenderPass(int32_t width, int32_t height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Shadowmap2::BindTextureMap(uint32_t unit)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	}

	Shadowmap2::~Shadowmap2()
	{
		glDeleteTextures(1, &mTextureID);
		glDeleteFramebuffers(1, &mBufferID);
	}

}
