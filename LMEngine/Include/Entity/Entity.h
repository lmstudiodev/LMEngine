#pragma once
#include <Prerequisites.h>

class Entity
{
public:
	Entity();
	virtual ~Entity();

public:
	void release();

	World* getWorld();

protected:
	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}

protected:
	size_t m_type_id = 0;
	World* m_world = nullptr;

private:
	friend class World;
};

