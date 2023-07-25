#pragma once

#include "pch.h"

typedef HRESULT (STDMETHODCALLTYPE* SwapChainPresent_t)(IDXGISwapChain3*, UINT, UINT);

namespace Pointers
{
	void Scan();

	// D3D12 renderer pointers
	inline IDXGISwapChain3** SwapChain{};
	inline SwapChainPresent_t SwapChainPresent{};
	inline ID3D12CommandQueue** CommandQueue{};
}
