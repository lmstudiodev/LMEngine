#pragma once
#include <Prerequisites.h>

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);

public:
	void present(bool vsync);
	void resize(unsigned int width, unsigned int height);
	void setFullscreen(bool fullscreen, unsigned int width, unsigned int height);

private:
	void reloadBuffers(unsigned int width, unsigned int height);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsw;

	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};



