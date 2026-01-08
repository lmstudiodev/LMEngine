#include "PixelShader.h"
#include "GraphicEngine.h"

PixelShader::PixelShader() : m_ps(nullptr)
{
}

PixelShader::~PixelShader()
{
}

bool PixelShader::Init(const void* shader_byte_code, size_t byte_code_size)
{
	if (FAILED(GraphicEngine::Get()->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_ps)))
		return false;

	return true;
}

bool PixelShader::Release()
{
	m_ps->Release();

	delete this;

	return true;
}