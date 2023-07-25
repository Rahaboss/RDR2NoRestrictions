#pragma once

#include "pch.h"
#include "rage/pgPtrCollection.h"
#include "rage/scrNativeCallContext.h"

typedef HRESULT (STDMETHODCALLTYPE* SwapChainPresent_t)(IDXGISwapChain3*, UINT, UINT);

namespace Pointers
{
	void Scan();

	// D3D12 renderer pointers
	inline IDXGISwapChain3** SwapChain{};
	inline SwapChainPresent_t SwapChainPresent{};
	inline ID3D12CommandQueue** CommandQueue{};

	inline void* RunScriptThreads{};
	inline rage::pgPtrCollection* ThreadCollection{};
	inline rage::scrThread** ActiveThread{};
	inline GetNativeHandler_t GetNativeHandler{};
}
