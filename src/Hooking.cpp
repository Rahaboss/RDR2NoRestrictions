#include "pch.h"
#include "Hooking.h"
#include "RendererD3D12.h"
#include "Menu.h"

void Hooking::Create()
{
	printf("Creating hooks.\n");
}

void Hooking::Destroy()
{
	printf("Destroying hooks.\n");
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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static POINT s_CursorCoords{};
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
