#pragma once
#include <Math/Rect.h>
#include <Prerequisites.h>
#include <WindowSystem/MainWindow.h>

class Display : public MainWindow
{
public:
	Display(Game* game);
	~Display();

private:
	SwapChainPtr m_swapChain;
	Game* m_game = nullptr;

protected:
	virtual void onSize(const Rect& size);

private:
	friend class GraphicEngine;
};

