#pragma once
#include <ResourceManagers/Resource.h>

enum CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK
};

class Material : public Resource
{
public:
	Material(const wchar_t* path, ResourceManager* manager); 
	Material(const MaterialPtr& material, ResourceManager* manager);

public:
	void AddTexture(const TexturePtr& texture);
	void RemoveTexture(unsigned int texture_index);

	void SetData(void* data, unsigned int size);
	void SetCullMode(CULL_MODE cull_mode);

	CULL_MODE GetCullMode() const;

private:
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	std::vector<Texture2DPtr> m_textures;
	CULL_MODE m_cull_mode;

private:
	friend class GraphicEngine;
};

