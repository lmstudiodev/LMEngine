#include <stdafx.h>
#include <Entity/Component.h>
#include <Entity/Entity.h>

Component::Component()
{
}

Component::~Component()
{
}

void Component::release()
{
	m_entity->removeComponent(m_type_id);
}