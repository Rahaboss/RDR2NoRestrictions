#include "pch.h"
#include "Fiber.h"
#include "Features.h"

Fiber::Fiber(void(*Function)()) :
	m_ScriptFiber(nullptr),
	m_MainFiber(nullptr),
	m_Function(Function)
{
	printf("Creating fiber 0x%llX.\n", (uint64_t)m_Function);
	m_ScriptFiber = CreateFiber(0, [](void* FiberParam)
		{
			Fiber* ThisFiber = static_cast<Fiber*>(FiberParam);
			while (true)
			{
				ThisFiber->m_Function();
				ThisFiber->YieldThread();
			}
		}, this);
}

void Fiber::Destroy()
{
	printf("Destroying fiber 0x%llX.\n", reinterpret_cast<uint64_t>(m_Function));
	if (m_ScriptFiber)
		DeleteFiber(m_ScriptFiber);
}

void Fiber::YieldThread()
{
	SwitchToFiber(m_MainFiber);
}

void Fiber::Tick()
{
	m_MainFiber = GetCurrentFiber();
	SwitchToFiber(m_ScriptFiber);
}

static bool s_Initialized = false;
void ScriptThreadTick()
{
	if (!s_Initialized)
	{
		if (!IsThreadAFiber())
			ConvertThreadToFiber(NULL);
		Features::OnSetup();
		s_Initialized = true;
	}

	for (const auto& CurrentFiber : g_FiberCollection)
		CurrentFiber->Tick();
}
