#include <stdafx.h>
#include <GraphicEngine.h>
#include <SwapChain.h>
#include <RenderSystem.h>
#include <DeviceContext.h>
#include <Game.h>
#include <Display.h>

GraphicEngine::GraphicEngine(Game* game) : m_game(game), m_render_system(nullptr)
{
	m_render_system = std::make_unique<RenderSystem>();
}

GraphicEngine::~GraphicEngine()
{
}

void GraphicEngine::update()
{
	auto swapchain = m_game->m_display->m_swapChain;

	auto context = m_render_system->getDeviceContext();

	context->clearRenderTarget(swapchain, { 1.0f, 0.0f, 0.0f, 1.0f });

	auto winSize = m_game->m_display->getClientSize();

	context->setViewPortSize(winSize.width, winSize.height);




	swapchain->present(true);
}

RenderSystem* GraphicEngine::getRenderSystem()
{
	return m_render_system.get();
}

