#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicEngine
{
private:
	GraphicEngine();
	~GraphicEngine();

public:
	RenderSystem* GetRenderSystem();

public:
	static GraphicEngine* Get();
	static void Create();
	static void Release();

private:
	RenderSystem* m_renderSystem;
	static GraphicEngine* m_engine;
};


