#include <stdafx.h>
#include "InputSystem.h"

InputSystem* InputSystem::m_inputSystem = nullptr;

InputSystem::InputSystem()
{
	m_gamepad = new GamePad();
}

InputSystem::~InputSystem()
{
	InputSystem::m_inputSystem = nullptr;
}

void InputSystem::AddListener(InputListener* listener)
{
	m_set_listeners.insert(listener);
}

void InputSystem::RemoveListener(InputListener* listener)
{
	m_set_listeners.erase(listener);
}

void InputSystem::Update()
{
	POINT currentMousePos{};

	GetCursorPos(&currentMousePos);
	
	UpdateMouseInput(currentMousePos);
	UpdateKeyboardInput(currentMousePos);

	if (!m_gamepad->Refresh())
	{
		if (wasConnected)
		{
			wasConnected = false;

			std::cout << "Please connect an Xbox 360 controller." << std::endl;
		}
	}
	else
	{
		if (!wasConnected)
		{
			wasConnected = true;

			std::cout << "Controller connected on port " << m_gamepad->GetPort() << std::endl;
		}
		else
		{
			UpdateGamePadButton();
			UpdateGamePadLeftStick();
			UpdateGamePadRightStick();
		}
	}
}

void InputSystem::SetCursorPosition(const Point& pos)
{
	SetCursorPos(pos.m_axis_x, pos.m_axis_y);
}

void InputSystem::ShowMouseCursor(bool show)
{
	ShowCursor(show);
}

InputSystem* InputSystem::Get()
{
	return m_inputSystem;
}

void InputSystem::Create()
{
	if(InputSystem::m_inputSystem)
		throw std::exception("[LMEngine Error] InputSystem already exist.");

	InputSystem::m_inputSystem = new InputSystem();
}

void InputSystem::Release()
{
	if (!InputSystem::m_inputSystem)
		return;

	delete InputSystem::m_inputSystem;
}

void InputSystem::UpdateKeyboardInput(POINT& p)
{
	for (unsigned int i = 0; i < 255; i++)
	{
		m_keys_state[i] = GetAsyncKeyState(i);
		
		if (m_keys_state[i] & 0x8001)
		{
			std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

			while (it != m_set_listeners.end())
			{
				if (i == VK_LBUTTON)
				{
					if (m_keys_state[i] != m_old_keys_state[i])
						(*it)->OnLeftMouseButtonDown(Point::GetDelta(p, m_old_mouse_pos));
				}
				else if (i == VK_RBUTTON)
				{
					if (m_keys_state[i] != m_old_keys_state[i])
						(*it)->OnRightMouseButtonDown(Point::GetDelta(p, m_old_mouse_pos));
				}
				else
				{
					(*it)->OnKeyDown(i);
				}

				++it;
			}
		}
		else
		{
			if (m_keys_state[i] != m_old_keys_state[i])
			{
				std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

				while (it != m_set_listeners.end())
				{
					if (i == VK_LBUTTON)
					{
						(*it)->OnLeftMouseButtonUp(Point::GetDelta(p, m_old_mouse_pos));
					}
					else if (i == VK_RBUTTON)
					{
						(*it)->OnRightMouseButtonUp(Point::GetDelta(p, m_old_mouse_pos));
					}
					else
					{
						(*it)->OnKeyUp(i);
					}

					++it;
				}
			}
		}
	}

	memcpy(m_old_keys_state, m_keys_state, sizeof(short) * 256);
}

void InputSystem::UpdateMouseInput(POINT& p)
{
	if (m_first_time)
	{
		m_old_mouse_pos = Point(p.x, p.y);
		m_first_time = false;
	}

	if (!Point::IsEqualTo(p, m_old_mouse_pos))
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnMouseMove(Point(p.x, p.y));
			++it;
		}
	}

	m_old_mouse_pos = Point(p.x, p.y);
}

void InputSystem::UpdateGamePadButton()
{

	if (m_gamepad->IsPressed(XINPUT_GAMEPAD_A))
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadButtonAPressed();

			++it;
		}
	}
	else if (m_gamepad->IsPressed(XINPUT_GAMEPAD_Y))
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadButtonYPressed();

			++it;
		}
	}
	else if (m_gamepad->IsPressed(XINPUT_GAMEPAD_X))
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadButtonXPressed();

			++it;
		}
	}
	else if (m_gamepad->IsPressed(XINPUT_GAMEPAD_B))
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadButtonBPressed();

			++it;
		}
	}
	
	if (m_gamepad->IsPressed(XINPUT_GAMEPAD_LEFT_THUMB))
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadLeftThumbPressed(true);

			++it;
		}
	}
	else
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadLeftThumbPressed(false);

			++it;
		}
	}
}

void InputSystem::UpdateGamePadLeftStick()
{
	if (abs(m_gamepad->m_leftStickX) >= 0.1f)
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadLeftStickXChanged(m_gamepad->m_leftStickX);

			++it;
		}
	}
	else
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadLeftStickXChanged(0);

			++it;
		}
	}

	if (abs(m_gamepad->m_leftStickY) >= 0.1f)
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadLeftStickYChanged(m_gamepad->m_leftStickY);

			++it;
		}
	}
	else
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadLeftStickYChanged(0);

			++it;
		}
	}
}

void InputSystem::UpdateGamePadRightStick()
{
	if (abs(m_gamepad->m_rightStickX) >= 0.1f && abs(m_gamepad->m_rightStickY) >= 0.1f)
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadRightStickMoved(m_gamepad->m_rightStickX, m_gamepad->m_rightStickY);

			++it;
		}
	}
	else if (abs(m_gamepad->m_rightStickX) < 0.1f && abs(m_gamepad->m_rightStickY) >= 0.1f)
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadRightStickMoved(0, m_gamepad->m_rightStickY);

			++it;
		}
	}
	else if (abs(m_gamepad->m_rightStickX) >= 0.1f && abs(m_gamepad->m_rightStickY) < 0.1f)
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadRightStickMoved(m_gamepad->m_rightStickX, 0);

			++it;
		}
	}
	else
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->OnGamePadRightStickMoved(0, 0);

			++it;
		}
	}
}
