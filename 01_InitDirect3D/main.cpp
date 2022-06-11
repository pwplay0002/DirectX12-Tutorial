#pragma once

#include "InitDirect3D.h"
#include "DirectX12-Framework/D3DUtil.h"

#include <DirectX12-Framework/framework.h>
#include <DirectX12-Framework/window.h>
#include <memory>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	try
	{
		const auto d3d = std::make_unique<InitDirect3D>();
		d3d->SetTitle(L"InitDirect3D");
		Window::GetInst()->Init(hInstance);
		Window::GetInst()->SetWindowSize(1000, 700);
		Window::GetInst()->MainLoop(d3d.get());
	}
	catch (const com_exception& exception)
	{
		OutputDebugStringA(exception.what());
		return 0;
	}

}