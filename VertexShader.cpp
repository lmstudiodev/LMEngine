#include "VertexShader.h"
#include "GraphicEngine.h"

VertexShader::VertexShader() : m_vs(nullptr)
{
}

VertexShader::~VertexShader()
{
}

bool VertexShader::Init(const void* shader_byte_code, size_t byte_code_size)
{
	if (FAILED(GraphicEngine::Get()->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vs)))
		return false;

	return true;
}

bool VertexShader::Release()
{
	m_vs->Release();

	delete this;

	return true;
}