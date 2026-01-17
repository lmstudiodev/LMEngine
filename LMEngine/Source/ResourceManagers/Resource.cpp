#include <stdafx.h>
#include <Resource.h>

Resource::Resource(const wchar_t* full_path, ResourceManager* resource_manager) : m_fullPath(full_path), m_manager(resource_manager)
{
}

Resource::~Resource()
{
}
