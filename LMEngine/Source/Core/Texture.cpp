#include "stdafx.h"
#include "Texture.h"
#include "GraphicEngine.h"

Texture::Texture(const wchar_t* full_path) : Resource(full_path), m_texture(nullptr), m_shaderResource(nullptr)
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
	desc.Texture2D.MipLevels = static_cast<UINT>(imageData.GetMetadata().mipLevels);
	desc.Texture2D.MostDetailedMip = 0;

	hr = GraphicEngine::Get()->GetRenderSystem()->m_d3d_device->CreateShaderResourceView(m_texture, &desc, &m_shaderResource);

	if (FAILED(hr))
		throw std::exception("[D3D11 Error] Create shader resource view failed.");
}

Texture::~Texture()
{
	m_shaderResource->Release();
	m_texture->Release();
}
