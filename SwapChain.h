#pragma once
#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

public:
	bool Init(HWND hwnd, UINT width, UINT height);
	void Present(bool vsync);
	bool Release();

private:
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_rtv;

private:
	friend class DeviceContext;
};



