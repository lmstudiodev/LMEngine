#pragma once
#include <Windows.h>


class Point
{
public:
	Point() : m_axis_x(0), m_axis_y(0)
	{
	}

	Point(int x, int y) : m_axis_x(x), m_axis_y(y)
	{
	}

	Point(const Point& point) : m_axis_x(point.m_axis_x), m_axis_y(point.m_axis_y)
	{
	}

	~Point()
	{

	}

	static bool IsEqualTo(POINT p1, Point p2)
	{
		if (p1.x != p2.m_axis_x || p1.y != p2.m_axis_y)
			return false;

		return true;
	}

	static Point GetDelta(POINT p1, Point p2)
	{
		auto deltaX = p1.x - p2.m_axis_x;
		auto deltaY = p1.y - p2.m_axis_y;
		
		return Point(deltaX, deltaY);
	}

public:
	int m_axis_x;
	int m_axis_y;
};