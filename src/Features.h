#pragma once

#include "pch.h"
#include "rage/joaat.h"

namespace Features
{
	void OnSetup();
	void OnTick();
	void OnExit();

	bool IsKeyClicked(DWORD vKey);
	bool IsKeyHeld(DWORD vKey);

	// Check if D3D12 pointers are valid
	bool IsUsingD3D12();
	bool IsUsingVulkan();

	void ExecuteAsThread(rage::joaat_t ScriptHash, void (*Function)());
	void YieldThread(uint32_t DurationMS = 0);
	void RunJobQueue();
}
