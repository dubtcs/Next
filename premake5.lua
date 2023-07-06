
-- global output paths
output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

-- includes directories
includes = {};
includes.glad = "NextEngine/vendor/glad/include"
includes.glm = "NextEngine/vendor/glm"
includes.spdlog = "NextEngine/vendor/spdlog/include"
includes.stb = "NextEngine/vendor/stb"
includes.gltf = "NextEngine/vendor/gltf"
includes.necs = "NextEngine/vendor/necs/nECS"

-- workspace
workspace "NEXT"
    architecture "x64"
    startproject "Editor"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    group "Vendors"
        include "NextEngine/vendor/glad"
        include "NextEngine/vendor/necs"
    group ""

    -- ENGINE PROJECT
    project "NextEngine"
        location "NextEngine";
        kind "StaticLib";
        language "C++";
        cppdialect "C++20";
        staticruntime "on";

        targetdir ("bin/"..output.."/%{prj.name}");
        objdir ("bin-int/"..output.."/%{prj.name}");

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",

            -- STB Image
            "%{prj.name}/vendor/stb/**.h",
            "%{prj.name}/vendor/stb/**.cpp",

            -- TINY GLTF
            "%{prj.name}/vendor/gltf/tinygltf.cpp",

            -- No GLFW so we need GLAD's opengl loader
            "%{prj.name}/vendor/glad/src/glad.c",
        };

        includedirs {
            "%{prj.name}/src",
            "%{includes.glad}",
            "%{includes.glm}",
            "%{includes.spdlog}",
            "%{includes.stb}",
            "%{includes.gltf}",
            "%{includes.necs}",

        }

        links {
            "glad",
            "necs",

            "opengl32.lib",
        }

        defines {
            "NXT_BUILD_ENGINE" -- Used to mark the engine build project for engine side logging
        }

        filter "system:windows"
            systemversion "latest";
            filter "configurations:Debug"
            defines {"NXT_DEBUG", "NXT_ENABLE_ASSERTS", "NXT_LOGGING_ENABLED"}
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            defines {"NXT_RELEASE", "NXT_ENABLE_ASSERTS", "NXT_LOGGING_ENABLED"}
            runtime "Release"
            optimize "On"
    
        filter "configurations:Dist"
            defines {"NXT_DIST"}
            runtime "Release"
            optimize "On"
    -- END ENGINE

    -- EDITOR PROJECT
    project "Editor"
        location "Editor"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"
            
        targetdir ("bin/"..output.."/%{prj.name}");
        objdir ("bin-int/"..output.."/%{prj.name}");
    
        links {
            "NextEngine",
        };
    
        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        };
    
        includedirs {
            "NextEngine/src",
            "%{includes.glad}",
            "%{includes.glm}",
            "%{includes.spdlog}",
            "%{includes.stb}",
            "%{includes.gltf}",
            "%{includes.necs}",

        };
    
        filter "configurations:Debug"
            defines "NXT_DEBUG"
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            defines "NXT_RELEASE"
            runtime "Release"
            optimize "On"
    
        filter "configurations:Dist"
            defines "NXT_DIST"
            runtime "Release"
            optimize "On"
    -- END EDITOR

