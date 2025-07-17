project "TVM"
	kind "StaticLib"

	targetdir ("../bin/" .. output_dir .. "/")
	objdir ("../bin-intermediate/" .. output_dir .. "/")

	flags {"MultiProcessorCompile"}

	files {
		"Source/**.cpp",
		"Source/**.h",
	}

	includedirs {
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

