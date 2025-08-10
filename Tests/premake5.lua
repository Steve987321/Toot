require "../scripts/cmake"
require "../scripts/ecc"

workspace "Tests"

    cppdialect "C++23"

    configurations
    {
        "Release",
        "Debug",
    }

    output_dir = "%{cfg.buildcfg}"

    project "gtest"
        kind "StaticLib"
        language "C++"

        targetdir ("bin/" .. output_dir .. "/%{prj.name}")
        objdir ("bin-intermediate/" .. output_dir .. "/%{prj.name}")

        includedirs
        {
            "../Libs/googletest/googletest",
            "../Libs/googletest/googletest/include",
        }
        files 
        {
            "../Libs/googletest/googletest/src/**.cc",
            "../Libs/googletest/googletest/**.h",
        }

        removefiles
        {
            "../Libs/googletest/googletest/src/gtest_main.cc",
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"
            optimize "off"

        filter "configurations:Release"
            runtime "Release"
            symbols "off"
            optimize "on"

    project "Tests"
        kind "ConsoleApp"
        language "C++"
        
        targetdir ("bin/" .. output_dir .. "/")
        objdir ("bin-intermediate/" .. output_dir .. "/")

        links "gtest"

        includedirs
        {
            "../Libs/googletest/googletest/include",
            "../Toot",
        }

        files 
        {
            -- toot 
            "../Toot/**.cpp",

            -- tests
            "TVM/**.cpp",
            "Compiler/**.cpp",
            "../Libs/googletest/googletest/src/gtest_main.cc",
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"
            optimize "off"

        filter "configurations:Release"
            runtime "Release"
            symbols "off"
            optimize "on"
