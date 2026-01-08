#pragma once
#include <Windows.h>

class MainWindow
{
public:
	MainWindow();
	~MainWindow();

public:
	bool Init();
	bool Broadcast();
	bool Release() const;
	bool IsRunnig() const;

	RECT GetClientWindowRect();
	void SetWindowHandle(HWND hwnd);

	virtual void OnUpdate();
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnFocus() = 0;
	virtual void OnKillFocus() = 0;

protected:
	HWND m_hwnd;
	bool m_isRunning;
	RECT m_wndRect;
};


