#include "stdafx.h"
#include "Texture.h"
#include "GraphicEngine.h"

Texture::Texture(const wchar_t* full_path) : Resource(full_path), 
m_texture(nullptr), 
m_shaderResource(nullptr), 
m_samplerState(nullptr), 
m_type(Texture::Type::Normal),
m_renderTargetView(nullptr),
m_depthStenciltView(nullptr)
{
	DirectX::ScratchImage imageData;

	HRESULT hr = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, imageData);

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] Load texture failed.");

	hr = DirectX::CreateTexture(GraphicEngine::Get()->GetRenderSystem()->m_d3d_device, 
		imageData.GetImages(), 
		imageData.GetImageCount(), 
		imageData.GetMetadata(),
		&m_texture);

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] Create texture failed.");

	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format = imageData.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = (UINT)(imageData.GetMetadata().mipLevels);
	desc.Texture2D.MostDetailedMip = 0;

	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = (UINT)(imageData.GetMetadata().mipLevels);

	hr = GraphicEngine::Get()->GetRenderSystem()->m_d3d_device->CreateSamplerState(&samplerDesc, &m_samplerState);

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] Create sampler state failed.");

	hr = GraphicEngine::Get()->GetRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texture, &desc, &m_shaderResource);

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] Create shader resource view failed.");
}

Texture::Texture(const Rect& size, Texture::Type type) : Resource(L""),
m_texture(nullptr),
m_shaderResource(nullptr),
m_samplerState(nullptr),
m_type(Texture::Type::Normal),
m_renderTargetView(nullptr),
m_depthStenciltView(nullptr)
{
	D3D11_TEXTURE2D_DESC tex_desc{};
	tex_desc.Width = size.width;
	tex_desc.Height = size.height;

	if (type == Normal)
	{
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == RenderTarget)
	{
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == DepthStencil)
	{
		tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	}

	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	HRESULT hr = GraphicEngine::Get()->GetRenderSystem()->m_d3d_device->CreateTexture2D(&tex_desc, nullptr, (ID3D11Texture2D**) &m_texture);

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] Texture creation failed.");

	if (type == Normal || type == RenderTarget)
	{
		hr = GraphicEngine::Get()->GetRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texture, NULL, &m_shaderResource);

		if (FAILED(hr))
			throw std::exception("[D3D11 Error] CreateShaderResourceView creation failed.");
	}
	
	if (type == RenderTarget)
	{
		hr = GraphicEngine::Get()->GetRenderSystem()->m_d3d_device->CreateRenderTargetView(m_texture, NULL, &m_renderTargetView);

		if (FAILED(hr))
			throw std::exception("[D3D11 Error] CreateRenderTargetView creation failed.");
	}
	else if (type == DepthStencil)
	{
		hr = GraphicEngine::Get()->GetRenderSystem()->m_d3d_device->CreateDepthStencilView(m_texture, NULL, &m_depthStenciltView);

		if (FAILED(hr))
			throw std::exception("[D3D11 Error] CreateDepthStencilView creation failed.");
	}

	m_type = type;
	m_size = size;
}

Texture::~Texture()
{
	if(m_renderTargetView)
		m_renderTargetView->Release();

	if (m_depthStenciltView)
		m_depthStenciltView->Release();

	if (m_samplerState)
		m_samplerState->Release();

	if (m_shaderResource)
		 m_shaderResource->Release();

	if (m_texture)
		m_texture->Release();
}

Rect Texture::GetSize()
{
	return m_size;
}

Texture::Type Texture::GetType()
{
	return m_type;
}
