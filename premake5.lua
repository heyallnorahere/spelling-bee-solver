newoption {
    trigger = "method",
    value = "ALGORITHM",
    description = "Method of sorting through words",
    default = "DAWG",
    allowed = {
        { "DAWG", "Directed Acyclic Word Graph" },
        { "slower", "Slower algorithm, the initial" }
    }
}
newoption {
    trigger = "dictionary",
    value = "DICT",
    description = "The list of words to use for parsing",
    default = "twl06",
    allowed = {
        { "dwyl", "DWYL Text file via codegen" },
        { "twl06", "Official Scrabble dictionary via codegen" },
        { "dwyl-json", "DWYL Json file (libCURL)" }
    }
}
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
    filter "options:method=DAWG"
        defines {
            "ALGORITHM_DAWG"
        }
    filter "options:method=slower"
        defines {
            "ALGORITHM_SLOWER"
        }
    filter "options:dictionary=dwyl"
        defines {
            "DICTIONARY_DWYL"
        }
    filter "options:dictionary=twl06"
        defines {
            "DICTIONARY_TWL06"
        }
    filter "options:dictionary=dwyl-json"
        defines {
            "DICTIONARY_DWYL_JSON"
        }
    filter "system:windows"
        defines {
            "SYSTEM_WINDOWS"
        }
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
urls = {}
urls["dwyl"] = "https://raw.githubusercontent.com/dwyl/english-words/master/words_alpha.txt"
urls["twl06"] = "https://www.wordgamedictionary.com/twl06/download/twl06.txt"
group "dependencies"
project "libcurl"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    staticruntime "on"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    includedirs {
        "vendor/curl/include",
        "vendor/curl/lib"
    }
    defines {
        "USE_OPENSSL",
        "BUILDING_LIBCURL"
    }
    filter "system:macosx"
        files {
            "libcurl_placeholder.c"
        }
    filter "system:not macosx"
        files {
            "vendor/curl/lib/**.c",
            "vendor/curl/lib/**.h",
            "vendor/curl/include/**.h",
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
group "tools"
project "codegen"
    location "codegen"
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
        "vendor/curl/include",
        "vendor/json/include"
    }
    filter "system:macosx"
        libdirs {
            "vendor/curl_binaries/lib",
            "/usr/local/opt/zlib/lib",
            "/usr/local/opt/openssl/lib",
            "/usr/local/opt/libssh2/lib",
            "/usr/local/opt/openldap/lib",
        }
        links {
            "curl",
            "z",
            "ssl",
            "crypto",
            "ssh2",
            "ldap",
            "lber"
        }
    filter "system:windows"
        links {
            "libcurl"
        }
        postbuildcommands {
            '{COPY} "C:/Program Files/OpenSSL/bin/*.dll" "%{cfg.targetdir}"',
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
        "vendor/json/include",
        "vendor/curl/include"
    }
    filter "options:not dictionary=dwyl-json"
        links {
            "codegen"
        }
        prebuildcommands {
            '"%{cfg.targetdir}/../codegen/codegen" embed "' .. urls[_OPTIONS["dictionary"]] .. '" "src/generated_dict.h"'
        }
    filter "system:macosx"
        libdirs {
            "vendor/curl_binaries/lib",
            "/usr/local/opt/zlib/lib",
            "/usr/local/opt/openssl/lib",
            "/usr/local/opt/libssh2/lib",
            "/usr/local/opt/openldap/lib",
        }
        links {
            "curl",
            "z",
            "ssl",
            "crypto",
            "ssh2",
            "ldap",
            "lber"
        }
    filter "system:windows"
        links {
            "libcurl"
        }
        postbuildcommands {
            '{COPY} "C:/Program Files/OpenSSL/bin/*.dll" "%{cfg.targetdir}"',
        }
    filter "configurations:Debug"
        symbols "on"
    filter "configurations:Release"
        optimize "on"
group ""