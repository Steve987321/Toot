project "Toot"

    kind "StaticLib"	

    targetdir ("bin/" .. output_dir .. "/")
    objdir ("bin-intermediate/" .. output_dir .. "/")

    files 
    {
        "**.cpp",
        "**.h",
    }

    includedirs
    {
        "%{prj.name}",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        optimize "off"

    filter "configurations:Release"
        runtime "Release"
        symbols "off"
        optimize "on"
