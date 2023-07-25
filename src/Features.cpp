#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "rage/pgPtrCollection.h"
#include "Fiber.h"
#include "JobQueue.h"
#include "rage/Natives.h"

void Features::OnSetup()
{
	printf("Hello, %s!\n", PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID()));
}

void Features::OnTick()
{
}

void Features::OnExit()
{
	QUEUE_JOB(=)
	{
		// Run any script related cleanup here
		g_Running = false;
	}
	END_JOB()

	// Wait for script cleanup
	const uint64_t EndTime = GetTickCount64() + 1000;
	while (g_Running && GetTickCount64() < EndTime)
		std::this_thread::sleep_for(10ms);

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

void Features::ExecuteAsThread(rage::joaat_t ScriptHash, void (*Function)())
{
	for (uint16_t i = 0; i < Pointers::ThreadCollection->m_pCount; i++)
	{
		rage::GtaThread* Thread = Pointers::ThreadCollection->m_pData[i];
		if (!Thread || !Thread->m_ThreadID || Thread->m_ScriptHash != ScriptHash)
			continue;

		// Backup and change thread
		rage::scrThread* OriginalThread = *Pointers::ActiveThread;
		*Pointers::ActiveThread = Thread;

		// Execute
		Function();

		// Restore thread
		*Pointers::ActiveThread = OriginalThread;
		return;
	}
}

void Features::YieldThread(uint32_t DurationMS)
{
	const uint64_t EndTime = GetTickCount64() + DurationMS;
	do
	{
		Fiber::GetCurrent()->YieldThread();
	}
	while (GetTickCount64() < EndTime);
}

void Features::RunJobQueue()
{
	g_JobQueue.Run();
}
