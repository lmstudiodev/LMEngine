#include <stdafx.h>
#include <Entity/Entity.h>
#include <Game/World.h>

Entity::Entity()
{
}

Entity::~Entity()
{
}

World* Entity::getWorld()
{
	return m_world;
}

void Entity::release()
{
	m_world->removeEntity(this);
}
