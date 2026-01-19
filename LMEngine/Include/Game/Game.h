#pragma once
#include <Math/Rect.h>
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
	World* getWorld();
	ResourceManager* getResourceManager();

protected:
	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}
	virtual void onQuit() {}

private:
	void onInternalUpdate();
	void onDisplaySize(const Rect& size);

private:
	std::unique_ptr<InputSystem> m_inputSystem;
	std::unique_ptr<GraphicEngine> m_graphicEngine;
	std::unique_ptr<Display> m_display;
	std::unique_ptr<ResourceManager> m_resourceManager;
	std::unique_ptr<World> m_world;

	bool m_isRunning;

	std::chrono::system_clock::time_point m_previousTime;

private:
	friend class GraphicEngine;
	friend class Display;
};

