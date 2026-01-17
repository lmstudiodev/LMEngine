#include <stdafx.h>
#include <GraphicEngine.h>
#include <RenderSystem.h>
#include <DeviceContext.h>


GraphicEngine::GraphicEngine() : m_render_system(nullptr)
{
	m_render_system = std::make_unique<RenderSystem>();
}

GraphicEngine::~GraphicEngine()
{

}

RenderSystem* GraphicEngine::getRenderSystem()
{
	return m_render_system.get();
}

