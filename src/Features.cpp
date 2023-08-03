#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "rage/pgPtrCollection.h"
#include "Fiber.h"
#include "JobQueue.h"
#include "rage/Natives.h"
#include "Menu.h"
#include "ScriptGlobal.h"
#include "JobQueue.h"

void Features::OnSetup()
{
}

void Features::OnTick()
{
	if (Menu::IsOpen)
		PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

	if (g_Settings["disable_cutscene_borders"].get<bool>())
	{
		CAM::_REQUEST_LETTER_BOX_OVERTIME(-1, -1, false, 17, true, false);
		CAM::_FORCE_LETTER_BOX_THIS_UPDATE();
	}

	if (g_Settings["disable_invisible_snipers"].get<bool>() && Features::IsEpilogueUnlocked())
		SetDisablePinkertonPatrols(true);
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

Entity Features::GetMainEntity()
{
	Ped player = PLAYER::PLAYER_PED_ID();
	
	if (Ped mount = PED::GET_MOUNT(player))
		return mount;
	
	if (Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(player, false))
		return vehicle;
	
	return player;
}

void Features::Teleport(float x, float y, float z)
{
	ENTITY::SET_ENTITY_COORDS(GetMainEntity(), x, y, z, false, false, false, false);
}

void Features::Teleport(const Vector3& pos)
{
	Teleport(pos.x, pos.y, pos.z);
}

void Features::TeleportOnGround(float x, float y, float z)
{
	QUEUE_JOB(=)
	{
		LoadGround(x, y, z);
		Teleport(x, y, z - 1.0f);
		YieldThread();
		ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(GetMainEntity(), true);
	}
	END_JOB()
}

void Features::TeleportOnGround(const Vector3& pos)
{
	TeleportOnGround(pos.x, pos.y, pos.z);
}

bool Features::LoadGround(float x, float y, float z)
{
	float groundZ;
	const uint8_t attempts = 10;

	for (uint8_t i = 0; i < attempts; i++)
	{
		// Only request a collision after the first try failed because the location might already be loaded on first attempt.
		for (uint16_t j = 0; i && j < 1000; j += 100)
		{
			STREAMING::REQUEST_COLLISION_AT_COORD(x, y, (float)j);

			YieldThread();
		}

		if (MISC::GET_GROUND_Z_FOR_3D_COORD(x, y, 1000.f, &groundZ, false))
		{
			z = groundZ + 1.f;

			return true;
		}

		YieldThread();
	}

	return false;
}

bool Features::LoadGround(const Vector3& pos)
{
	return LoadGround(pos.x, pos.y, pos.z);
}

void Features::SetDisablePinkertonPatrols(bool Toggle)
{
	if (bool* b = ScriptGlobal(1934266).At(56).Get<bool*>())
		*b = Toggle;
}

bool Features::IsEpilogueUnlocked()
{
	if (Hash* Global_1946054_f_1 = ScriptGlobal(1946054).At(1).Get<Hash*>())
		return (*Global_1946054_f_1) == RAGE_JOAAT("MPC_PLAYER_TYPE_SP_MARSTON");

	return false;
}

void Features::RevealMap()
{
	QUEUE_JOB(=)
	{
		MAP::SET_MINIMAP_HIDE_FOW(true);
		MAP::_REVEAL_MINIMAP_FOW(0);
	}
	END_JOB()
}
