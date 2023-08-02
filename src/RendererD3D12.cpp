#include "pch.h"
#include "RendererD3D12.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Menu.h"
#include "Renderer.h"

void RendererD3D12::Create()
{
	// Setup pointers
	SwapChain = *Pointers::SwapChain;
	CommandQueue = *Pointers::CommandQueue;
	hWnd = FindWindow(L"sgaWindow", NULL);

	assert(SUCCEEDED(SwapChain->GetDevice(IID_PPV_ARGS(&Device))));

	DXGI_SWAP_CHAIN_DESC Desc;
	SwapChain->GetDesc(&Desc);
	BufferCount = Desc.BufferCount;
	FrameContext = new _FrameContext[BufferCount];

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender;
	DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DescriptorImGuiRender.NumDescriptors = BufferCount;
	DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DescriptorImGuiRender.NodeMask = 0;

	assert(SUCCEEDED(Device->CreateDescriptorHeap(&DescriptorImGuiRender,
		IID_PPV_ARGS(&DescriptorHeapImGuiRender))));

	ID3D12CommandAllocator* Allocator;
	assert(SUCCEEDED(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&Allocator))));

	for (size_t i = 0; i < BufferCount; i++)
		FrameContext[i].CommandAllocator = Allocator;

	assert(SUCCEEDED(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		Allocator, NULL, IID_PPV_ARGS(&CommandList))));
	assert(SUCCEEDED(CommandList->Close()));

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
	DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	DescriptorBackBuffers.NumDescriptors = BufferCount;
	DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DescriptorBackBuffers.NodeMask = 1;

	assert(SUCCEEDED(Device->CreateDescriptorHeap(&DescriptorBackBuffers,
		IID_PPV_ARGS(&DescriptorHeapBackBuffers))));

	const UINT RTVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < BufferCount; i++)
	{
		FrameContext[i].DescriptorHandle = RTVHandle;
		ID3D12Resource* pBackBuffer;
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
		Device->CreateRenderTargetView(pBackBuffer, NULL, RTVHandle);
		FrameContext[i].Resource = pBackBuffer;
		RTVHandle.ptr += RTVDescriptorSize;
	}

	// Setup ImGui
	CreateImGui();

	// Create hooks
	Hooking::SwapChain.Create(SwapChain, Hooking::SwapChainMethodCount);
	Hooking::SwapChain.Hook(Hooking::SwapChainPresentIndex, Hooking::SwapChainPresentHook);
	//Hooking::SwapChain.Hook(Hooking::SwapChainResizeBuffersIndex, Hooking::SwapChainResizeBuffersHook);
	Hooking::SwapChain.Enable();
}

void RendererD3D12::CreateImGui()
{
	// Create ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup ImGui menu style
	Menu::SetupStyle();

	// Initialize ImGui backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX12_Init(Device, BufferCount, DXGI_FORMAT_R8G8B8A8_UNORM, DescriptorHeapImGuiRender,
		DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
		DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
	ImGui_ImplDX12_CreateDeviceObjects();

	// Setup WndProc hook
	Hooking::WndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)Hooking::WndProcHook);
}

void RendererD3D12::Destroy()
{
	// Destroy hooks
	SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)Hooking::WndProc);

	Hooking::SwapChain.Disable();
	Hooking::SwapChain.Destroy();
}

void RendererD3D12::Present()
{
	NewFrame();
	Renderer::Render();
	EndFrame();
}

void RendererD3D12::NewFrame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = Menu::IsOpen;
}

void RendererD3D12::EndFrame()
{
	_FrameContext& CurrentFrameContext = FrameContext[SwapChain->GetCurrentBackBufferIndex()];
	CurrentFrameContext.CommandAllocator->Reset();

	D3D12_RESOURCE_BARRIER Barrier;
	Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	Barrier.Transition.pResource = CurrentFrameContext.Resource;
	Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	CommandList->Reset(CurrentFrameContext.CommandAllocator, nullptr);
	CommandList->ResourceBarrier(1, &Barrier);
	CommandList->OMSetRenderTargets(1, &CurrentFrameContext.DescriptorHandle, FALSE, nullptr);
	CommandList->SetDescriptorHeaps(1, &DescriptorHeapImGuiRender);

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CommandList);

	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	CommandList->ResourceBarrier(1, &Barrier);
	CommandList->Close();
	CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&CommandList));
}
