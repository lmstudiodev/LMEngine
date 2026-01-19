#include <stdafx.h>
#include <Game/Player.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::onUpdate(f32 deltaTime)
{
	Entity::onUpdate(deltaTime);

	m_elapsed_seconds += deltaTime;

	if (m_entity && m_elapsed_seconds > 3.0f)
	{
		m_entity->release();
		m_entity = nullptr;
	}
}

void Player::onCreate()
{
	Entity::onCreate();

	m_entity = getWorld()->createEntity<Entity>();
}