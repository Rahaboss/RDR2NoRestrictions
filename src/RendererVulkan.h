#pragma once

#include "pch.h"

// Vulkan renderer specific code
namespace RendererVulkan
{
	void Create();
	void CreateImGui();
	void Destroy();

	void Present();
	void NewFrame();
	void EndFrame();

	inline HWND hWnd{};
	inline ImGui_ImplVulkan_InitInfo InitInfo{};
}
