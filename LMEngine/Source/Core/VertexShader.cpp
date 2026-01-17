#include <stdafx.h>
#include <VertexShader.h>
#include <RenderSystem.h>

VertexShader::VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* system) : m_vs(nullptr), m_system(system)
{
	if (FAILED(m_system->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vs)))
		throw std::exception("[D3D11 Error] VertexShader creation failed.");
}
