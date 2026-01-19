#include <stdafx.h>
#include <WindowSystem/MainWindow.h>

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_SIZE:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window)
			window->onSize(window->getClientSize());
		break;
	}
	case WM_DESTROY:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_SETFOCUS:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(window)
			window->onFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

MainWindow::MainWindow() : m_hwnd(nullptr), m_size{0,0,1920,1280}
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"LMEWindow";
	wc.lpfnWndProc = WndProc;

	auto classId = RegisterClassEx(&wc);

	if (!classId)
		Dx3DError("Register Window Class failed.");

	RECT rc{ 0, 0, m_size.width, m_size.height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	m_hwnd = CreateWindowEx(NULL,
		MAKEINTATOM(classId),
		L"D3DX LMEngine",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL, NULL, NULL, NULL);

	if (!m_hwnd)
		Dx3DError("[LMEngine Error] Window creation failed.");

	auto hwnd = static_cast<HWND>(m_hwnd);

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
}

MainWindow::~MainWindow()
{
	DestroyWindow(static_cast<HWND>(m_hwnd));
}

Rect MainWindow::getClientSize()
{
	RECT rc{};

	auto hwnd = static_cast<HWND>(m_hwnd);

	GetClientRect(hwnd, &rc);
	ClientToScreen(hwnd, (LPPOINT)&rc.left);
	ClientToScreen(hwnd, (LPPOINT)&rc.right);

	return { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}

Rect MainWindow::getScreenSize()
{
	RECT rc{};

	rc.right = GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = GetSystemMetrics(SM_CYSCREEN);
	return { 0, 0, rc.right - rc.left, rc.bottom - rc.top };
}

void MainWindow::onDestroy()
{

}

void MainWindow::onFocus()
{
}

void MainWindow::onKillFocus()
{
}

void MainWindow::onCreate()
{
	
}

void MainWindow::onUpdate()
{

}


void MainWindow::onSize(const Rect& size)
{

}
