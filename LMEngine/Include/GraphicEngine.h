#pragma once
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Material.h"

class GraphicEngine
{
private:
	GraphicEngine();
	~GraphicEngine();

public:
	RenderSystem* GetRenderSystem();
	TextureManager* GetTextureManager();
	MeshManager* GetMeshManager();

public:
	MaterialPtr CreateMaterial(const wchar_t* vertex_sahder_path, const wchar_t* pixel_shader_path);
	MaterialPtr CreateMaterial(const MaterialPtr& material);
	void SetMaterial(const MaterialPtr& material);
	void GetVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);

public:
	static GraphicEngine* Get();
	static void Create();
	static void Release();

private:
	RenderSystem* m_renderSystem;
	TextureManager* m_textureManager;
	MeshManager* m_meshManager;

	static GraphicEngine* m_engine;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;
};


