#pragma once
#include <Game.h>

class MyGame : public Game
{
public:
	MyGame();
	~MyGame();

	virtual void onUpdate(float deltaTime);
};

