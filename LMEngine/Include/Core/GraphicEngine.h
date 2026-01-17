#pragma once
#include <Prerequisites.h>
#include <RenderSystem.h>
//#include <TextureManager.h>
#include <MeshManager.h>
#include <Material.h>

class GraphicEngine
{
public:
	GraphicEngine();
	~GraphicEngine();

public:
	RenderSystem* getRenderSystem();

private:
	std::unique_ptr<RenderSystem> m_render_system;
};


