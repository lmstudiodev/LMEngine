#include <stdafx.h>
#include <Texture.h>
#include <GraphicEngine.h>
#include <ResourceManager.h>
#include <Game.h>
#include <RenderSystem.h>

Texture::Texture(const wchar_t* full_path, ResourceManager* resource_manager) : Resource(full_path, resource_manager)
{
	m_texture = resource_manager->getGame()->getGraphicEngine()->getRenderSystem()->createTexture(full_path);
}
