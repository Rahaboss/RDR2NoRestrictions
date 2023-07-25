#include "pch.h"
#include "Renderer.h"
#include "Features.h"
#include "RendererD3D12.h"
#include "RendererVulkan.h"
#include "Menu.h"

void Renderer::Create()
{
	printf("Creating renderer.\n");

	if (Features::IsUsingD3D12())
		RendererD3D12::Create();
	else
		RendererVulkan::Create();
}

void Renderer::Destroy()
{
	printf("Destroying renderer.\n");

	if (Features::IsUsingD3D12())
		RendererD3D12::Destroy();
	else
		RendererVulkan::Destroy();
}

void Renderer::Render()
{
	Menu::Render();
}
