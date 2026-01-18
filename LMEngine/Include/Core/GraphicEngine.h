#pragma once
#include <Prerequisites.h>

struct MeshData
{
	MeshPtr mesh;
	MaterialPtr material;
};

class GraphicEngine
{
public:
	GraphicEngine(Game* game);
	~GraphicEngine();

public:
	void update(const MeshData& data);

public:
	RenderSystem* getRenderSystem();

private:
	std::unique_ptr<RenderSystem> m_render_system;

	Game* m_game = nullptr;
};


