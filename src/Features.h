#pragma once

#include "pch.h"

namespace Features
{
	void OnSetup();
	void OnTick();
	void OnExit();

	bool IsKeyClicked(DWORD vKey);
	bool IsKeyHeld(DWORD vKey);
}
