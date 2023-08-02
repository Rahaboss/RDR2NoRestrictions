#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "rage/pgPtrCollection.h"
#include "Fiber.h"
#include "JobQueue.h"
#include "rage/Natives.h"
#include "Menu.h"

void Features::OnSetup()
{
}

void Features::OnTick()
{
	if (Menu::IsOpen)
		PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
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

static std::filesystem::path s_ConfigPath;
void Features::CreateConfigPath()
{
	// Get Windows %APPDATA% environment variable
	char* Buffer = nullptr;
	size_t BufferCount = 0;
	_dupenv_s(&Buffer, &BufferCount, "APPDATA");
	assert(Buffer); // Fix _dupenv_s warning
	s_ConfigPath = Buffer;
	delete Buffer;

	// Change path to %APPDATA%/RDR2NoRestrictions
	s_ConfigPath.append("RDR2NoRestrictions");

	// Create folder at path if it doesn't exist
	if (!std::filesystem::exists(s_ConfigPath))
	{
		std::filesystem::create_directory(s_ConfigPath);
		printf("Created configuration directory at: %s.\n", s_ConfigPath.string().c_str());
	}
	// If a file not a folder at that path exists, replace with folder
	else if (!std::filesystem::is_directory(s_ConfigPath))
	{
		std::filesystem::remove(s_ConfigPath);
		std::filesystem::create_directory(s_ConfigPath);
		printf("Created configuration directory at: %s.\n", s_ConfigPath.string().c_str());
	}
}

std::filesystem::path Features::GetConfigPath()
{
	return s_ConfigPath;
}
