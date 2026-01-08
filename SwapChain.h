#pragma once
#include <d3d11.h>
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

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};



