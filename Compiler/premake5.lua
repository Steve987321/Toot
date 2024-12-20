project "Compiler"
	kind "ConsoleApp"

	targetdir ("../bin/" .. output_dir .. "/")
	objdir ("../bin-intermediate/" .. output_dir .. "/")

    flags {"MultiProcessorCompile"}

	files {
        "Source/**.cpp",
        "Source/**.h",
    }

	-- update this  
	includedirs {
		"Source/",
	}
