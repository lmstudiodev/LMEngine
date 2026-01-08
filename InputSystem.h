#pragma once
#include "InputListener.h"
#include "Point.h"
#include <unordered_set>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void AddListener(InputListener* listener);
	void RemoveListener(InputListener* listener);
	void Update();

	void SetCursorPosition(const Point& pos);
	void ShowMouseCursor(bool show);

public:
	static InputSystem* Get();

private:
	void UpdateKeyboardInput(POINT& p);
	void UpdateMouseInput(POINT& p);

private:
	std::unordered_set<InputListener*> m_set_listeners;

	unsigned char m_keys_state[256]{};
	unsigned char m_old_keys_state[256]{};

	Point m_old_mouse_pos;

	bool m_first_time = true;
};

