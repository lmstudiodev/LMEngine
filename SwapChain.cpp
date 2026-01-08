#include "SwapChain.h"
#include "RenderSystem.h"

SwapChain::SwapChain(RenderSystem* system): m_swapChain(nullptr), m_rtv(nullptr), m_system(system)
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::Init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* device = m_system->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc{};
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = true;
	
	if (FAILED(m_system->m_dxgiFactory->CreateSwapChain(device, &desc, &m_swapChain)))
		return false;

	ID3D11Texture2D* buffer = nullptr;

	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr))
		return false;

	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(hr))
		return false;
	
	return true;
}

void SwapChain::Present(bool vsync)
{
	m_swapChain->Present(vsync, NULL);
}

bool SwapChain::Release()
{
	m_swapChain->Release();
	m_rtv->Release();

	delete this;

	return true;
}