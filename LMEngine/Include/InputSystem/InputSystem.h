#pragma once
#include <Prerequisites.h>
#include <Vector2D.h>
#include <Rect.h>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

public:
	void Update();

	bool isKeyDown(const Key& key);
	bool isKeyUp(const Key& key);

	void lockMouseCursor(bool lock);
	void setLockArea(const Rect& area);

	Vector2D getDeltaMousePosition();

private:
	short getInternalKeyCode(const Key& key);
	
	void UpdateMouseInput();
	void UpdateKeyboardInput();

private:

	short m_keys_state[256]{};
	short m_old_keys_state[256]{};
	short m_final_keys_state[256]{};

	bool m_cursorLocked = false;
	Rect m_lock_area;
	Vector2D m_lock_area_center;
	Vector2D m_delta_mouse_pos;
	Vector2D m_old_mouse_pos;
};

