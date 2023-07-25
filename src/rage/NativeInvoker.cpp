#include "pch.h"
#include "NativeInvoker.h"
#include "Pointers.h"

NativeContext::NativeContext()
{
	memset(this, 0, sizeof(NativeContext));
	GetContext()->m_ReturnValue = m_ReturnStack;
	GetContext()->m_Args = m_ArgStack;
}

rage::scrNativeCallContext* NativeContext::GetContext()
{
	return &m_Context;
}

void NativeContext::Reset()
{
	GetContext()->m_ArgCount = 0;
	GetContext()->m_DataCount = 0;
	memset(m_ArgStack, 0, sizeof(m_ArgStack));
}

rage::scrNativeHandler NativeContext::GetHandler(rage::scrNativeHash hash)
{
	if (Pointers::GetNativeHandler)
		return Pointers::GetNativeHandler(hash);
	return nullptr;
}

void NativeContext::FixVectors()
{
	for (uint32_t i = 0; i < GetContext()->m_DataCount; i++)
	{
		auto& out = *(GetContext()->m_OutVectors[i]);
		auto& in = GetContext()->m_InVectors[i];

		out.x = in.x;
		out.y = in.y;
		out.z = in.z;
	}
}

void NativeContext::EndCall(rage::scrNativeHash hash)
{
	if (const rage::scrNativeHandler Handler = GetHandler(hash))
	{
		Handler(GetContext());
		FixVectors();
	}
	else
	{
		printf("Failed to find native 0x%llX!\n", hash);
	}
}
