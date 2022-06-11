#pragma once

#include "framework.h"
#include "D3DUtil.h"
#include "window.h"

// Link necessary d3d12 libraries
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class D3D
{
protected:
	D3D() {}
	D3D(HWND _mainWnd, LPCWCH name);
	D3D(HWND _mainWnd, LPCWCH name, int _clientWidth, int _clientHeight);
	virtual ~D3D();

public:
	void Initialize(Window* window);

	void Render();
	void Update();

	const LPCWCH GetTitle() const { return m_title; }
	const void SetTitle(LPCWCH title) { m_title = title; }
protected:
	bool InitD3D(Window* window);
	void CreateFactory();
	void CreateDevice();
	void CreateFence(); // TODO: make Fence more

	void InitRtvDescriptorSize();
	void InitDsvDescriptorSize();
	void InitCbvSrvUavDescriptorSize();

	void CheckMSQualityLevels();

	// Enumerate all adapters in system.
	void LogAdapters();

	// Enumerates all outputs associated with an adapter. 
	void LogAdapterOutputs(IDXGIAdapter* adapter);

	// Gets a list of all display modes that support a given output and display format.
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);


protected:
	void CreateCommandObjects();
	void CreateSwapChain(Window* window);
	void CreateRtvAndDsvDescriptorHeap();

	void FlushCommandQueue();
	void OnResize();
	virtual bool OnInitialize(Window* window) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	//void Draw();

	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

protected:
	HWND m_mainWnd = nullptr;
	int m_clientWidth = 800;
	int m_clientHeight = 600;

	D3D12_VIEWPORT m_screenViewport = { 0, };
	D3D12_RECT m_scissorRect = { 0, };

	bool m_4xMsaaState = false;
	UINT m_4xMsaaQuality = 0;

	static const int m_swapChainBufferCount = 2;
	int m_currentBackBuffer = 0;
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device> m_device = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_swapChainBuffer[m_swapChainBufferCount] = { nullptr, };
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer = nullptr;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[m_swapChainBufferCount];

	// Synchronization objects.
	UINT m_frameIndex = 0;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence = nullptr;
	UINT64 m_currentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap = nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState = nullptr;

	UINT m_rtvDescriptorSize = 0;
	UINT m_dsvDescriptorSize = 0;
	UINT m_cbvSrvUavDescriptorSize = 0;

	DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

private:
	// Window title.
	LPCWCH m_title;
};