#pragma once
#include <Rect.h>
#include <Prerequisites.h>
#include <MainWindow.h>

class Display : public MainWindow
{
public:
	Display(Game* game);
	~Display();

private:
	SwapChainPtr m_swapChain;
	Game* m_game = nullptr;
};

