#include "stdafx.h"
#include "GraphicEngine.h"
#include "RenderSystem.h"

GraphicEngine* GraphicEngine::m_engine = nullptr;

GraphicEngine::GraphicEngine() : m_renderSystem(nullptr), m_textureManager(nullptr), m_meshManager(nullptr)
{
	try
	{
		m_renderSystem = new RenderSystem();
	}
	catch (...) 
	{
		throw std::exception("[LMEngine Error] RenderSystem creation failed.");
	}

	try
	{
		m_textureManager = new TextureManager();
	}
	catch (...)
	{
		throw std::exception("[LMEngine Error] TextureManager creation failed.");
	}

	try
	{
		m_meshManager = new MeshManager();
	}
	catch (...)
	{
		throw std::exception("[LMEngine Error] MeshManager creation failed.");
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	m_renderSystem->CompileVertexShader(L"Resources/Shader/VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	memcpy(m_mesh_layout_byte_code, shader_byte_code, size_shader);
	m_mesh_layout_size = size_shader;

	m_renderSystem->ReleaseCompiledShader();
}

GraphicEngine::~GraphicEngine()
{
	GraphicEngine::m_engine = nullptr;

	if (m_meshManager)
	{
		delete m_meshManager;
	}

	if (m_textureManager)
	{
		delete m_textureManager;
	}
	
	if (m_renderSystem)
	{
		delete m_renderSystem;
	}
}

RenderSystem* GraphicEngine::GetRenderSystem()
{
	return m_renderSystem;
}

TextureManager* GraphicEngine::GetTextureManager()
{
	return m_textureManager;
}

MeshManager* GraphicEngine::GetMeshManager()
{
	return m_meshManager;
}

void GraphicEngine::GetVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = m_mesh_layout_byte_code;
	*size = m_mesh_layout_size;
}

GraphicEngine* GraphicEngine::Get()
{
	return m_engine;
}

void GraphicEngine::Create()
{
	if(GraphicEngine::m_engine)
		throw std::exception("[LMEngine Error] GraphicEngine already exist.");

	GraphicEngine::m_engine = new GraphicEngine();
}

void GraphicEngine::Release()
{
	if (!GraphicEngine::m_engine)
		return;

	delete GraphicEngine::m_engine;
}
