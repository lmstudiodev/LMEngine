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

	virtual void OnGamePadButtonAPressed() = 0;
	virtual void OnGamePadButtonYPressed() = 0;
	virtual void OnGamePadButtonXPressed() = 0;
	virtual void OnGamePadButtonBPressed() = 0;
	virtual void OnGamePadLeftStickXChanged(const float value) = 0;
	virtual void OnGamePadLeftStickYChanged(const float value) = 0;
	virtual void OnGamePadRightStickMoved(const float valueX, const float valueY) = 0;
	virtual void OnGamePadLeftThumbPressed(bool value) = 0;
};