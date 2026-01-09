#include "GraphicEngine.h"
#include "RenderSystem.h"
#include <exception>

GraphicEngine* GraphicEngine::m_engine = nullptr;

GraphicEngine::GraphicEngine() : m_renderSystem(nullptr), m_textureManager(nullptr)
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
}

GraphicEngine::~GraphicEngine()
{
	GraphicEngine::m_engine = nullptr;

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
