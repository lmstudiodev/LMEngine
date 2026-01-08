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
	m_renderSystem = new RenderSystem();
	m_renderSystem->Init();
	
	return true;
}

bool GraphicEngine::Release()
{
	if (m_renderSystem)
	{
		m_renderSystem->Release();
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