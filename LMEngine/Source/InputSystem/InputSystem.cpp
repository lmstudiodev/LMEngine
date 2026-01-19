#include <stdafx.h>
#include <InputSystem/InputSystem.h>

InputSystem::InputSystem()
{

}

InputSystem::~InputSystem()
{

}

short InputSystem::getInternalKeyCode(const Key& key)
{
	short winKey = 0;

	if (key >= Key::A && key <= Key::Z)
	{
		winKey = 'A' + ((short)key - (short)Key::A);
	}
	else if (key >= Key::_0 && key <= Key::_9)
	{
		winKey = '0' + ((short)key - (short)Key::_0);
	}
	else if (key >= Key::LeftMouseButton)
	{
		winKey = VK_LBUTTON;
	}
	else if (key >= Key::MiddleMouseButton)
	{
		winKey = VK_MBUTTON;
	}
	else if (key >= Key::RightMouseButton)
	{
		winKey = VK_RBUTTON;
	}
	else if (key == Key::Shift)
	{
		winKey = VK_SHIFT;
	}
	else if (key == Key::Escape)
	{
		winKey = VK_ESCAPE;
	}
	else if (key == Key::Space)
	{
		winKey = VK_SPACE;
	}
	else if (key == Key::Enter)
	{
		winKey = VK_RETURN;
	}

	return winKey;
}

void InputSystem::Update()
{
	UpdateMouseInput();
	UpdateKeyboardInput();
}

bool InputSystem::isKeyDown(const Key& key)
{
	return (m_final_keys_state[getInternalKeyCode(key)] == 0);
}

bool InputSystem::isKeyUp(const Key& key)
{
	return (m_final_keys_state[getInternalKeyCode(key)] == 1);
}

void InputSystem::lockMouseCursor(bool lock)
{
	m_cursorLocked = lock;
}

void InputSystem::setLockArea(const Rect& area)
{
	m_lock_area = area;
	m_lock_area_center = Vector2D((float)area.left + ((float)area.width / 2.0f), (float)area.top + ((float)area.height / 2.0f));
}

Vector2D InputSystem::getDeltaMousePosition()
{
	return m_delta_mouse_pos;
}

void InputSystem::UpdateKeyboardInput()
{
	for (unsigned int i = 0; i < 255; i++)
	{
		m_keys_state[i] = GetAsyncKeyState(i);
		
		//KEY DOWN
		if (m_keys_state[i] & 0x8001)
		{
			m_final_keys_state[i] = 0;
		}
		else
		{
			// KEY UP
			if (m_keys_state[i] != m_old_keys_state[i])
			{
				m_final_keys_state[i] = 1;
			}
			else
			{
				m_final_keys_state[i] = 2;
			}
		}
	}

	memcpy(m_old_keys_state, m_keys_state, sizeof(short) * 256);
}

void InputSystem::UpdateMouseInput()
{
	POINT currentMousePos{};
	GetCursorPos(&currentMousePos);
	
	if (currentMousePos.x != m_old_mouse_pos.m_x || currentMousePos.y != m_old_mouse_pos.m_y)
	{
		float deltaX = (float)(currentMousePos.x - m_old_mouse_pos.m_x);
		float deltaY = (float)(currentMousePos.y - m_old_mouse_pos.m_y);

		m_delta_mouse_pos = Vector2D(deltaX, deltaY);
	}
	else
	{
		m_delta_mouse_pos = Vector2D(0, 0);
	}

	if (!m_cursorLocked)
	{
		m_old_mouse_pos = Vector2D(currentMousePos.x, currentMousePos.y);
	}
	else
	{
		SetCursorPos((int)m_lock_area_center.m_x, (int)m_lock_area_center.m_y);
		m_old_mouse_pos = m_lock_area_center;
	}
}

//void InputSystem::UpdateGamePadButton()
//{
//
//	if (m_gamepad->IsPressed(XINPUT_GAMEPAD_A))
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadButtonAPressed();
//
//			++it;
//		}
//	}
//	else if (m_gamepad->IsPressed(XINPUT_GAMEPAD_Y))
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadButtonYPressed();
//
//			++it;
//		}
//	}
//	else if (m_gamepad->IsPressed(XINPUT_GAMEPAD_X))
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadButtonXPressed();
//
//			++it;
//		}
//	}
//	else if (m_gamepad->IsPressed(XINPUT_GAMEPAD_B))
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadButtonBPressed();
//
//			++it;
//		}
//	}
//	
//	if (m_gamepad->IsPressed(XINPUT_GAMEPAD_LEFT_THUMB))
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadLeftThumbPressed(true);
//
//			++it;
//		}
//	}
//	else
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadLeftThumbPressed(false);
//
//			++it;
//		}
//	}
//}

//void InputSystem::UpdateGamePadLeftStick()
//{
//	if (abs(m_gamepad->m_leftStickX) >= 0.1f)
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadLeftStickXChanged(m_gamepad->m_leftStickX);
//
//			++it;
//		}
//	}
//	else
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadLeftStickXChanged(0);
//
//			++it;
//		}
//	}
//
//	if (abs(m_gamepad->m_leftStickY) >= 0.1f)
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadLeftStickYChanged(m_gamepad->m_leftStickY);
//
//			++it;
//		}
//	}
//	else
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadLeftStickYChanged(0);
//
//			++it;
//		}
//	}
//}

//void InputSystem::UpdateGamePadRightStick()
//{
//	if (abs(m_gamepad->m_rightStickX) >= 0.1f && abs(m_gamepad->m_rightStickY) >= 0.1f)
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadRightStickMoved(m_gamepad->m_rightStickX, m_gamepad->m_rightStickY);
//
//			++it;
//		}
//	}
//	else if (abs(m_gamepad->m_rightStickX) < 0.1f && abs(m_gamepad->m_rightStickY) >= 0.1f)
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadRightStickMoved(0, m_gamepad->m_rightStickY);
//
//			++it;
//		}
//	}
//	else if (abs(m_gamepad->m_rightStickX) >= 0.1f && abs(m_gamepad->m_rightStickY) < 0.1f)
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadRightStickMoved(m_gamepad->m_rightStickX, 0);
//
//			++it;
//		}
//	}
//	else
//	{
//		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
//
//		while (it != m_set_listeners.end())
//		{
//			(*it)->OnGamePadRightStickMoved(0, 0);
//
//			++it;
//		}
//	}
//}
