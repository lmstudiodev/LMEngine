#pragma once
#include <ResourceManagers/Resource.h>

class Material : public Resource
{
public:
	Material(const wchar_t* path, ResourceManager* manager); 
	Material(const MaterialPtr& material, ResourceManager* manager);

public:
	void AddTexture(const TexturePtr& texture);
	void RemoveTexture(unsigned int texture_index);

	void SetData(void* data, unsigned int size);
	void SetCullMode(CullMode cull_mode);

	CullMode GetCullMode() const;

private:
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	std::vector<Texture2DPtr> m_textures;
	CullMode m_cull_mode;

private:
	friend class GraphicEngine;
};

