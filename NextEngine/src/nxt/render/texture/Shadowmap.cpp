
#include "Shadowmap.h"

#include "TextureEnums.h"
#include <nxt/core/log/Log.h>

#include <nxt/render/RenderAPI.h>

#include <glad/glad.h>

namespace nxt
{

	// Switch to cubemap for more flexibility and CSM

	Shadowmap::Shadowmap(const uint32_t& resolution) :
		mResolution{ resolution }
	{
		glCreateFramebuffers(1, &mBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
		glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTextureStorage2D(mTextureID, 1, nxtTextureFormatInternal_Depth24Stencil8, mResolution, mResolution);
		glTextureSubImage2D(mTextureID, 0, 0, 0, mResolution, mResolution, nxtTextureFormat_DepthStencil, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_NONE, GL_NONE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTextureID, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			NXT_LOG_CRIT("Shadowmap buffer creation failure. Code {0}", (std::stringstream{} << std::hex << glCheckFramebufferStatus(GL_FRAMEBUFFER)).str());
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Shadowmap::BeginRenderPass()
	{
		glViewport(0, 0, mResolution, mResolution);
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
		glClear(GL_DEPTH_BUFFER_BIT);
		//render::command::SetFaceCullingMode(nxtCullingMode_Front);
	}

	void Shadowmap::EndRenderPass(int32_t width, int32_t height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClear(GL_DEPTH_BUFFER_BIT);
		//render::command::SetFaceCullingMode(nxtCullingMode_Back);
	}
	
	void Shadowmap::BindTextureMap(uint32_t unit)
	{
		glBindTextureUnit(unit, mTextureID);
	}

	Shadowmap::~Shadowmap()
	{
		glDeleteTextures(1, &mTextureID);
		glDeleteFramebuffers(1, &mBufferID);
	}

}
