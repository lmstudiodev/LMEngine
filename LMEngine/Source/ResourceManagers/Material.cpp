#include <stdafx.h>
#include "Material.h"
#include "GraphicEngine.h"

Material::Material(const wchar_t* vertex_sahder_path, const wchar_t* pixel_shader_path) : m_cull_mode(CULL_MODE_BACK)
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicEngine::Get()->GetRenderSystem()->compileVertexShader(vertex_sahder_path, "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicEngine::Get()->GetRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicEngine::Get()->GetRenderSystem()->releaseCompiledShader();

	if (!m_vertex_shader)
		throw std::runtime_error("[D3D11 Error] Unable to compile vertex shader for material. Material not created.");

	GraphicEngine::Get()->GetRenderSystem()->compilePixelShader(pixel_shader_path, "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicEngine::Get()->GetRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicEngine::Get()->GetRenderSystem()->releaseCompiledShader();

	if (!m_pixel_shader)
		throw std::runtime_error("[D3D11 Error] Unable to compile pixel shader for material. Material not created.");
}

Material::Material(const MaterialPtr& material)
{
	m_vertex_shader = material->m_vertex_shader;
	m_pixel_shader = material->m_pixel_shader;
}

Material::~Material()
{
}

void Material::AddTexture(const TexturePtr& texture)
{
	m_textures.push_back(texture);
}

void Material::RemoveTexture(unsigned int texture_index)
{
	if (texture_index >= this->m_textures.size())
	{
		std::cout << "Texture index out of range" << "\n";
		return;
	}

	m_textures.erase(m_textures.begin() + texture_index);
}

void Material::SetData(void* data, unsigned int size)
{
	if (!m_constant_buffer)
	{
		m_constant_buffer = GraphicEngine::Get()->GetRenderSystem()->createConstantBuffer(data, size);
	}
	else
	{
		m_constant_buffer->Update(GraphicEngine::Get()->GetRenderSystem()->getDeviceContext(), data);
	}
}

void Material::SetCullMode(CULL_MODE cull_mode)
{
	m_cull_mode = cull_mode;
}

CULL_MODE Material::GetCullMode() const
{
	return m_cull_mode;
}
