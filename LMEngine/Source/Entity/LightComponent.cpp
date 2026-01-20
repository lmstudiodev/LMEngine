#include <stdafx.h>
#include <Entity/LightComponent.h>
#include <Entity/Entity.h>
#include <Game/World.h>
#include <Game/Game.h>
#include <Core/GraphicEngine.h>

LightComponent::LightComponent()
{
	
}

LightComponent::~LightComponent()
{
	m_entity->getWorld()->getGame()->getGraphicEngine()->removeComponent(this);
}

void LightComponent::setColor(const Vector4D& color)
{
	m_color = color;
}

Vector4D LightComponent::getColor()
{
	return m_color;
}

void LightComponent::onCreateInternal()
{
	m_entity->getWorld()->getGame()->getGraphicEngine()->addComponent(this);
}
