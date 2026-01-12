
class MainWindow
{
public:
	MainWindow();
	~MainWindow();

public:
	bool IsRunnig();

	RECT GetClientWindowRect();
	RECT GetScreenSize();

	virtual void OnUpdate();
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnFocus() = 0;
	virtual void OnKillFocus() = 0;
	virtual void OnSize();

private:
	void Broadcast();

protected:
	HWND m_hwnd;
	bool m_isRunning;
	bool m_isInitialized;
	RECT m_wndRect;
};


