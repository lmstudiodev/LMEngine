#pragma once
#include "Global.h"
#include "Prerequisites.h"
#include "Resource.h"

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	ResourcerPtr CreateResourceFromFile(const wchar_t* file_path);

protected:
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* file_path) = 0;

private:
	std::unordered_map<std::wstring, ResourcerPtr> m_resources;
};

