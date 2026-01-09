#pragma once
#include "Vec2.h"

class Vector2D
{
public:
	Vector2D() : m_x(0.0f), m_y(0.0f) {}
	Vector2D(float x, float y) : m_x(x), m_y(y) {}
	Vector2D(Vec2 vec) : m_x(vec.x), m_y(vec.y) {}
	Vector2D(const Vector2D& vec) : m_x(vec.m_x), m_y(vec.m_y) {}

	Vector2D operator *(float num)
	{
		return Vector2D(m_x * num, m_y * num);
	}

	Vector2D operator +(Vector2D vec)
	{
		return Vector2D(m_x + vec.m_x, m_y + vec.m_y);
	}

	~Vector2D()
	{
	}

public:
	float m_x;
	float m_y;
};

