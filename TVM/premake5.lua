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