workspace "ToadLanguage"

	configurations 
	{ 
		"Debug", 
		"Release", 
	}

	startproject "Compiler"
	
	output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	architecture "x64"
	language "c++"
	cppdialect "c++20"

	include "TVM"
	include "Compiler"
