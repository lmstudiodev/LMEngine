#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourcerPtr ResourceManager::CreateResourceFromFile(const wchar_t* file_path)
{
	std::wstring fullPath = std::filesystem::absolute(file_path);
	
	auto it = m_resources.find(fullPath);

	if (it != m_resources.end())
		return it->second;

	Resource* raw_res = this->CreateResourceFromFileConcrete(fullPath.c_str());

	if (raw_res)
	{
		ResourcerPtr res(raw_res);
		m_resources[fullPath] = res;
		return res;
	}

	return nullptr;
}
