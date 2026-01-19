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
	void update();

public:
	RenderSystem* getRenderSystem();

	void addComponent(Component* component);
	void removeComponent(Component* component);

private:
	std::unique_ptr<RenderSystem> m_render_system;
	std::set<MeshComponent*> m_meshes;

	Game* m_game = nullptr;
};


