#include "stdafx.h"
#include "GamePad.h"

GamePad::GamePad() : m_deadzoneX(0.05f), m_deadzoneY(0.02f), m_cId(-1), m_state(0)
{
}

GamePad::GamePad(float dzX, float dzY) : m_deadzoneX(dzX), m_deadzoneY(dzY), m_cId(-1), m_state(0)
{
}

int GamePad::GetPort()
{
	return m_cId + 1;
}

XINPUT_GAMEPAD* GamePad::GetState()
{
	return &m_state.Gamepad;;
}

bool GamePad::CheckConnection()
{
	int controllerId = -1;

	for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			controllerId = i;
	}

	m_cId = controllerId;

	return controllerId != -1;
}

bool GamePad::Refresh()
{
	if (m_cId == -1)
		CheckConnection();

	if (m_cId != -1)
	{
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));
		if (XInputGetState(m_cId, &m_state) != ERROR_SUCCESS)
		{
			m_cId = -1;
			return false;
		}

		float normLX = fmaxf(-1, (float)m_state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)m_state.Gamepad.sThumbLY / 32767);

		m_leftStickX = (abs(normLX) < m_deadzoneX ? 0 : (abs(normLX) - m_deadzoneX) * (normLX / abs(normLX)));
		m_leftStickY = (abs(normLY) < m_deadzoneY ? 0 : (abs(normLY) - m_deadzoneY) * (normLY / abs(normLY)));

		if (m_deadzoneX > 0) m_leftStickX *= 1 / (1 - m_deadzoneX);
		if (m_deadzoneY > 0) m_leftStickY *= 1 / (1 - m_deadzoneY);

		float normRX = fmaxf(-1, (float)m_state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)m_state.Gamepad.sThumbRY / 32767);

		m_rightStickX = (abs(normRX) < m_deadzoneX ? 0 : (abs(normRX) - m_deadzoneX) * (normRX / abs(normRX)));
		m_rightStickY = (abs(normRY) < m_deadzoneY ? 0 : (abs(normRY) - m_deadzoneY) * (normRY / abs(normRY)));

		if (m_deadzoneX > 0) m_rightStickX *= 1 / (1 - m_deadzoneX);
		if (m_deadzoneY > 0) m_rightStickY *= 1 / (1 - m_deadzoneY);

		m_leftTrigger = (float)m_state.Gamepad.bLeftTrigger / 255;
		m_rightTrigger = (float)m_state.Gamepad.bRightTrigger / 255;

		return true;
	}

	return false;
}

bool GamePad::IsPressed(WORD button) const
{
	return (m_state.Gamepad.wButtons & button) != 0;
}
