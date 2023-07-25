#include "pch.h"
#include "Features.h"
#include "Pointers.h"

void Features::OnSetup()
{
}

void Features::OnTick()
{
}

void Features::OnExit()
{
	g_Running = false;
}

bool Features::IsKeyClicked(DWORD vKey)
{
	return GetAsyncKeyState(vKey) & static_cast<SHORT>(1);
}

bool Features::IsKeyHeld(DWORD vKey)
{
	return GetAsyncKeyState(vKey) & static_cast<SHORT>(1 << 15);
}

bool Features::IsUsingD3D12()
{
	return Pointers::SwapChain && (*Pointers::SwapChain)
		&& Pointers::SwapChainPresent
		&& Pointers::CommandQueue && (*Pointers::CommandQueue);
}

bool Features::IsUsingVulkan()
{
	return !Features::IsUsingD3D12();
}
