#pragma once
#include <Prerequisites.h>

class Resource
{
public:
	Resource(const wchar_t* full_path, ResourceManager* resource_manager);
	virtual ~Resource();

protected:
	std::wstring m_fullPath;
	ResourceManager* m_manager = nullptr;
};

