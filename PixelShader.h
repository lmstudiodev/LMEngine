#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class PixelShader
{
public:
	PixelShader(RenderSystem* system);
	~PixelShader();

public:
	bool Release();

private:
	bool Init(const void* shader_byte_code, size_t byte_code_size);

private:
	ID3D11PixelShader* m_ps;

	RenderSystem* m_system = nullptr;

private:
	friend class RenderSystem;
	friend class DeviceContext;
};

