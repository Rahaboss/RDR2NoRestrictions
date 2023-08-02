#pragma once

// C++ standard library
#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <string_view>
#include <functional>
#include <queue>
#include <filesystem>
#include <fstream>

// Windows
#include <Windows.h>
#include <Psapi.h>
#include <dxgi1_5.h>
#include <d3d12.h>

// Third-party libraries
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#include <MinHook.h>
#include <json.hpp>
typedef nlohmann::json json;

// User headers
#include "Settings.h"

// Global variables
inline HMODULE g_Module{}; // DLL handle (used to exit thread)
inline HMODULE g_GameModule{}; // Game handle
inline uintptr_t g_BaseAddress{}; // Game base address
inline HANDLE g_MainThread{}; // Main thread handle (must be closed before exiting)
inline std::atomic_bool g_Running = true; // Set to false to eject

// Enable literal macros
using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;
