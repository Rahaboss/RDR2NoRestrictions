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

	ThreadCollection = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(3).Rip().Get<decltype(ThreadCollection)>();
	RunScriptThreads = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(8).Rip().Get<decltype(RunScriptThreads)>();
	ActiveThread = Signature("41 C6 04 06 01 48 8D 45 08 48 89 44 24 20 E8").Add(34).Rip().Get<decltype(ActiveThread)>();
	GetNativeHandler = Signature("E8 ? ? ? ? 42 8B 9C FE").Add(1).Rip().Get<decltype(GetNativeHandler)>();
	ScriptGlobals = Signature("48 8D 15 ? ? ? ? 48 8B 1D ? ? ? ? 8B").Add(3).Rip().Get<decltype(ScriptGlobals)>();
}
