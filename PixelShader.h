#pragma once
#include <d3d11.h>

class GraphicEngine;
class DeviceContext;

class PixelShader
{
public:
	PixelShader();
	~PixelShader();

public:
	bool Release();

private:
	bool Init(const void* shader_byte_code, size_t byte_code_size);

private:
	ID3D11PixelShader* m_ps;

private:
	friend class GraphicEngine;
	friend class DeviceContext;
};

