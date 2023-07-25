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
	objdir "bin/%{cfg.buildcfg}/%{prj.name}"

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
			"bin/lib/%{cfg.buildcfg}"
		}
		
		-- .lib links
		links {
			
		}

		includedirs {
			"src"
		}

		disablewarnings {
			"4838", -- Narrowing conversion
			"26812", -- Prefer "enum class"
			"26819", -- Unannotated fallthrough between switch labels
			"33011" -- Unchecked lower bound for enum key/source as index
		}
