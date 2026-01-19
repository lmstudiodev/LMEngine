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

	//if (m_entity && m_elapsed_seconds > 3.0f)
	//{
	//	m_entity->getComponent<Component>()->release();
	//	m_entity->release();
	//	m_entity = nullptr;
	//}

	m_entity->getTransformComponent()->setRotation(Vector3D(m_elapsed_seconds, 0.0f, 0.0f));
}

void Player::onCreate()
{
	Entity::onCreate();

	m_entity = getWorld()->createEntity<Entity>();
	m_entity->createComponent<Component>();
}