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

	virtual void onUpdate();
	virtual void onCreate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize(const Rect& size);

protected:
	void* m_hwnd;
	Rect m_size;
};


