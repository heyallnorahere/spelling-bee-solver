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
    defines {
        "CURL_STATICLIB"
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
        "vendor/curl/lib/**.c",
        "vendor/curl/lib/**.h",
        "vendor/curl/include/**.h",
    }
    includedirs {
        "vendor/curl/include",
        "vendor/curl/lib"
    }
    defines {
        "USE_OPENSSL",
        "BUILDING_LIBCURL"
    }
    filter "system:windows"
        links {
            "ws2_32.lib",
            "libssl.lib",
            "libcrypto.lib",
            "wldap32.lib",
            "crypt32.lib",
        }
        libdirs {
            "C:/Program Files/OpenSSL/lib"
        }
        sysincludedirs {
            "C:/Program Files/OpenSSL/include"
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
    filter "system:windows"
        postbuildcommands {
            '{COPY} "C:/Program Files/OpenSSL/bin/*.dll" "%{cfg.targetdir}"',
        }
    filter "configurations:Debug"
        symbols "on"
    filter "configurations:Release"
        optimize "on"
    