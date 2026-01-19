#pragma once
#include <All.h>

class MyGame : public Game
{
public:
	MyGame();
	~MyGame();

protected:
	virtual void onCreate();
	virtual void onUpdate(f32 deltaTime);

private:
	Entity* m_entity = nullptr;
	f32 m_rotation = 0.0f;
};

