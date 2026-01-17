#include <stdafx.h>
#include <Game.h>
#include <Display.h>
#include <GraphicEngine.h>
#include <ResourceManager.h>
#include <Mesh.h>

Game::Game() : m_isRunning(true)
{
	m_graphicEngine = std::make_unique<GraphicEngine>();
	m_display = std::make_unique<Display>(this);
	m_resourceManager = std::make_unique<ResourceManager>(this);

	m_resourceManager->createResourceFromFile<Mesh>(L"Assets/Meshes/house.obj");
}

Game::~Game()
{
}

void Game::run()
{
	MSG msg{};

	while (m_isRunning)
	{
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_isRunning = false;
				continue;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}

GraphicEngine* Game::getGraphicEngine()
{
	return m_graphicEngine.get();
}
