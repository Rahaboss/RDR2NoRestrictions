#include "pch.h"
#include "Features.h"

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
