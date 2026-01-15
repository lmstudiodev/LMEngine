#pragma once
#include "Resource.h"
#include "Rect.h"

class Texture : public Resource
{
public:
	enum Type
	{
		Normal = 0,
		RenderTarget,
		DepthStencil
	};

public:
	Texture(const wchar_t* full_path);
	Texture(const Rect& size, Texture::Type type);
	~Texture();

public:
	Rect GetSize();
	Texture::Type GetType();

private:
	ID3D11Resource* m_texture;
	ID3D11ShaderResourceView* m_shaderResource;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStenciltView;
	ID3D11SamplerState* m_samplerState;

	Texture::Type m_type;
	Rect m_size;

private:
	friend class DeviceContext;
};

