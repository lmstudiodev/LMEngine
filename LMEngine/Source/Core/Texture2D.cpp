#include <stdafx.h>
#include <Core/Texture2D.h>
#include <Core/GraphicEngine.h>
#include <Core/RenderSystem.h>

Texture2D::Texture2D(const wchar_t* full_path, RenderSystem* renderSystem) :
m_texture(nullptr), 
m_shaderResource(nullptr), 
m_samplerState(nullptr), 
m_type(Texture2D::Type::Normal),
m_renderTargetView(nullptr),
m_depthStenciltView(nullptr),
m_renderSystem(renderSystem)
{
	DirectX::ScratchImage imageData;

	HRESULT hr = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, imageData);

	if (FAILED(hr))
		Dx3DError("Load texture failed.");

	hr = DirectX::CreateTexture(m_renderSystem->m_d3d_device.Get(),
		imageData.GetImages(), 
		imageData.GetImageCount(), 
		imageData.GetMetadata(),
		&m_texture);

	if (FAILED(hr))
		Dx3DError("Create texture failed.");

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

	hr = m_renderSystem->m_d3d_device->CreateSamplerState(&samplerDesc, &m_samplerState);

	if (FAILED(hr))
		Dx3DError("Create sampler state failed.");

	hr = m_renderSystem->m_d3d_device->CreateShaderResourceView(m_texture.Get(), &desc, &m_shaderResource);

	if (FAILED(hr))
		Dx3DError("Create shader resource view failed.");

	m_size = Rect(0, 0, (i32)imageData.GetMetadata().width, (i32)imageData.GetMetadata().height);
}

Texture2D::Texture2D(const Rect& size, Texture2D::Type type, RenderSystem* renderSystem) :
m_texture(nullptr),
m_shaderResource(nullptr),
m_samplerState(nullptr),
m_type(Texture2D::Type::Normal),
m_renderTargetView(nullptr),
m_depthStenciltView(nullptr),
m_renderSystem(renderSystem)
{
	D3D11_TEXTURE2D_DESC tex_desc{};
	tex_desc.Width = size.width;
	tex_desc.Height = size.height;

	if (type == Texture2D::Type::Normal)
	{
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == Texture2D::Type::RenderTarget)
	{
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == Texture2D::Type::DepthStencil)
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

	HRESULT hr = m_renderSystem->m_d3d_device->CreateTexture2D(&tex_desc, nullptr, (ID3D11Texture2D**)m_texture.GetAddressOf());

	if (FAILED(hr))
		Dx3DError(" Texture creation failed.");

	if (type == Texture2D::Type::Normal || type == Texture2D::Type::RenderTarget)
	{
		hr = m_renderSystem->m_d3d_device->CreateShaderResourceView(m_texture.Get(), NULL, &m_shaderResource);

		if (FAILED(hr))
			Dx3DError("CreateShaderResourceView creation failed.");
	}
	
	if (type == Texture2D::Type::RenderTarget)
	{
		hr = m_renderSystem->m_d3d_device->CreateRenderTargetView(m_texture.Get(), NULL, &m_renderTargetView);

		if (FAILED(hr))
			Dx3DError("CreateRenderTargetView creation failed.");
	}
	else if (type == Texture2D::Type::DepthStencil)
	{
		hr = m_renderSystem->m_d3d_device->CreateDepthStencilView(m_texture.Get(), NULL, &m_depthStenciltView);

		if (FAILED(hr))
			Dx3DError("CreateDepthStencilView creation failed.");
	}

	m_type = type;
	m_size = size;
}


Rect Texture2D::getSize()
{
	return m_size;
}
