#include "InitDirect3D.h"

InitDirect3D::InitDirect3D() : D3D()
{
}

InitDirect3D::InitDirect3D(HWND _hWnd) : D3D(_hWnd, L"InitDirect3D")
{
}

InitDirect3D::~InitDirect3D()
{
}

bool InitDirect3D::OnInitialize(Window* window)
{
	if (!InitD3D(window)) return false;
	else true;
}

void InitDirect3D::OnUpdate()
{
}

void InitDirect3D::OnRender()
{
	// Record all the commands we need to render the scene into the command list.
	InitCommandList();

	// Excute the command list.
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	ThrowIfFailed(m_swapChain->Present(1, 0));
}

void InitDirect3D::InitCommandList()
{
	ThrowIfFailed(m_commandAllocator->Reset());

	ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));
	
	// Indicate that the back buffer will be used as a render target.
	const auto backBufferForRT = CD3DX12_RESOURCE_BARRIER::Transition(m_swapChainBuffer[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commandList->ResourceBarrier(1, &backBufferForRT);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);

	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// Indicate that the back buffer will now be used to present.
	const auto backBufferForPresent = CD3DX12_RESOURCE_BARRIER::Transition(m_swapChainBuffer[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_commandList->ResourceBarrier(1, &backBufferForPresent);

	ThrowIfFailed(m_commandList->Close());
}
