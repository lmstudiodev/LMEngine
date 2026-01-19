#include <stdafx.h>
#include <Game/Game.h>
#include <Game/Display.h>
#include <Core/GraphicEngine.h>
#include <ResourceManagers/ResourceManager.h>
#include <ResourceManagers/Mesh.h>
#include <ResourceManagers/Texture.h>
#include <ResourceManagers/Material.h>
#include <InputSystem/InputSystem.h>
#include <Game/World.h>

Game::Game() : m_isRunning(true)
{
	m_inputSystem = std::make_unique<InputSystem>();
	m_graphicEngine = std::make_unique<GraphicEngine>(this);
	m_display = std::make_unique<Display>(this);
	m_resourceManager = std::make_unique<ResourceManager>(this);
	m_world = std::make_unique<World>(this);

	m_inputSystem->setLockArea(m_display->getClientSize());
	m_inputSystem->lockMouseCursor(false);
}

Game::~Game()
{
}

void Game::onInternalUpdate()
{
	auto currentTime = std::chrono::system_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>();

	if (m_previousTime.time_since_epoch().count())
	{
		elapsedSeconds = currentTime - m_previousTime;
	}

	m_previousTime = currentTime;

	auto deltaTime = (float)elapsedSeconds.count();
	
	m_inputSystem->Update();

	onUpdate(deltaTime);

	m_world->update(deltaTime);

	m_graphicEngine->update();
}

void Game::onDisplaySize(const Rect& size)
{
	m_inputSystem->setLockArea(m_display->getClientSize());

	onInternalUpdate();
}

void Game::run()
{
	onCreate();
	
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

		onInternalUpdate();
	}

	onQuit();
}

void Game::quit()
{
	m_isRunning = false;
}

GraphicEngine* Game::getGraphicEngine()
{
	return m_graphicEngine.get();
}

World* Game::getWorld()
{
	return m_world.get();
}

ResourceManager* Game::getResourceManager()
{
	return m_resourceManager.get();
}

InputSystem* Game::getInputSystem()
{
	return m_inputSystem.get();
}
