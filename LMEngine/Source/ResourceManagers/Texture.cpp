#include <stdafx.h>
#include <ResourceManagers/Texture.h>
#include <Core/GraphicEngine.h>
#include <ResourceManagers/ResourceManager.h>
#include <Game/Game.h>
#include <Core/RenderSystem.h>

Texture::Texture(const wchar_t* full_path, ResourceManager* resource_manager) : Resource(full_path, resource_manager)
{
	m_texture = resource_manager->getGame()->getGraphicEngine()->getRenderSystem()->createTexture(full_path);
}
