#include <stdafx.h>
#include <ResourceManagers/ResourceManager.h>
#include <ResourceManagers/Mesh.h>
#include <ResourceManagers/Texture.h>
#include <ResourceManagers/Material.h>

ResourceManager::ResourceManager(Game* game) : m_game(game)
{
}

ResourceManager::~ResourceManager()
{
}

Game* ResourceManager::getGame()
{
	return m_game;
}

ResourcePtr ResourceManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	std::filesystem::path resourcePath = file_path;
	auto ext = resourcePath.extension();
		
	auto it = m_resources.find(file_path);

	if (it != m_resources.end())
	{
		auto mat = std::dynamic_pointer_cast<Material>(it->second);

		if(mat)
			return std::make_shared<Material>(mat, this);
		
		return it->second;
	}
		

	ResourcePtr resPtr;

	if (!std::filesystem::exists(resourcePath))
	{
		return ResourcePtr();
	}

	if (!ext.compare(L".obj"))
	{
		resPtr = std::make_shared<Mesh>(resourcePath.c_str(), this);
	}
	else if (!ext.compare(L".jpg") || !ext.compare(L".png") || !ext.compare(L".bmp"))
	{
		resPtr = std::make_shared<Texture>(resourcePath.c_str(), this);
	}
	else if (!ext.compare(L".hlsl") || !ext.compare(L".fx"))
	{
		resPtr = std::make_shared<Material>(resourcePath.c_str(), this);
	}

	if (resPtr)
	{
		m_resources.emplace(file_path, resPtr);
		return resPtr;
	}

	return nullptr;
}
