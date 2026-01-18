#include <stdafx.h>
#include <Material.h>
#include <Game.h>
#include <RenderSystem.h>
#include <GraphicEngine.h>
#include <ResourceManager.h>
#include <VertexShader.h>
#include <PixelShader.h>
#include <ConstantBuffer.h>

Material::Material(const wchar_t* path, ResourceManager* manager) : Resource(path, manager),  m_cull_mode(CULL_MODE_BACK)
{
	auto rsys = manager->getGame()->getGraphicEngine()->getRenderSystem();

	m_vertex_shader = rsys->createVertexShader(path, "vsmain");

	if (!m_vertex_shader)
		Dx3DError("Unable to compile vertex shader for material. Material not created.");

	m_pixel_shader = rsys->createPixelShader(path, "psmain");

	if (!m_pixel_shader)
		Dx3DError("Unable to compile pixel shader for material. Material not created.");
}

Material::Material(const MaterialPtr& material, ResourceManager* manager) : Resource(L"", manager), m_cull_mode(CULL_MODE_BACK)
{
	m_vertex_shader = material->m_vertex_shader;
	m_pixel_shader = material->m_pixel_shader;
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
	auto rsys = m_manager->getGame()->getGraphicEngine()->getRenderSystem();
	
	if (!m_constant_buffer)
	{
		m_constant_buffer = rsys->createConstantBuffer(data, size);
	}
	else
	{
		m_constant_buffer->Update(rsys->getDeviceContext(), data);
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
