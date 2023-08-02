#include "pch.h"
#include "Console.h"
#include "Features.h"
#include "Pointers.h"
#include "Hooking.h"
#include "Renderer.h"
#include "Fiber.h"
#include "JobQueue.h"
#include "Settings.h"

void MainLoop()
{
	Console::Create();
	Features::CreateConfigPath();
	Settings::Create();
	Pointers::Scan();

	Fiber MainFiber(Features::OnTick);
	g_FiberCollection.push_back(&MainFiber);

	Fiber JobQueueFiber(Features::RunJobQueue);
	g_FiberCollection.emplace_back(&JobQueueFiber);

	Renderer::Create();
	Hooking::Create();
	Hooking::Enable();
		
	while (g_Running)
	{
		if (Features::IsKeyHeld(VK_LCONTROL) && Features::IsKeyHeld(VK_END))
		{
			Features::OnExit();
			break;
		}
		std::this_thread::sleep_for(25ms);
	}

	Hooking::Disable();
	Hooking::Destroy();
	Renderer::Destroy();
	JobQueueFiber.Destroy();
	MainFiber.Destroy();
	Settings::Destroy();
	Console::Destroy();
}

BOOL WINAPI DllMain(HMODULE Module, DWORD Reason, LPVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(Module);
		
		g_Module = Module;
		g_GameModule = GetModuleHandle(NULL);
		g_BaseAddress = reinterpret_cast<uintptr_t>(g_GameModule);
		g_MainThread = CreateThread(NULL, 0, [](LPVOID) -> DWORD
			{
				MainLoop();
				CloseHandle(g_MainThread);
				FreeLibraryAndExitThread(g_Module, EXIT_SUCCESS);
			}, NULL, 0, NULL);

		if (!g_MainThread)
			return FALSE;
	}

	return TRUE;
}
