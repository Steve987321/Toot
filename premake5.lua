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

	include "Toot"
