
#include "Skybox.h"

#include <nxt/world/model/Model2.h>
#include <nxt/render/shader/Shader.h>

namespace nxt
{

	// Global box model used by all skyboxes
	static Shared<Model2> gModel{ nullptr };
	//static Shared<Shader> gShader{ nullptr }; // dont use, let the render system handle what it wants to do with this

	Skybox::Skybox(const std::initializer_list<std::filesystem::path>& paths)
	{
		if (gModel == nullptr)
		{
			gModel = NewShared<Model2>("assets/models/SkyboxCube.gltf");
			//gShader = NewShared<Shader>(std::initializer_list<std::filesystem::path>{"assets/shaders/skybox/skybox.vert", "assets/shaders/skybox/skybox.frag"});
		}
		if (paths.size() == 6)
		{
			mTexture = NewShared<Cubemap>(paths);
		}
		else
		{
			NXT_LOG_WARN("Filepath given for skybox is not a directory or has less than 6 images provided.");
		}


		//if ((paths.size() == 1) && (std::filesystem::is_directory(*paths.begin())))
		//{
		//	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(*paths.begin()))
		//	{
		//		if (entry.exists())
		//		{
		//			// too many things could go wrong here, just stick with 1 or 6 images
		//		}
		//	}
		//}
	}

	Skybox::~Skybox()
	{

	}

	Shared<Model2> Skybox::GetModel() const
	{
		return gModel;
	}

}
