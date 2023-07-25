#include "pch.h"
#include "Pointers.h"
#include "Signature.h"

static SwapChainPresent_t GetSwapChainPresent(IDXGISwapChain3** sc)
{
	// vtbl** -> vtbl* -> present
	if (sc; void* p = *(void**)sc)
		if (p = *(void**)p)
			return ((SwapChainPresent_t*)p)[8];
	return 0;
}

void Pointers::Scan()
{
	printf("Scanning for pointers.\n");

	SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(SwapChain)>();
	SwapChainPresent = GetSwapChainPresent(SwapChain);
	CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(CommandQueue)>();
}
