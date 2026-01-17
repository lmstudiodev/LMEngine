#pragma once
#include <Prerequisites.h>
#include <Rect.h>

class Texture2D
{
public:
	enum class Type
	{
		Normal = 0,
		RenderTarget,
		DepthStencil
	};

public:
	Texture2D(const wchar_t* full_path, RenderSystem* renderSystem);
	Texture2D(const Rect& size, Texture2D::Type type, RenderSystem* renderSystem);

public:
	Rect getSize();

private:
	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResource;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStenciltView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	Texture2D::Type m_type;
	Rect m_size;
	RenderSystem* m_renderSystem = nullptr;
private:
	friend class DeviceContext;
};

