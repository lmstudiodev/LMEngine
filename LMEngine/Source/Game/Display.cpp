#include <stdafx.h>
#include <Display.h>
#include <Game.h>
#include <GraphicEngine.h>
#include <RenderSystem.h>
#include <SwapChain.h>

Display::Display(Game* game) : m_game(game)
{
	auto size = getClientSize();

	m_swapChain = game->getGraphicEngine()->getRenderSystem()->createSwapChain(static_cast<HWND>(m_hwnd), size.width, size.height);
}

Display::~Display()
{
}

void Display::onSize(const Rect& size)
{
	
	m_swapChain->resize(size.width, size.height);
	m_game->onDisplaySize(size);
}
