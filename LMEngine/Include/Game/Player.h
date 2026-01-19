#pragma once
#include <All.h>

class Player : public Entity
{
public:
	Player();
	virtual ~Player();

protected:
	virtual void onCreate();
	virtual void onUpdate(f32 deltaTime);

private:
	Entity* m_entity = nullptr;
	f32 m_elapsed_seconds = 0.0f;

	f32 m_forward = 0.0f;
	f32 m_rightward = 0.0f;
};

