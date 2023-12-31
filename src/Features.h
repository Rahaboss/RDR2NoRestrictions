#pragma once

#include "pch.h"
#include "rage/joaat.h"
#include "rage/scrNativeCallContext.h"

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

	void CreateConfigPath();
	std::filesystem::path GetConfigPath();

	Entity GetMainEntity();
	void Teleport(float x, float y, float z);
	void Teleport(const Vector3& pos);
	void TeleportOnGround(float x, float y, float z);
	void TeleportOnGround(const Vector3& pos);
	bool LoadGround(float x, float y, float z);
	bool LoadGround(const Vector3& pos);

	void SetDisablePinkertonPatrols(bool Toggle);
	bool IsEpilogueUnlocked();
	void RevealMap();
}
