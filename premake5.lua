workspace "spelling-bee-solver"
    architecture "x64"
    targetdir "build"
    configurations {
        "Debug",
        "Release"
    }
    flags {
        "MultiProcessorCompile"
    }
    startproject "spelling-bee-solver"
    filter "system:windows"
        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
group "dependencies"
project "libcurl"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "vendor/curl/src/*.c",
        "vendor/curl/src/*.h",
        "vendor/curl/include/**.h"
    }
    includedirs {
        "vendor/curl/src",
        "vendor/curl/lib",
        "vendor/curl/include"
    }
    filter "system:macosx"
        files {
            "vendor/curl/src/macos/src/*.cpp"
        }
    filter "configurations:Debug"
        symbols "on"
    filter "configurations:Release"
        optimize "on"
group ""
project "spelling-bee-solver"
    location "spelling-bee-solver"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h",
    }
    sysincludedirs {
        "vendor/json/include",
        "vendor/curl/include"
    }
    links {
        "libcurl"
    }
    filter "configurations:Debug"
        symbols "on"
    filter "configurations:Release"
        optimize "on"
    