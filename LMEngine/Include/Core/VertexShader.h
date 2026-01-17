#pragma once
#include <Prerequisites.h>

class VertexShader
{
public:
	VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;

	RenderSystem* m_system = nullptr;

private:
	friend class RenderSystem;
	friend class DeviceContext;
};

