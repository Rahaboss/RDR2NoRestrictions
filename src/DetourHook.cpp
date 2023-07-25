#include "pch.h"
#include "DetourHook.h"

void DetourHook::Create(void* Target, void* Detour)
{
	m_Target = Target;
	printf("Creating hook 0x%llX.\n", (uint64_t)m_Target);
	assert(m_Target && Detour);
	MH_STATUS Status = MH_CreateHook(m_Target, Detour, &m_Original);
	if (Status != MH_OK)
		printf("MinHook error %s while creating hook 0x%llX.\n", MH_StatusToString(Status), (uint64_t)m_Target);
	assert(Status == MH_OK);
}

void DetourHook::Destroy()
{
	printf("Destroying hook 0x%llX.\n", (uint64_t)m_Target);
	assert(m_Target);
	MH_STATUS Status = MH_RemoveHook(m_Target);
	if (Status != MH_OK)
		printf("MinHook error %s while destroying hook 0x%llX.\n", MH_StatusToString(Status), (uint64_t)m_Target);
	assert(Status == MH_OK);
}
