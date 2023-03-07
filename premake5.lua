
-- global output paths
output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

-- includes directories
includes = {};
includes.glad = "NextEngine/vendor/glad/include"

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
        };

        includedirs {
            "%{prj.name}/src",
            "%{includes.glad}",
        }

        links {
            "glad",
            
            "opengl32.lib",
        }

        filter "system:windows"
            systemversion "latest";
            filter "configurations:Debug"
            defines {"NXT_DEBUG", "NXT_ENABLE_ASSERTS"}
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            defines {"NXT_RELEASE", "NXT_ENABLE_ASSERTS"}
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

