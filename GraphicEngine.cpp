#include "GraphicEngine.h"
#include "RenderSystem.h"

GraphicEngine::GraphicEngine() : m_renderSystem(nullptr)
{
}

GraphicEngine::~GraphicEngine()
{
}

bool GraphicEngine::Init()
{
	try
	{
		m_renderSystem = new RenderSystem();
	}
	catch (...) {}
	
	return true;
}

bool GraphicEngine::Release()
{
	if (m_renderSystem)
	{
		delete m_renderSystem;
	}
	
	return true;
}

RenderSystem* GraphicEngine::GetRenderSystem()
{
	return m_renderSystem;
}

GraphicEngine* GraphicEngine::Get()
{
	static GraphicEngine engine;
	
	return &engine;
}