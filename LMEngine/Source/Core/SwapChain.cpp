#include "SwapChain.h"
#include "RenderSystem.h"

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system): m_swapChain(nullptr), m_rtv(nullptr), m_system(system), m_dsw(nullptr)
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
		throw std::exception("[D3D11 Error] CreateSwapChain creation failed.");

	ID3D11Texture2D* buffer = nullptr;

	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] SwapChain->GetBuffer failed.");

	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] CreateRenderTargetView creation failed.");

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
		throw std::exception("[D3D11 Error] Depth stencil creation failed.");

	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsw);
	buffer->Release();

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] CreateDepthStencilView creation failed.");
}

SwapChain::~SwapChain()
{
	if(m_swapChain)
		m_swapChain->Release();

	if(m_rtv)
		m_rtv->Release();

	if (m_dsw)
		m_dsw->Release();
}

void SwapChain::Present(bool vsync)
{
	m_swapChain->Present(vsync, NULL);
}