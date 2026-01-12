#pragma once
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();

public:
	void Present(bool vsync);
	void Resize(unsigned int width, unsigned int height);
	void SetFullscreen(bool fullscreen, unsigned int width, unsigned int height);

private:
	void ReloadBuffers(unsigned int width, unsigned int height);

private:
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsw;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};



