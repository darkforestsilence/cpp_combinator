workspace "Compiler"
	configurations { "Debug", "Release" }

project "Compiler"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"

	buildoptions {
		"-Wall",
		"-Werror",
		"-Wpedantic",
		"-O3",
		"-std=c++2a"
	}

	files { "include/**.h", "src/**.cpp" }
	includedirs { "include" }

	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"

	filter "configurations:Release"
		defines {"NDEBUG"}
		optimize "On"
