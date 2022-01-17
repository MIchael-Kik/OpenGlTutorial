workspace "OpenGl"
    startproject "Application"

    platforms {
        "x86_64"
    }

    configurations {
        "Debug",
        "Release"
    }

    filter "platforms:x86_64"
        architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "dependencies/glfw"
include "dependencies/glad"

project "Application"
    location "Application"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs {
        "dependencies/glfw/include",
        "dependencies/glad/include"
    }

    links {
        "GLFW",
        "GLAD",
        "GL"
    }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "Release" }
