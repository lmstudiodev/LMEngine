#include <stdafx.h>
#include <Display.h>
#include <Game.h>
#include <GraphicEngine.h>
#include <RenderSystem.h>

Display::Display(Game* game) : m_game(game)
{
	auto size = getClientSize();

	m_swapChain = game->getGraphicEngine()->getRenderSystem()->createSwapChain(static_cast<HWND>(m_hwnd), size.width, size.height);
}

Display::~Display()
{
}
