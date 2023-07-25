#pragma once

#include "pch.h"

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
}
