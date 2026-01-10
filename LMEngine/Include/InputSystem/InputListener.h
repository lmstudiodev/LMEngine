#pragma once
#include "Point.h"

class InputListener
{
public:
	InputListener()
	{

	}

	~InputListener()
	{

	}

	virtual void OnKeyUp(int key) = 0;
	virtual void OnKeyDown(int key) = 0;
	virtual void OnMouseMove(const Point& mouse_pos) = 0;

	virtual void OnLeftMouseButtonDown(const Point& delta_mouse_pos) = 0;
	virtual void OnLeftMouseButtonUp(const Point& delta_mouse_pos) = 0;

	virtual void OnRightMouseButtonDown(const Point& delta_mouse_pos) = 0;
	virtual void OnRightMouseButtonUp(const Point& delta_mouse_pos) = 0;
};