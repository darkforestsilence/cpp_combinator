workspace "Compiler"
	configurations { "Debug", "Release" }

project "Compiler"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	targetdir "bin/%{cfg.buildcfg}"

	buildoptions {
		"-Wall",
		"-Werror",
		"-Wpedantic",
		"-O3"
	}

	files { "include/**.h", "src/**.cpp" }
	includedirs { "include" }

	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"

	filter "configurations:Release"
		defines {"NDEBUG"}
		optimize "On"
