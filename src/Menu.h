#pragma once

#include "pch.h"

// Renderer independent ImGui menu code
namespace Menu
{
	inline bool IsOpen = false;

	void SetupStyle();
	void Render();
}
