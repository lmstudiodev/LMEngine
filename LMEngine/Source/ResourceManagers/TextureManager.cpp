#include "stdafx.h"
#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager() : ResourceManager()
{
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::CreateTextureFromFile(const wchar_t* file_path)
{
	return std::static_pointer_cast<Texture>(CreateResourceFromFile(file_path));
}

TexturePtr TextureManager::CreateTexture(const Rect& size, Texture::Type type)
{
	Texture* text = nullptr;

	try
	{
		text = new Texture(size, type);
	}
	catch (...)
	{
	}

	TexturePtr texture(text);

	return texture;
}

Resource* TextureManager::CreateResourceFromFileConcrete(const wchar_t* file_path)
{
	Texture* text = nullptr;

	try
	{
		text = new Texture(file_path);
	}
	catch (...)
	{	 
	}
	
	return text;
}
