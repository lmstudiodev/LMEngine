#pragma once
#include <Rect.h>
#include <Prerequisites.h>

class Game
{
public:
	Game();
	virtual ~Game();

public:
	void run();
	void quit();
	GraphicEngine* getGraphicEngine();

protected:
	virtual void onCreate() {}
	virtual void onUpdate(float deltaTime) {}
	virtual void onQuit() {}



private:
	void onInternalUpdate();
	void onDisplaySize(const Rect& size);

private:
	std::unique_ptr<GraphicEngine> m_graphicEngine;
	std::unique_ptr<Display> m_display;
	std::unique_ptr<ResourceManager> m_resourceManager;

	MeshPtr m_mesh;
	MaterialPtr m_material;

	bool m_isRunning;

private:
	friend class GraphicEngine;
	friend class Display;
};

