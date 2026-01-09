#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"

class GraphicEngine
{
private:
	GraphicEngine();
	~GraphicEngine();

public:
	RenderSystem* GetRenderSystem();
	TextureManager* GetTextureManager();

public:
	static GraphicEngine* Get();
	static void Create();
	static void Release();

private:
	RenderSystem* m_renderSystem;
	TextureManager* m_textureManager;
	static GraphicEngine* m_engine;
};


