project "Compiler"
	kind "ConsoleApp"

	targetdir ("../bin/" .. output_dir .. "/")
	objdir ("../bin-intermediate/" .. output_dir .. "/")

	flags {"MultiProcessorCompile"}

	files {
		"Source/**.cpp",
		"Source/**.h",
	}

	includedirs {
		"../TVM/Source/",
	}

	links {
		"TVM"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		optimize "off"

	filter "configurations:Release"
		runtime "Release"
		symbols "off"
		optimize "on"
