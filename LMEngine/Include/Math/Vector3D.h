#pragma once
#include "Vec3.h"

class Vector3D
{
public:
	Vector3D() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
	Vector3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
	Vector3D(Vec3 vec) : m_x(vec.x), m_y(vec.y), m_z(vec.z) {}
	Vector3D(const Vector3D& vec) : m_x(vec.m_x), m_y(vec.m_y), m_z(vec.m_z) {}

	static Vector3D Lerp(const Vector3D& start, const Vector3D& end, float delta)
	{
		Vector3D v;

		v.m_x = start.m_x * (1.0f - delta) + end.m_x * delta;
		v.m_y = start.m_y * (1.0f - delta) + end.m_y * delta;
		v.m_z = start.m_z * (1.0f - delta) + end.m_z * delta;

		return v;
	}

	Vector3D operator *(float num) const
	{
		return Vector3D(m_x * num, m_y * num, m_z * num);
	}

	Vector3D operator +(const Vector3D& vec) const
	{
		return Vector3D(m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z);
	}

	Vector3D operator -(const Vector3D& vec) const
	{
		return Vector3D(m_x - vec.m_x, m_y - vec.m_y, m_z - vec.m_z);
	}

	static Vector3D Normalize(const Vector3D& vec)
	{
		Vector3D result;
		
		float len = sqrt((vec.m_x * vec.m_x) + (vec.m_y * vec.m_y) + (vec.m_z * vec.m_z));

		if (!len)
			return Vector3D();

		result.m_x = vec.m_x / len;
		result.m_y = vec.m_y / len;
		result.m_z = vec.m_z / len;

		return result;
	}

	static Vector3D Cross(const Vector3D& vec1, const Vector3D& vec2)
	{
		Vector3D result;

		result.m_x = (vec1.m_y * vec2.m_z) - (vec1.m_z * vec2.m_y);
		result.m_y = (vec1.m_z * vec2.m_x) - (vec1.m_x * vec2.m_z);
		result.m_z = (vec1.m_x * vec2.m_y) - (vec1.m_y * vec2.m_x);

		return result;
	}

	~Vector3D()
	{
	}

public:
	float m_x;
	float m_y;
	float m_z;
};