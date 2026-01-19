#pragma once
#include <Prerequisites.h>

class Component
{
public:
	Component();
	virtual ~Component();

public:
	void release();

private:
	size_t m_type_id = 0;
	Entity* m_entity = nullptr;

private:
	friend class Entity;
};

