#include <stdafx.h>
#include <Game/MyGame.h>
#include <Game/Player.h>

MyGame::MyGame()
{

}

MyGame::~MyGame()
{
}

void MyGame::onUpdate(f32 deltaTime)
{
	Game::onUpdate(deltaTime);
}

void MyGame::onCreate()
{
	Game::onCreate();

	m_entity = getWorld()->createEntity<Player>();
}