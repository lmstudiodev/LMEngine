#include <stdafx.h>
#include <Game/Player.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::onCreate()
{
	Entity::onCreate();

	createComponent<CameraComponent>();

	getTransformComponent()->setPosition(Vector3D(0.0f, 0.0f, -10.0f));
}

void Player::onUpdate(f32 deltaTime)
{
	Entity::onUpdate(deltaTime);

	auto transform = getTransformComponent();

	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (getInputSystem()->isKeyDown(Key::W))
	{
		m_forward = 1.0f;
	}
	else if (getInputSystem()->isKeyDown(Key::S))
	{
		m_forward = -1.0f;
	}
	else if (getInputSystem()->isKeyDown(Key::A))
	{
		m_rightward = -1.0f;
	}
	else if (getInputSystem()->isKeyDown(Key::D))
	{
		m_rightward = 1.0f;
	}

	auto deltaMousePos = getInputSystem()->getDeltaMousePosition();

	auto rotation = transform->getRotation();
	rotation = rotation + Vector3D(deltaMousePos.m_y * 0.001f, deltaMousePos.m_x * 0.001f, 0.0f);

	Matrix4x4 world;
	transform->getWorldMatrix(world);

	auto pos = transform->getPosition();
	pos = pos + world.GetZDirection() * m_forward * 8.0f * deltaTime;
	pos = pos + world.GetXDirection() * m_rightward * 8.0f * deltaTime;

	transform->setPosition(pos);
	transform->setRotation(rotation);
}

