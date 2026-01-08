#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(RenderSystem* system);
	~SwapChain();

public:
	bool Init(HWND hwnd, UINT width, UINT height);
	void Present(bool vsync);
	bool Release();

private:
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_rtv;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};



