#include <stdafx.h>
#include <PixelShader.h>
#include <RenderSystem.h>

PixelShader::PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system) : m_ps(nullptr), m_system(system)
{
	if (FAILED(m_system->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_ps)))
	{
		throw std::exception("[D3D11 Error] PixelShader creation failed.");
	}
}