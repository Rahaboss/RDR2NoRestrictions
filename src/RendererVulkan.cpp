#include "pch.h"
#include "RendererVulkan.h"
#include "Signature.h"
#include "Menu.h"
#include "Hooking.h"
#include "Renderer.h"

// Vulkan renderer is currently disabled
#define ENABLE_VULKAN_RENDERER 0

void RendererVulkan::Create()
{
#if !ENABLE_VULKAN_RENDERER
	fprintf(stderr, "Vulkan renderer is currently disabled, use D3D12.\n");
#else
	// Setup pointers
	hWnd = FindWindow(L"sgaWindow", NULL);

	auto h = GetModuleHandle(L"vulkan-1.dll");
	if (!h) return;
	printf("vulkan-1.dll: 0x%p\n", h);

	auto vkGetDeviceProcAddr = (void* (*)(void*, void*))(GetProcAddress(h, "vkGetDeviceProcAddr"));
	if (!vkGetDeviceProcAddr) return;
	printf("vkGetDeviceProcAddr: 0x%p\n", vkGetDeviceProcAddr);

	auto vkInstance = Signature("4C 8D 05 ? ? ? ? 33 D2 48 8D 8C 24").Add(3).Rip().Get<VkInstance*>();
	if (!vkInstance) return;
	printf("vkInstance: 0x%p\n", vkInstance);

	auto vkPhysicalDevice = Signature("48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? B8").Add(3).Rip().Get<VkPhysicalDevice*>();
	if (!vkPhysicalDevice) return;
	printf("vkPhysicalDevice: 0x%p\n", vkPhysicalDevice);

	auto vkDevice = Signature("48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? B8").Add(10).Rip().Get<VkDevice*>();
	if (!vkDevice) return;
	printf("vkDevice: 0x%p\n", vkDevice);

	auto vkQueueFamily = 0; // Probably 0 or 1

	auto vkQueue = Signature("48 89 05 ? ? ? ? EB 0C 49 8B C4").Add(3).Rip().Get<VkQueue*>();
	if (!vkQueue) return;
	printf("vkQueue: 0x%p\n", vkQueue);

	auto vkPipelineCache = VK_NULL_HANDLE;
	auto vkDescriptorPool = VK_NULL_HANDLE;
	auto vkSubpass = NULL;
	auto vkMinImageCount = 2;
	auto vkImageCount = VK_NULL_HANDLE;
	auto vkMSAASamples = VK_NULL_HANDLE;
	auto vkAllocator = VK_NULL_HANDLE;
	static auto vkCheckFn = [](VkResult err) {
		if (err == 0)
			return;
		fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
	};

	auto qword_7FF7421B1A30 = Signature("48 8B 05 ? ? ? ? 48 8B 08 0F 10 41 10 48 8B 51 08 48 8B 01").Add(3).Rip().GetRaw();
	if (!qword_7FF7421B1A30 || !(*(uintptr_t*)qword_7FF7421B1A30 + 80)) return;
	printf("qword_7FF7421B1A30: 0x%llX", qword_7FF7421B1A30);

	auto msaa = *(uintptr_t*)(*(uintptr_t*)qword_7FF7421B1A30 + 80);
	printf("msaa: 0x%llX", msaa);
	printf("msaa2: %d\n", (uint8_t)msaa);

	auto sub_7FF73D042E94 = [](uint8_t a1) -> int64_t {
		if (a1 > 7u)
		{
			if (a1 < 8u)
				return 0i64;
			if (a1 <= 9u)
				return 3i64;
			if (a1 <= 13u)
				return 4i64;
			if (a1 != 14)
			{
				if (a1 == 15)
					return 2i64;
				if (a1 != 16)
				{
					if (a1 != 17)
						return 0i64;
					return 4i64;
				}
				return 3i64;
			}
			return 1i64;
		}
		switch (a1)
		{
		case 7u:
			return 2i64;
		case 0u:
			return 0i64;
		case 1u:
			return 1i64;
		case 2u:
			return 2i64;
		}
		if (a1 != 3)
		{
			if (a1 != 4)
			{
				if (a1 != 5)
				{
					if (a1 != 6)
						return 0i64;
					return 3i64;
				}
				return 2i64;
			}
			return 4i64;
		}
		return 3i64;
	};
	printf("msaa3: 0x%llX\n", sub_7FF73D042E94((uint8_t)msaa));

	InitInfo.Instance = *vkInstance;
	InitInfo.PhysicalDevice = *vkPhysicalDevice;
	InitInfo.Device = *vkDevice;
	InitInfo.QueueFamily = vkQueueFamily;
	InitInfo.Queue = *vkQueue;
	InitInfo.PipelineCache = vkPipelineCache;
	InitInfo.DescriptorPool;
	InitInfo.Subpass = vkSubpass;
	InitInfo.MinImageCount = vkMinImageCount;
	InitInfo.ImageCount;
	InitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	InitInfo.Allocator = vkAllocator;
	InitInfo.CheckVkResultFn = vkCheckFn;

	// Setup ImGui
	CreateImGui();
#endif
}

void RendererVulkan::CreateImGui()
{
	// Create ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup ImGui menu style
	Menu::SetupStyle();

	// Initialize ImGui backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplVulkan_Init(&InitInfo, VK_NULL_HANDLE);

	// Setup WndProc hook
	Hooking::WndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)Hooking::WndProcHook);
}

void RendererVulkan::Destroy()
{
#if ENABLE_VULKAN_RENDERER
	ImGui_ImplVulkan_Shutdown();
#endif
}

void RendererVulkan::Present()
{
	NewFrame();
	Renderer::Render();
	EndFrame();
}

void RendererVulkan::NewFrame()
{
	ImGui_ImplVulkan_NewFrame();
}

void RendererVulkan::EndFrame()
{
	ImGui::Render();
	//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), nullptr, nullptr);
}
