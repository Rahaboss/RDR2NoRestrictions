#pragma once

#include "pch.h"
#include "VMTHook.h"
#include "DetourHook.h"
#include "rage/pgPtrCollection.h"
#include "rage/scrNativeCallContext.h"

namespace Hooking
{
	void Create();
	void Destroy();
	void Enable();
	void Disable();

	inline VMTHook SwapChain;
	inline constexpr size_t SwapChainMethodCount = 41, SwapChainPresentIndex = 8, SwapChainResizeBuffersIndex = 13;
	HRESULT STDMETHODCALLTYPE SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags);
	HRESULT STDMETHODCALLTYPE SwapChainResizeBuffersHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

	inline WNDPROC WndProc;
	LRESULT CALLBACK WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	inline DetourHook RunScriptThreads;
	bool RunScriptThreadsHook(rage::pgPtrCollection* this_, uint32_t ops);

	// DLC spoofing
	inline DetourHook IsDLCPresent;
	void IsDLCPresentHook(rage::scrNativeCallContext* Context);

	// No Snipers
	inline DetourHook ShootBullet;
	void ShootBulletHook(rage::scrNativeCallContext* Context);

	// No Snipers
	inline DetourHook IsEntityInArea;
	void IsEntityInAreaHook(rage::scrNativeCallContext* Context);
}
