#include <stdafx.h>
#include <Entity/Entity.h>
#include <Entity/Component.h>
#include <Entity/TransformComponent.h>
#include <Game/World.h>

Entity::Entity()
{
	m_transformComponent = createComponent<TransformComponent>();
}

Entity::~Entity()
{
}

void Entity::createComponentInternal(Component* component, size_t id)
{
	auto componentPtr = std::unique_ptr<Component>(component);

	m_components.emplace(id, std::move(componentPtr));
	component->m_type_id = id;
	component->m_entity = this;
}

Component* Entity::getComponentInternal(size_t id)
{
	auto it = m_components.find(id);

	if (it == m_components.end())
		return nullptr;

	return it->second.get();
}

void Entity::removeComponent(size_t id)
{
	m_components.erase(id);
}

World* Entity::getWorld()
{
	return m_world;
}

TransformComponent* Entity::getTransformComponent()
{
	return m_transformComponent;
}

void Entity::release()
{
	m_world->removeEntity(this);
}
