#include <Rect.h>
#include <Prerequisites.h>

class MainWindow
{
public:
	MainWindow();
	virtual ~MainWindow();

public:
	Rect getClientSize();
	Rect getScreenSize();

	virtual void OnUpdate();
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnFocus();
	virtual void OnKillFocus();
	virtual void OnSize();

protected:
	void* m_hwnd;
	Rect m_size;
};


