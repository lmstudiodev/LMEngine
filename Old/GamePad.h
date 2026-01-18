#pragma once
#include <Xinput.h>

class GamePad
{
public:
	GamePad();
	GamePad(float dzX, float dzY);

public:
	int  GetPort();
	XINPUT_GAMEPAD* GetState();
	bool CheckConnection();
	bool Refresh();
	bool IsPressed(WORD button) const;

public:
	float m_leftStickX = 0;
	float m_leftStickY = 0;
	float m_rightStickX = 0;
	float m_rightStickY = 0;
	float m_leftTrigger = 0;
	float m_rightTrigger = 0;

private:
	int m_cId;
	XINPUT_STATE m_state;

	float m_deadzoneX;
	float m_deadzoneY;
};

