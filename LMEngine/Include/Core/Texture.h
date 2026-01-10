#pragma once
#include "Global.h"
#include "Resource.h"

class Texture : public Resource
{
public:
	Texture(const wchar_t* full_path);
	~Texture();

private:
	ID3D11Resource* m_texture;
	ID3D11ShaderResourceView* m_shaderResource;

private:
	friend class DeviceContext;
};

