#pragma once
#include "ResourceManager.h"
#include "Texture.h"

class TextureManager : public ResourceManager
{
public:
	TextureManager();
	~TextureManager();

	TexturePtr CreateTextureFromFile(const wchar_t* file_path);
	TexturePtr CreateTexture(const Rect& size, Texture::Type type);

protected:
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* file_path);
};

