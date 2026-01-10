#pragma once
#include "Global.h"
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();

public:
	void Present(bool vsync);

private:
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsw;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};



