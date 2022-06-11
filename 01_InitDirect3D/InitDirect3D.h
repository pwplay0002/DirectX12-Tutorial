#pragma once
#include <DirectX12-Framework/D3D.h>

class InitDirect3D : public D3D
{
public:
	InitDirect3D();
	InitDirect3D(HWND _hWnd);
	~InitDirect3D();

protected:
	virtual bool OnInitialize(Window* window) override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

private:
	void InitCommandList();
};