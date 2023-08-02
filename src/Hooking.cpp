#include "pch.h"
#include "Hooking.h"
#include "RendererD3D12.h"
#include "Menu.h"
#include "Features.h"
#include "Pointers.h"
#include "Fiber.h"

void Hooking::Create()
{
	printf("Creating hooks.\n");
	assert(MH_Initialize() == MH_OK);

	RunScriptThreads.Create(Pointers::RunScriptThreads, RunScriptThreadsHook);
}

void Hooking::Destroy()
{
	printf("Destroying hooks.\n");

	RunScriptThreads.Destroy();

	assert(MH_Uninitialize() == MH_OK);
}

void Hooking::Enable()
{
	printf("Enabling hooks.\n");
	assert(MH_EnableHook(MH_ALL_HOOKS) == MH_OK);
}

void Hooking::Disable()
{
	printf("Disabling hooks.\n");
	assert(MH_DisableHook(MH_ALL_HOOKS) == MH_OK);
}

HRESULT STDMETHODCALLTYPE Hooking::SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags)
{
	if (g_Running)
		RendererD3D12::Present();

	return Hooking::SwapChain.GetOriginal<decltype(&SwapChainPresentHook)>(SwapChainPresentIndex)(SwapChain, SyncInterval, Flags);
}

HRESULT STDMETHODCALLTYPE Hooking::SwapChainResizeBuffersHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	if (g_Running)
		ImGui_ImplDX12_InvalidateDeviceObjects();

	HRESULT Result = Hooking::SwapChain.GetOriginal<decltype(&SwapChainResizeBuffersHook)>(SwapChainResizeBuffersIndex)(SwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	if (g_Running && SUCCEEDED(Result))
		ImGui_ImplDX12_CreateDeviceObjects();

	return Result;
}

static POINT s_CursorCoords{};
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Hooking::WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYUP && wParam == VK_INSERT)
	{
		// Persist and restore the cursor position between menu instances
		if (Menu::IsOpen)
			GetCursorPos(&s_CursorCoords);
		else if (s_CursorCoords.x + s_CursorCoords.y != 0)
			SetCursorPos(s_CursorCoords.x, s_CursorCoords.y);

		Menu::IsOpen = !Menu::IsOpen;
	}

	if (Menu::IsOpen)
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);

	// Always call the original event handler even if menu is open
	return CallWindowProc(WndProc, hwnd, uMsg, wParam, lParam);
}

bool Hooking::RunScriptThreadsHook(rage::pgPtrCollection* this_, uint32_t ops)
{
	bool Result = RunScriptThreads.GetOriginal<decltype(&RunScriptThreadsHook)>()(this_, ops);

	if (g_Running)
		Features::ExecuteAsThread(RAGE_JOAAT("main"), ScriptThreadTick);

	return Result;
}
