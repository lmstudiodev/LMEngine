#pragma once
#include <Resource.h>

class Texture : public Resource
{
public:
	Texture(const wchar_t* full_path, ResourceManager* resource_manager);

private:
	Texture2DPtr m_texture;

private:
	friend class Material;
};


