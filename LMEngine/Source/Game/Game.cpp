#include <stdafx.h>
#include <Game.h>
#include <Display.h>
#include <GraphicEngine.h>
#include <ResourceManager.h>
#include <Mesh.h>
#include <Texture.h>
#include <Material.h>
#include <InputSystem.h>

Game::Game() : m_isRunning(true)
{
	m_inputSystem = std::make_unique<InputSystem>();
	m_graphicEngine = std::make_unique<GraphicEngine>(this);
	m_display = std::make_unique<Display>(this);
	m_resourceManager = std::make_unique<ResourceManager>(this);

	m_mesh = m_resourceManager->createResourceFromFile<Mesh>(L"Resources/Meshes/house.obj");
	auto texture = m_resourceManager->createResourceFromFile<Texture>(L"Resources/Textures/wood.jpg");
	m_material = m_resourceManager->createResourceFromFile<Material>(L"Resources/Shader/MyMaterial.hlsl");
	m_material->AddTexture(texture);

	m_inputSystem->setLockArea(m_display->getClientSize());
	m_inputSystem->lockMouseCursor(false);
}

Game::~Game()
{
}

void Game::onInternalUpdate()
{
	m_inputSystem->Update();

	if (m_inputSystem->isKeyUp(Key::Escape))
	{
		m_inputSystem->lockMouseCursor(false);
	}
	else if (m_inputSystem->isKeyUp(Key::Enter))
	{
		m_inputSystem->lockMouseCursor(true);
	}

	auto deltaPos = m_inputSystem->getDeltaMousePosition();

	Dx3DInfo("Delta Mouse Pos - x: " << deltaPos.m_x << " - Y: " << deltaPos.m_y << "\n");

	
	m_graphicEngine->update({m_mesh, m_material});
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
