project "Toot"

    kind "StaticLib"	

    targetdir ("%{wks.location}/bin/" .. output_dir .. "/")
    objdir ("%{wks.location}/bin-intermediate/" .. output_dir .. "/")

    files 
    {
        "**.cpp",
        "**.h",
    }

    includedirs
    {
        "%{wks.location}/%{prj.name}",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        optimize "off"

    filter "configurations:Release"
        runtime "Release"
        symbols "off"
        optimize "on"
