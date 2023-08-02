workspace "RDR2NoRestrictions"
	architecture "x86_64"
	configurations {
		-- "Debug", -- Regular debug build
		"Release" -- Build with optimization
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX", -- std::min, std::max
		"WIN32_LEAN_AND_MEAN" -- Exclude rarely used stuff
	}

	-- Locations
	startproject "RDR2NoRestrictions"
	targetdir "bin/%{cfg.buildcfg}/%{prj.name}"
	objdir "int/%{cfg.buildcfg}/%{prj.name}"

	-- Optimizations
	vectorextensions "SSE4.2"
	floatingpoint "Fast"
	staticruntime "Off" -- Sets Run-Time Library to MultiThreadedDLL
	flags {
		"MultiProcessorCompile",
		"NoManifest" -- Disable creation of Manifest XML file
	}

	-- Build configuration specifics
	filter "configurations:Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		symbols "On"
		optimize "Speed" -- All optimizations favoring speed
		flags {
			"LinkTimeOptimization"
		}

	project "RDR2NoRestrictions"
		kind "SharedLib" -- .dll
		language "C++"
		cppdialect "C++17"
		location "src" -- Project file location

		pchheader "pch.h"
		pchsource "src/pch.cpp" -- Must be defined relative to the script
		forceincludes "pch.h"

		defines {
			"NOGDI"
		}

		-- Source files
		files {
			"src/**.hpp",
			"src/**.h",
			"src/**.cpp",
			"src/**.c",
			"src/**.asm"
		}

		-- .lib locations
		libdirs {
			"bin/lib/%{cfg.buildcfg}",
			"%VULKAN_SDK%/lib"
		}
		
		-- .lib links
		links {
			"ImGui",
			"vulkan-1",
			"MinHook"
		}

		includedirs {
			"src",
			"vendor/ImGui",
			"vendor/ImGui/backends",
			"%VULKAN_SDK%/include",
			"vendor/MinHook/include",
			"vendor/json/single_include/nlohmann"
		}

		disablewarnings {
			"4838", -- Narrowing conversion
			"26812", -- Prefer "enum class"
			"26819", -- Unannotated fallthrough between switch labels
			"33011" -- Unchecked lower bound for enum key/source as index
		}

	project "ImGui"
		kind "StaticLib" -- .lib
		language "C++"
		cppdialect "C++11"
		location "src" -- .vcxproj file location

		files {
			"vendor/%{prj.name}/*.cpp",
			"vendor/%{prj.name}/*.h",
			"vendor/%{prj.name}/backends/imgui_impl_vulkan.*",
			"vendor/%{prj.name}/backends/imgui_impl_dx12.*",
			"vendor/%{prj.name}/backends/imgui_impl_win32.*"
		}

		includedirs {
			"vendor/%{prj.name}",
			"%VULKAN_SDK%/include"
		}
		
		disablewarnings {
			"26812", -- Prefer "enum class"
			"28020", -- The expression ... is not true at this call
			"33011" -- Unchecked lower bound for enum key/source as index
		}

	project "MinHook"
		kind "StaticLib" -- .lib
		language "C"
		location "src" -- Project file location

		files {
			"vendor/MinHook/src/**.c",
			"vendor/MinHook/src/**.h",
			"vendor/MinHook/src/include/*"
		}

		includedirs {
			"vendor/%{prj.name}/include"
		}
