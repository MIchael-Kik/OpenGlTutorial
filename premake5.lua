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

project "Application"
    location "Application"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    defines {
        "GLEW_STATIC"
    }

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs {
        "dependencies/glfw/include",
        "dependencies/glew/include",
    }

    libdirs {
        "dependencies/glew/lib/"
    }

    links {
        "GLFW",
        "GLEW",
        "GL",
        "GLU",
        "dl",
        "pthread"
    }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "Release" }
