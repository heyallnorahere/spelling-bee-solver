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
project "codegen"
    location "codegen"
    kind "ConsoleApp"
    language "C"
    staticruntime "on"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.h",
    }
    filter "configurations:Debug"
        symbols "on"
    filter "configurations:Release"
        optimize "on"
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
        "vendor/json/include"
    }
    links {
        "codegen"
    }
    prebuildcommands {
        '"%{cfg.targetdir}/../codegen/codegen" embed "../vendor/english-words/words_dictionary.json" "generated_words.h"'
    }
    filter "configurations:Debug"
        symbols "on"
    filter "configurations:Release"
        optimize "on"
    