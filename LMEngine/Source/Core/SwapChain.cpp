#include <stdafx.h>
#include <SwapChain.h>
#include <RenderSystem.h>

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system): m_swapChain(nullptr), m_rtv(nullptr), m_system(system), m_dsw(nullptr)
{
	ID3D11Device* device = m_system->m_d3d_device.Get();

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
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = true;

	if (FAILED(m_system->m_dxgiFactory->CreateSwapChain(device, &desc, &m_swapChain)))
		Dx3DError("CreateSwapChain creation failed.");

	reloadBuffers(width, height);
}

void SwapChain::present(bool vsync)
{
	m_swapChain->Present(vsync, NULL);
}

void SwapChain::resize(unsigned int width, unsigned int height)
{
	m_rtv.Reset();
	m_dsw.Reset();
	
	HRESULT hr = m_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	if (FAILED(hr))
		Dx3DWarning("Unable to resize buffers.");

	reloadBuffers(width, height);
}

void SwapChain::setFullscreen(bool fullscreen, unsigned int width, unsigned int height)
{
	resize(width, height);
	
	HRESULT hr = m_swapChain->SetFullscreenState(fullscreen, nullptr);

	if (FAILED(hr))
		Dx3DWarning("[D3D11 warning] Unable to switch to full screen mode.");
}

void SwapChain::reloadBuffers(unsigned int width, unsigned int height)
{
	ID3D11Device* device = m_system->m_d3d_device.Get();
	
	ID3D11Texture2D* buffer = nullptr;

	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr))
		Dx3DError("SwapChain->GetBuffer failed.");

	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(hr))
		Dx3DError("CreateRenderTargetView creation failed.");

	D3D11_TEXTURE2D_DESC tex_desc{};
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	hr = device->CreateTexture2D(&tex_desc, nullptr, &buffer);

	if (FAILED(hr))
		Dx3DError("Depth stencil creation failed.");

	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsw);
	buffer->Release();

	if (FAILED(hr))
		Dx3DError("CreateDepthStencilView creation failed.");
}
