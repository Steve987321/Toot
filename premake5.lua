require "Scripts/cmake"

workspace "Toot"
	configurations 
	{ 
		"Debug", 
		"Release", 
	}

	output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	architecture "x64"
	language "c++"
	cppdialect "c++23"

	project "Language"
	
		kind "StaticLib"	

		targetdir ("../bin/" .. output_dir .. "/")
		objdir ("../bin-intermediate/" .. output_dir .. "/")

		files 
		{
			"Source/**.cpp",
			"Source/**.h",
		}

		includedirs
		{
			"Source/",
		}

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
			optimize "off"

		filter "configurations:Release"
			runtime "Release"
			symbols "off"
			optimize "on"
