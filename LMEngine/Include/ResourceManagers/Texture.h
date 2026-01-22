#pragma once
#include <ResourceManagers/Resource.h>

class Texture : public Resource
{
public:
	Texture(const wchar_t* full_path, ResourceManager* resource_manager);
	const Texture2DPtr& getTexture();

private:
	Texture2DPtr m_texture;

private:
	friend class Material;
};


