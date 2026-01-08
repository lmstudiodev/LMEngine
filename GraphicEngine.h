#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicEngine
{
public:
	GraphicEngine();
	~GraphicEngine();

public:
	bool Init();
	bool Release();

	RenderSystem* GetRenderSystem();

public:
	static GraphicEngine* Get();

private:
	RenderSystem* m_renderSystem;
};


