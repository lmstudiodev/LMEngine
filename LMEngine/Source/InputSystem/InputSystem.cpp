#include "stdafx.h"
#include "InputSystem.h"

InputSystem* InputSystem::m_inputSystem = nullptr;

InputSystem::InputSystem()
{
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
	if (GetKeyboardState(m_keys_state))
	{
		for (unsigned int i = 0; i < 255; i++)
		{
			if (m_keys_state[i] & 0x80)
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

		memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
	}
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
			//(*it)->OnMouseMove(Point::GetDelta(p, m_old_mouse_pos));
			(*it)->OnMouseMove(Point(p.x, p.y));
			++it;
		}
	}

	m_old_mouse_pos = Point(p.x, p.y);
}
