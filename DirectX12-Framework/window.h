#pragma once

#include "framework.h"

class D3D;

class Window
{
public:
	static Window* GetInst();

	static void DestroyInst();
private:
	// Default Constructor
	Window();

public:
	// Destructor
	~Window();

	bool Init(HINSTANCE hInst);
	int MainLoop();
	int MainLoop(D3D* d3d);

	ATOM MyRegisterClass();

	BOOL InitInstance();

	HWND GetWindow() const { return m_window; }
	UINT GetWidth() { return m_width; }
	UINT GetHeight() { return m_height; }
	std::tuple<UINT, UINT> GetWindowSize() { return std::make_tuple(m_width, m_height); }

	void SetWidth(UINT width);
	void SetHeight(UINT height);
	void SetWindowSize(UINT width, UINT height);
	void SetWindowSize(std::tuple<UINT, UINT> windowSize);


private:
	HINSTANCE m_hInst = nullptr;
	HWND m_window = nullptr;
	UINT m_width = 0;
	UINT m_height = 0; 
	LPCWCH m_winClassName = L"DX12Tutorial";
	LPCWCH m_currentWinName = 0;
};