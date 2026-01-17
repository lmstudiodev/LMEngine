#pragma once
#include <Rect.h>
#include <Prerequisites.h>

class Game
{
public:
	Game();
	virtual ~Game();

	void run();
	GraphicEngine* getGraphicEngine();

private:
	std::unique_ptr<GraphicEngine> m_graphicEngine;
	std::unique_ptr<Display> m_display;
	std::unique_ptr<ResourceManager> m_resourceManager;;

	bool m_isRunning;
};

