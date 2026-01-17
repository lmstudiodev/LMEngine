#include <stdafx.h>
#include <ResourceManager.h>
#include <Mesh.h>

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
		return it->second;

	if (!std::filesystem::exists(resourcePath))
	{
		return ResourcePtr();
	}

	if (!ext.compare(L".obj"))
	{
		auto ptr = std::make_shared<Mesh>(resourcePath.c_str(), this);

		if (ptr)
		{
			m_resources.emplace(file_path, ptr);
			return ptr;
		}
	}

	return nullptr;
}
